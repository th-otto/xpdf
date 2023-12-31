//========================================================================
//
// Array.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef XPDF_ARRAY_H
#define XPDF_ARRAY_H

#include <xpdf/aconf.h>

#ifdef XPDF_MULTITHREADED
#include "xpdf/goo/GMutex.h"
#endif
#include "xpdf/xpdf/Object.h"

class XRef;

//------------------------------------------------------------------------
// Array
//------------------------------------------------------------------------

class Array {
public:

  // Constructor.
  Array(XRef *xrefA);

  // Destructor.
  ~Array();

  // Reference counting.
#ifdef XPDF_MULTITHREADED
  long incRef() { return gAtomicIncrement(&ref); }
  long decRef() { return gAtomicDecrement(&ref); }
#else
  long incRef() { return ++ref; }
  long decRef() { return --ref; }
#endif

  // Get number of elements.
  int getLength() { return length; }

  // Add an element.
  void add(Object *elem);

  // Accessors.
  Object *get(int i, Object *obj, int recursion = 0);
  Object *getNF(int i, Object *obj);

private:

  XRef *xref;			// the xref table for this PDF file
  Object *elems;		// array of elements
  int size;			// size of <elems> array
  int length;			// number of elements in array
#ifdef XPDF_MULTITHREADED
  GAtomicCounter ref;		// reference count
#else
  long ref;			// reference count
#endif
};

#endif
