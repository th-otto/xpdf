//========================================================================
//
// SplashFTFont.h
//
// Copyright 2003-2013 Glyph & Cog, LLC
//
//========================================================================

#ifndef XPDF_SPLASHFTFONT_H
#define XPDF_SPLASHFTFONT_H

#include <xpdf/aconf.h>

#ifdef XPDF_HAVE_FREETYPE

#include <ft2build.h>
#include <freetype/freetype.h>
#include "xpdf/splash/SplashFont.h"

class SplashFTFontFile;

//------------------------------------------------------------------------
// SplashFTFont
//------------------------------------------------------------------------

class SplashFTFont: public SplashFont {
public:

  SplashFTFont(SplashFTFontFile *fontFileA, SplashCoord *matA,
	       SplashCoord *textMatA);

  virtual ~SplashFTFont();

  // Munge xFrac and yFrac before calling SplashFont::getGlyph.
  virtual GBool getGlyph(int c, int xFrac, int yFrac,
			 SplashGlyphBitmap *bitmap);

  // Rasterize a glyph.  The <xFrac> and <yFrac> values are the same
  // as described for getGlyph.
  virtual GBool makeGlyph(int c, int xFrac, int yFrac,
			  SplashGlyphBitmap *bitmap);

  // Return the path for a glyph.
  virtual SplashPath *getGlyphPath(int c);

private:

  FT_Size sizeObj;
  FT_Matrix matrix;
  FT_Matrix textMatrix;
  SplashCoord textScale;
};

#endif /* XPDF_HAVE_FREETYPE */

#endif
