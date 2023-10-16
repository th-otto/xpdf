/*
 *========================================================================
 *
 * config.h
 *
 * Copyright 1996-2022 Glyph & Cog, LLC
 *
 *========================================================================
 */

#ifndef XPDF_CONFIG_H
#define XPDF_CONFIG_H

/*
 *------------------------------------------------------------------------
 * version
 *------------------------------------------------------------------------
 */

/* xpdf version */
#define xpdfVersion          "4.04"
#define xpdfVersionNum       4.04
#define xpdfMajorVersion     4
#define xpdfMinorVersion     4
#define xpdfUpdateVersion    0
#define xpdfMajorVersionStr  "4"
#define xpdfMinorVersionStr  "4"
#define xpdfUpdateVersionStr "0"

/* supported PDF version */
#define supportedPDFVersionStr "2.0"
#define supportedPDFVersionNum 2.0

/* copyright notice */
#define xpdfCopyright "Copyright 1996-2022 Glyph & Cog, LLC"

/* Windows resource file stuff */
#define winxpdfVersion "WinXpdf 4.04"
#define xpdfCopyrightAmp "Copyright 1996-2022 Glyph && Cog, LLC"

/*
 *------------------------------------------------------------------------
 * paper size
 *------------------------------------------------------------------------
 */

/* default paper size (in points) for PostScript output */
#ifdef XPDF_A4_PAPER
#define defPaperWidth  595    /* ISO A4 (210x297 mm) */
#define defPaperHeight 842
#else
#define defPaperWidth  612    /* American letter (8.5x11") */
#define defPaperHeight 792
#endif

/*
 *------------------------------------------------------------------------
 * X-related constants
 *------------------------------------------------------------------------
 */

/* default maximum size of color cube to allocate */
#define defaultRGBCube 5

#endif
