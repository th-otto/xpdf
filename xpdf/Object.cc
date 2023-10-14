//========================================================================
//
// Object.cc
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#include <xpdf/aconf.h>

#include <stddef.h>
#include "../goo/gmempp.h"
#include "xpdf/xpdf/Object.h"
#include "xpdf/xpdf/Array.h"
#include "xpdf/xpdf/Dict.h"
#include "xpdf/xpdf/Error.h"
#include "xpdf/xpdf/Stream.h"
#include "xpdf/xpdf/XRef.h"

//------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------

static const char *const objTypeNames[numObjTypes] = {
  "boolean",
  "integer",
  "real",
  "string",
  "name",
  "null",
  "array",
  "dictionary",
  "stream",
  "ref",
  "cmd",
  "error",
  "eof",
  "none"
};

#ifdef DEBUG_MEM
#ifdef MULTITHREADED
GAtomicCounter Object::numAlloc[numObjTypes] =
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
long Object::numAlloc[numObjTypes] =
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
#endif // DEBUG_MEM

Object *Object::initArray(XRef *xref) {
  initObj(objArray);
  array = new Array(xref);
  return this;
}

Object *Object::initDict(XRef *xref) {
  initObj(objDict);
  dict = new Dict(xref);
  return this;
}

Object *Object::initDict(Dict *dictA) {
  initObj(objDict);
  dict = dictA;
  dict->incRef();
  return this;
}

Object *Object::initStream(Stream *streamA) {
  initObj(objStream);
  stream = streamA;
  return this;
}

Object *Object::copy(Object *obj) {
  *obj = *this;
  switch (type) {
  case objString:
    obj->string = string->copy();
    break;
  case objName:
    obj->name = copyString(name);
    break;
  case objArray:
    array->incRef();
    break;
  case objDict:
    dict->incRef();
    break;
  case objStream:
    obj->stream = stream->copy();
    break;
  case objCmd:
    obj->cmd = copyString(cmd);
    break;
  default:
    break;
  }
#ifdef DEBUG_MEM
#ifdef MULTITHREADED
  gAtomicIncrement(&numAlloc[type]);
#else
  ++numAlloc[type];
#endif
#endif
  return obj;
}

Object *Object::fetch(XRef *xref, Object *obj, int recursion) {
  return (type == objRef && xref) ?
         xref->fetch(ref.num, ref.gen, obj, recursion) : copy(obj);
}

void Object::free() {
  switch (type) {
  case objString:
    delete string;
    break;
  case objName:
    gfree(name);
    break;
  case objArray:
    if (!array->decRef()) {
      delete array;
    }
    break;
  case objDict:
    if (!dict->decRef()) {
      delete dict;
    }
    break;
  case objStream:
    delete stream;
    break;
  case objCmd:
    gfree(cmd);
    break;
  default:
    break;
  }
#ifdef DEBUG_MEM
#ifdef MULTITHREADED
  gAtomicDecrement(&numAlloc[type]);
#else
  --numAlloc[type];
#endif
#endif
  type = objNone;
}

const char *Object::getTypeName() {
  return objTypeNames[type];
}

void Object::print(FILE *f) {
  Object obj;
  int i;

  switch (type) {
  case objBool:
    fprintf(f, "%s", booln ? "true" : "false");
    break;
  case objInt:
    fprintf(f, "%d", intg);
    break;
  case objReal:
    fprintf(f, "%g", real);
    break;
  case objString:
    fprintf(f, "(");
    fwrite(string->getCString(), 1, string->getLength(), f);
    fprintf(f, ")");
    break;
  case objName:
    fprintf(f, "/%s", name);
    break;
  case objNull:
    fprintf(f, "null");
    break;
  case objArray:
    fprintf(f, "[");
    for (i = 0; i < arrayGetLength(); ++i) {
      if (i > 0)
	fprintf(f, " ");
      arrayGetNF(i, &obj);
      obj.print(f);
      obj.free();
    }
    fprintf(f, "]");
    break;
  case objDict:
    fprintf(f, "<<");
    for (i = 0; i < dictGetLength(); ++i) {
      fprintf(f, " /%s ", dictGetKey(i));
      dictGetValNF(i, &obj);
      obj.print(f);
      obj.free();
    }
    fprintf(f, " >>");
    break;
  case objStream:
    fprintf(f, "<stream>");
    break;
  case objRef:
    fprintf(f, "%d %d R", ref.num, ref.gen);
    break;
  case objCmd:
    fprintf(f, "%s", cmd);
    break;
  case objError:
    fprintf(f, "<error>");
    break;
  case objEOF:
    fprintf(f, "<EOF>");
    break;
  case objNone:
    fprintf(f, "<none>");
    break;
  }
}

void Object::memCheck(FILE *f) {
#ifdef DEBUG_MEM
  int i;
  long t;

  t = 0;
  for (i = 0; i < numObjTypes; ++i) {
    t += numAlloc[i];
  }
  if (t > 0) {
    fprintf(f, "Allocated objects:\n");
    for (i = 0; i < numObjTypes; ++i) {
      if (numAlloc[i] > 0) {
	fprintf(f, "  %-20s: %6ld\n", objTypeNames[i], numAlloc[i]);
      }
    }
  }
#endif
  (void) f;
}
