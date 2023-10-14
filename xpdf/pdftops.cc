//========================================================================
//
// pdftops.cc
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#include <xpdf/aconf.h>
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
#include "xpdf/xpdf/GlobalParams.h"
#include "xpdf/xpdf/Object.h"
#include "xpdf/xpdf/Stream.h"
#include "xpdf/xpdf/Array.h"
#include "xpdf/xpdf/Dict.h"
#include "xpdf/xpdf/XRef.h"
#include "xpdf/xpdf/Catalog.h"
#include "xpdf/xpdf/Page.h"
#include "xpdf/xpdf/PDFDoc.h"
#include "PSOutputDev.h"
#include "xpdf/xpdf/Error.h"
#include "xpdf/xpdf/config.h"

static int firstPage;
static int lastPage;
static GBool level1;
static GBool level1Sep;
static GBool level2;
static GBool level2Gray;
static GBool level2Sep;
static GBool level3;
static GBool level3Gray;
static GBool level3Sep;
static GBool doEPS;
static GBool doForm;
#ifdef OPI_SUPPORT
static GBool doOPI = gFalse;
#endif
static GBool noEmbedT1Fonts;
static GBool noEmbedTTFonts;
static GBool noEmbedCIDPSFonts;
static GBool noEmbedCIDTTFonts;
static GBool preload;
static char paperSize[15];
static int paperWidth;
static int paperHeight;
static GBool noCrop;
static GBool expand;
static GBool noShrink;
static GBool noCenter;
static GBool pageCrop;
static GBool userUnit;
static GBool duplex;
static char ownerPassword[33];
static char userPassword[33];
static GBool verbose;
static GBool quiet;
static char cfgFileName[256];
static GBool printVersion;
static GBool printHelp;

static ArgDesc const argDesc[] = {
  {"-f",          argInt,      &firstPage,      0,
   "first page to print"},
  {"-l",          argInt,      &lastPage,       0,
   "last page to print"},
  {"-level1",     argFlag,     &level1,         0,
   "generate Level 1 PostScript"},
  {"-level1sep",  argFlag,     &level1Sep,      0,
   "generate Level 1 separable PostScript"},
  {"-level2",     argFlag,     &level2,         0,
   "generate Level 2 PostScript"},
  {"-level2gray", argFlag,     &level2Gray,     0,
   "generate Level 2 grayscale PostScript"},
  {"-level2sep",  argFlag,     &level2Sep,      0,
   "generate Level 2 separable PostScript"},
  {"-level3",     argFlag,     &level3,         0,
   "generate Level 3 PostScript"},
  {"-level3gray", argFlag,     &level3Gray,     0,
   "generate Level 3 grayscale PostScript"},
  {"-level3sep",  argFlag,     &level3Sep,      0,
   "generate Level 3 separable PostScript"},
  {"-eps",        argFlag,     &doEPS,          0,
   "generate Encapsulated PostScript (EPS)"},
  {"-form",       argFlag,     &doForm,         0,
   "generate a PostScript form"},
#ifdef OPI_SUPPORT
  {"-opi",        argFlag,     &doOPI,          0,
   "generate OPI comments"},
#endif
  {"-noembt1",    argFlag,     &noEmbedT1Fonts, 0,
   "don't embed Type 1 fonts"},
  {"-noembtt",    argFlag,     &noEmbedTTFonts, 0,
   "don't embed TrueType fonts"},
  {"-noembcidps", argFlag,     &noEmbedCIDPSFonts, 0,
   "don't embed CID PostScript fonts"},
  {"-noembcidtt", argFlag,     &noEmbedCIDTTFonts,  0,
   "don't embed CID TrueType fonts"},
  {"-preload",    argFlag,     &preload,        0,
   "preload images and forms"},
  {"-paper",      argString,   paperSize,       sizeof(paperSize),
   "paper size (letter, legal, A4, A3, match)"},
  {"-paperw",     argInt,      &paperWidth,     0,
   "paper width, in points"},
  {"-paperh",     argInt,      &paperHeight,    0,
   "paper height, in points"},
  {"-nocrop",     argFlag,     &noCrop,         0,
   "don't crop pages to CropBox"},
  {"-expand",     argFlag,     &expand,         0,
   "expand pages smaller than the paper size"},
  {"-noshrink",   argFlag,     &noShrink,       0,
   "don't shrink pages larger than the paper size"},
  {"-nocenter",   argFlag,     &noCenter,       0,
   "don't center pages smaller than the paper size"},
  {"-pagecrop",   argFlag,     &pageCrop,       0,
   "treat the CropBox as the page size"},
  {"-userunit",   argFlag,     &userUnit,       0,
   "honor the UserUnit"},
  {"-duplex",     argFlag,     &duplex,         0,
   "enable duplex printing"},
  {"-opw",        argString,   ownerPassword,   sizeof(ownerPassword),
   "owner password (for encrypted files)"},
  {"-upw",        argString,   userPassword,    sizeof(userPassword),
   "user password (for encrypted files)"},
  {"-verbose", argFlag,    &verbose,       0,
   "print per-page status information"},
  {"-q",          argFlag,     &quiet,          0,
   "don't print any messages or errors"},
  {"-cfg",        argString,      cfgFileName,    sizeof(cfgFileName),
   "configuration file to use in place of .xpdfrc"},
  {"-v",          argFlag,     &printVersion,   0,
   "print copyright and version info"},
  {"-h",          argFlag,     &printHelp,      0,
   "print usage information"},
  {"-help",       argFlag,     &printHelp,      0,
   "print usage information"},
  {"--help",      argFlag,     &printHelp,      0,
   "print usage information"},
  {"-?",          argFlag,     &printHelp,      0,
   "print usage information"},
  {NULL, argFlag, 0, 0, 0 }
};

#include "xpdftools.h"

int main(int argc, char *argv[]) {
  PDFDoc *doc;
  char *fileName;
  GString *psFileName;
  PSLevel level;
  PSOutMode mode;
  GString *ownerPW, *userPW;
  PSOutputDev *psOut;
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
  level1 = gFalse;
  level1Sep = gFalse;
  level2 = gFalse;
  level2Gray = gFalse;
  level2Sep = gFalse;
  level3 = gFalse;
  level3Gray = gFalse;
  level3Sep = gFalse;
  doEPS = gFalse;
  doForm = gFalse;
#ifdef OPI_SUPPORT
  doOPI = gFalse;
#endif
  noEmbedT1Fonts = gFalse;
  noEmbedTTFonts = gFalse;
  noEmbedCIDPSFonts = gFalse;
  noEmbedCIDTTFonts = gFalse;
  preload = gFalse;
  paperSize[0] = '\0';
  paperWidth = 0;
  paperHeight = 0;
  noCrop = gFalse;
  expand = gFalse;
  noShrink = gFalse;
  noCenter = gFalse;
  pageCrop = gFalse;
  userUnit = gFalse;
  duplex = gFalse;
  verbose = gFalse;
  quiet = gFalse;
  ownerPassword[0] = '\001';
  userPassword[0] = '\001';
  cfgFileName[0] = '\0';
  printVersion = gFalse;
  printHelp = gFalse;

  // parse args
  fixCommandLine(&argc, &argv);
  ok = parseArgs(argDesc, &argc, argv);
  if (!ok || argc < 2 || argc > 3 || printVersion || printHelp) {
    fprintf(stderr, "pdftops version %s [www.xpdfreader.com]\n", xpdfVersion);
    fprintf(stderr, "%s\n", xpdfCopyright);
    if (!printVersion) {
      printUsage("pdftops", "<PDF-file> [<PS-file>]", argDesc);
    }
    exit(1);
  }
  if ((level1 ? 1 : 0) +
      (level1Sep ? 1 : 0) +
      (level2 ? 1 : 0) +
      (level2Gray ? 1 : 0) +
      (level2Sep ? 1 : 0) +
      (level3 ? 1 : 0) +
      (level3Gray ? 1 : 0) +
      (level3Sep ? 1 : 0) > 1) {
    fprintf(stderr, "Error: use only one of the 'level' options.\n");
    exit(1);
  }
  if (doEPS && doForm) {
    fprintf(stderr, "Error: use only one of -eps and -form\n");
    exit(1);
  }
  if (level1) {
    level = psLevel1;
  } else if (level1Sep) {
    level = psLevel1Sep;
  } else if (level2Gray) {
    level = psLevel2Gray;
  } else if (level2Sep) {
    level = psLevel2Sep;
  } else if (level3) {
    level = psLevel3;
  } else if (level3Gray) {
    level = psLevel3Gray;
  } else if (level3Sep) {
    level = psLevel3Sep;
  } else {
    level = psLevel2;
  }
  if (doForm && level < psLevel2) {
    fprintf(stderr, "Error: forms are only available with Level 2 output.\n");
    exit(1);
  }
  mode = doEPS ? psModeEPS
               : doForm ? psModeForm
                        : psModePS;
  fileName = argv[1];

  // read config file
  if (cfgFileName[0] && !pathIsFile(cfgFileName)) {
    error(errConfig, -1, "Config file '{0:s}' doesn't exist or isn't a file",
	  cfgFileName);
  }
  globalParams = new GlobalParams(cfgFileName);
#ifdef HAVE_SPLASH
  globalParams->setupBaseFonts(NULL);
#endif
  if (paperSize[0]) {
    if (!globalParams->setPSPaperSize(paperSize)) {
      fprintf(stderr, "Invalid paper size\n");
      goto err0;
    }
  } else {
    if (paperWidth) {
      globalParams->setPSPaperWidth(paperWidth);
    }
    if (paperHeight) {
      globalParams->setPSPaperHeight(paperHeight);
    }
  }
  if (noCrop) {
    globalParams->setPSCrop(gFalse);
  }
  if (pageCrop) {
    globalParams->setPSUseCropBoxAsPage(gTrue);
  }
  if (expand) {
    globalParams->setPSExpandSmaller(gTrue);
  }
  if (noShrink) {
    globalParams->setPSShrinkLarger(gFalse);
  }
  if (noCenter) {
    globalParams->setPSCenter(gFalse);
  }
  if (duplex) {
    globalParams->setPSDuplex(duplex);
  }
  if (level1 || level1Sep ||
      level2 || level2Gray || level2Sep ||
      level3 || level3Gray || level3Sep) {
    globalParams->setPSLevel(level);
  }
  if (noEmbedT1Fonts) {
    globalParams->setPSEmbedType1(!noEmbedT1Fonts);
  }
  if (noEmbedTTFonts) {
    globalParams->setPSEmbedTrueType(!noEmbedTTFonts);
  }
  if (noEmbedCIDPSFonts) {
    globalParams->setPSEmbedCIDPostScript(!noEmbedCIDPSFonts);
  }
  if (noEmbedCIDTTFonts) {
    globalParams->setPSEmbedCIDTrueType(!noEmbedCIDTTFonts);
  }
  if (preload) {
    globalParams->setPSPreload(preload);
  }
#ifdef OPI_SUPPORT
  if (doOPI) {
    globalParams->setPSOPI(doOPI);
  }
#endif
  if (verbose) {
    globalParams->setPrintStatusInfo(verbose);
  }
  if (quiet) {
    globalParams->setErrQuiet(quiet);
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
    goto err1;
  }

  // check for print permission
  if (!doc->okToPrint()) {
    error(errNotAllowed, -1, "Printing this document is not allowed.");
    exitCode = 3;
    goto err1;
  }

  // construct PostScript file name
  if (argc == 3) {
    psFileName = new GString(argv[2]);
  } else {
    p = fileName + strlen(fileName) - 4;
    if (strlen(fileName) > 4 && (!strcmp(p, ".pdf") || !strcmp(p, ".PDF"))) {
      psFileName = new GString(fileName, (int)strlen(fileName) - 4);
    } else {
      psFileName = new GString(fileName);
    }
    psFileName->append(doEPS ? ".eps" : ".ps");
  }
  if (psFileName->cmp("-") == 0) {
    globalParams->setPrintStatusInfo(gFalse);
  }

  // get page range
  if (firstPage < 1) {
    firstPage = 1;
  }
  if (lastPage < 1 || lastPage > doc->getNumPages()) {
    lastPage = doc->getNumPages();
  }

  // check for multi-page EPS or form
  if ((doEPS || doForm) && firstPage != lastPage) {
    error(errCommandLine, -1, "EPS and form files can only contain one page.");
    goto err2;
  }

  // write PostScript file
  psOut = new PSOutputDev(psFileName->getCString(), doc,
			  firstPage, lastPage, mode,
			  0, 0, 0, 0, gFalse, NULL, NULL, userUnit, gTrue);
  if (!psOut->isOk()) {
    delete psOut;
    exitCode = 2;
    goto err2;
  }
  doc->displayPages(psOut, firstPage, lastPage, 72, 72,
		    0, !globalParams->getPSUseCropBoxAsPage(),
		    globalParams->getPSCrop(), gTrue);
  exitCode = 0;
  if (!psOut->checkIO()) {
    exitCode = 2;
  }
  delete psOut;

  // clean up
 err2:
  delete psFileName;
 err1:
  delete doc;
 err0:
  delete globalParams;

  // check for memory leaks
  Object::memCheck(stderr);
  gMemReport(stderr);

  return exitCode;
}
