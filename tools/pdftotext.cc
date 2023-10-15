//========================================================================
//
// pdftotext.cc
//
// Copyright 1997-2013 Glyph & Cog, LLC
//
//========================================================================

#include "xpdf/xpdfbuild.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef DEBUG_FP_LINUX
#  include <fenv.h>
#  include <fpu_control.h>
#endif
#include "xpdf/goo/gmem.h"
#include "../goo/gmempp.h"
#include "../goo/parseargs.h"
#include "xpdf/goo/GString.h"
#include "xpdf/goo/GList.h"
#include "xpdf/xpdf/GlobalParams.h"
#include "xpdf/xpdf/Object.h"
#include "xpdf/xpdf/Stream.h"
#include "xpdf/xpdf/Array.h"
#include "xpdf/xpdf/Dict.h"
#include "xpdf/xpdf/XRef.h"
#include "xpdf/xpdf/Catalog.h"
#include "xpdf/xpdf/Page.h"
#include "xpdf/xpdf/PDFDoc.h"
#include "xpdf/xpdf/TextOutputDev.h"
#include "xpdf/xpdf/CharTypes.h"
#include "xpdf/xpdf/UnicodeMap.h"
#include "xpdf/xpdf/TextString.h"
#include "xpdf/xpdf/Error.h"
#include "xpdf/xpdf/config.h"

static int firstPage;
static int lastPage;
static GBool physLayout;
static GBool simpleLayout;
static GBool simple2Layout;
static GBool tableLayout;
static GBool linePrinter;
static GBool rawOrder;
static double fixedPitch;
static double fixedLineSpacing;
static GBool clipText;
static GBool discardDiag;
static char textEncName[128];
static char textEOL[16];
static GBool noPageBreaks;
static GBool insertBOM;
static double marginLeft;
static double marginRight;
static double marginTop;
static double marginBottom;
static char ownerPassword[33];
static char userPassword[33];
static GBool verbose;
static GBool quiet;
static char cfgFileName[256];
static GBool listEncodings;
static GBool printVersion;
static GBool printHelp;

static ArgDesc const argDesc[] = {
  {"-f",       argInt,      &firstPage,     0,
   "first page to convert"},
  {"-l",       argInt,      &lastPage,      0,
   "last page to convert"},
  {"-layout",  argFlag,     &physLayout,    0,
   "maintain original physical layout"},
  {"-simple",  argFlag,     &simpleLayout,  0,
   "simple one-column page layout"},
  {"-simple2", argFlag,     &simple2Layout, 0,
   "simple one-column page layout, version 2"},
  {"-table",   argFlag,     &tableLayout,   0,
   "similar to -layout, but optimized for tables"},
  {"-lineprinter", argFlag, &linePrinter,   0,
   "use strict fixed-pitch/height layout"},
  {"-raw",     argFlag,     &rawOrder,      0,
   "keep strings in content stream order"},
  {"-fixed",   argFP,       &fixedPitch,    0,
   "assume fixed-pitch (or tabular) text"},
  {"-linespacing", argFP,   &fixedLineSpacing, 0,
   "fixed line spacing for LinePrinter mode"},
  {"-clip",    argFlag,     &clipText,      0,
   "separate clipped text"},
  {"-nodiag",  argFlag,     &discardDiag,   0,
   "discard diagonal text"},
  {"-enc",     argString,   textEncName,    sizeof(textEncName),
   "output text encoding name"},
  {"-eol",     argString,   textEOL,        sizeof(textEOL),
   "output end-of-line convention (unix, dos, or mac)"},
  {"-nopgbrk", argFlag,     &noPageBreaks,  0,
   "don't insert a page break at the end of each page"},
  {"-bom",     argFlag,     &insertBOM,     0,
   "insert a Unicode BOM at the start of the text file"},
  {"-marginl", argFP,       &marginLeft,    0,
   "left page margin"},
  {"-marginr", argFP,       &marginRight,   0,
   "right page margin"},
  {"-margint", argFP,       &marginTop,     0,
   "top page margin"},
  {"-marginb", argFP,       &marginBottom,  0,
   "bottom page margin"},
  {"-opw",     argString,   ownerPassword,  sizeof(ownerPassword),
   "owner password (for encrypted files)"},
  {"-upw",     argString,   userPassword,   sizeof(userPassword),
   "user password (for encrypted files)"},
  {"-verbose", argFlag,    &verbose,       0,
   "print per-page status information"},
  {"-q",       argFlag,     &quiet,         0,
   "don't print any messages or errors"},
  {"-cfg",     argString,   cfgFileName,    sizeof(cfgFileName),
   "configuration file to use in place of .xpdfrc"},
  {"-listencodings", argFlag, &listEncodings, 0,
   "list all available output text encodings"},
  {"-v",       argFlag,     &printVersion,  0,
   "print copyright and version info"},
  {"-h",       argFlag,     &printHelp,     0,
   "print usage information"},
  {"-help",    argFlag,     &printHelp,     0,
   "print usage information"},
  {"--help",   argFlag,     &printHelp,     0,
   "print usage information"},
  {"-?",       argFlag,     &printHelp,     0,
   "print usage information"},
  {NULL, argFlag, 0, 0, 0}
};

#include "xpdftools.h"

int main(int argc, char *argv[]) {
  PDFDoc *doc;
  char *fileName;
  GString *textFileName;
  GString *ownerPW, *userPW;
  TextOutputControl textOutControl;
  TextOutputDev *textOut;
  UnicodeMap *uMap;
  GBool ok;
  char *p;
  int exitCode;

#ifdef DEBUG_FP_LINUX
  // enable exceptions on floating point div-by-zero
  feenableexcept(FE_DIVBYZERO);
  // force 64-bit rounding: this avoids changes in output when minor
  // code changes result in spills of x87 registers; it also avoids
  // differences in output with valgrind's 64-bit floating point
  // emulation (yes, this is a kludge; but it's pretty much
  // unavoidable given the x87 instruction set; see gcc bug 323 for
  // more info)
  fpu_control_t cw;
  _FPU_GETCW(cw);
  cw = (fpu_control_t)((cw & ~_FPU_EXTENDED) | _FPU_DOUBLE);
  _FPU_SETCW(cw);
#endif

  exitCode = 99;

  firstPage = 1;
  lastPage = 0;
  physLayout = gFalse;
  simpleLayout = gFalse;
  simple2Layout = gFalse;
  tableLayout = gFalse;
  linePrinter = gFalse;
  rawOrder = gFalse;
  fixedPitch = 0;
  fixedLineSpacing = 0;
  clipText = gFalse;
  discardDiag = gFalse;
  textEncName[0] = '\0';
  textEOL[0] = '\0';
  noPageBreaks = gFalse;
  insertBOM = gFalse;
  marginLeft = 0;
  marginRight = 0;
  marginTop = 0;
  marginBottom = 0;
  verbose = gFalse;
  quiet = gFalse;
  ownerPassword[0] = '\001';
  userPassword[0] = '\001';
  cfgFileName[0] = '\0';
  listEncodings = gFalse;
  printVersion = gFalse;
  printHelp = gFalse;

  // parse args
  fixCommandLine(&argc, &argv);
  ok = parseArgs(argDesc, &argc, argv);
  if (ok && listEncodings) {
    // list available encodings
    if (cfgFileName[0] && !pathIsFile(cfgFileName)) {
      error(errConfig, -1, "Config file '{0:s}' doesn't exist or isn't a file",
	    cfgFileName);
    }
    globalParams = new GlobalParams(cfgFileName);
    GList *encs = globalParams->getAvailableTextEncodings();
    for (int i = 0; i < encs->getLength(); ++i) {
      printf("%s\n", ((GString *)encs->get(i))->getCString());
    }
    deleteGList(encs, GString);
    delete globalParams;
    goto err0;
  }
  if (!ok || argc < 2 || argc > 3 || printVersion || printHelp) {
    fprintf(stderr, "pdftotext version %s [www.xpdfreader.com]\n", xpdfVersion);
    fprintf(stderr, "%s\n", xpdfCopyright);
    if (!printVersion) {
      printUsage("pdftotext", "<PDF-file> [<text-file>]", argDesc);
    }
    goto err0;
  }
  fileName = argv[1];

  // read config file
  if (cfgFileName[0] && !pathIsFile(cfgFileName)) {
    error(errConfig, -1, "Config file '{0:s}' doesn't exist or isn't a file",
	  cfgFileName);
  }
  globalParams = new GlobalParams(cfgFileName);
  if (textEncName[0]) {
    globalParams->setTextEncoding(textEncName);
  }
  if (textEOL[0]) {
    if (!globalParams->setTextEOL(textEOL)) {
      fprintf(stderr, "Bad '-eol' value on command line\n");
    }
  }
  if (noPageBreaks) {
    globalParams->setTextPageBreaks(gFalse);
  }
  if (verbose) {
    globalParams->setPrintStatusInfo(verbose);
  }
  if (quiet) {
    globalParams->setErrQuiet(quiet);
  }

  // get mapping to output encoding
  if (!(uMap = globalParams->getTextEncoding())) {
    error(errConfig, -1, "Couldn't get text encoding");
    goto err1;
  }

  // open PDF file
  if (ownerPassword[0] != '\001') {
    ownerPW = new GString(ownerPassword);
  } else {
    ownerPW = NULL;
  }
  if (userPassword[0] != '\001') {
    userPW = new GString(userPassword);
  } else {
    userPW = NULL;
  }
  doc = new PDFDoc(fileName, ownerPW, userPW);
  if (userPW) {
    delete userPW;
  }
  if (ownerPW) {
    delete ownerPW;
  }
  if (!doc->isOk()) {
    exitCode = 1;
    goto err2;
  }

#if 0
  // check for copy permission
  if (!doc->okToCopy()) {
    error(errNotAllowed, -1,
	  "Copying of text from this document is not allowed.");
    exitCode = 3;
    goto err2;
  }
#endif

  // construct text file name
  if (argc == 3) {
    textFileName = new GString(argv[2]);
  } else {
    p = fileName + strlen(fileName) - 4;
    if (strlen(fileName) > 4 && (!strcmp(p, ".pdf") || !strcmp(p, ".PDF"))) {
      textFileName = new GString(fileName, (int)strlen(fileName) - 4);
    } else {
      textFileName = new GString(fileName);
    }
    textFileName->append(".txt");
  }
  if (textFileName->cmp("-") == 0) {
    globalParams->setPrintStatusInfo(gFalse);
  }

  // get page range
  if (firstPage < 1) {
    firstPage = 1;
  }
  if (lastPage < 1 || lastPage > doc->getNumPages()) {
    lastPage = doc->getNumPages();
  }

  // write text file
  if (tableLayout) {
    textOutControl.mode = textOutTableLayout;
    textOutControl.fixedPitch = fixedPitch;
  } else if (physLayout) {
    textOutControl.mode = textOutPhysLayout;
    textOutControl.fixedPitch = fixedPitch;
  } else if (simpleLayout) {
    textOutControl.mode = textOutSimpleLayout;
  } else if (simple2Layout) {
    textOutControl.mode = textOutSimple2Layout;
  } else if (linePrinter) {
    textOutControl.mode = textOutLinePrinter;
    textOutControl.fixedPitch = fixedPitch;
    textOutControl.fixedLineSpacing = fixedLineSpacing;
  } else if (rawOrder) {
    textOutControl.mode = textOutRawOrder;
  } else {
    textOutControl.mode = textOutReadingOrder;
  }
  textOutControl.clipText = clipText;
  textOutControl.discardDiagonalText = discardDiag;
  textOutControl.insertBOM = insertBOM;
  textOutControl.marginLeft = marginLeft;
  textOutControl.marginRight = marginRight;
  textOutControl.marginTop = marginTop;
  textOutControl.marginBottom = marginBottom;
  textOut = new TextOutputDev(textFileName->getCString(), &textOutControl,
			      gFalse, gTrue);
  if (textOut->isOk()) {
    doc->displayPages(textOut, firstPage, lastPage, 72, 72, 0,
		      gFalse, gTrue, gFalse);
  } else {
    delete textOut;
    exitCode = 2;
    goto err3;
  }
  delete textOut;

  exitCode = 0;

  // clean up
 err3:
  delete textFileName;
 err2:
  delete doc;
  uMap->decRefCnt();
 err1:
  delete globalParams;
 err0:

  // check for memory leaks
  Object::memCheck(stderr);
  gMemReport(stderr);

  return exitCode;
}