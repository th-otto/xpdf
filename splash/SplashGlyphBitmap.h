//========================================================================
//
// SplashGlyphBitmap.h
//
// Copyright 2003-2013 Glyph & Cog, LLC
//
//========================================================================

#ifndef SPLASHGLYPHBITMAP_H
#define SPLASHGLYPHBITMAP_H

#include <xpdf/aconf.h>

#include "xpdf/goo/gtypes.h"

//------------------------------------------------------------------------
// SplashGlyphBitmap
//------------------------------------------------------------------------

struct SplashGlyphBitmap {
  int x, y, w, h;		// offset and size of glyph
  GBool aa;			// anti-aliased: true means 8-bit alpha
				//   bitmap; false means 1-bit
  Guchar *data;			// bitmap data
  GBool freeData;		// true if data memory should be freed
};

#endif
