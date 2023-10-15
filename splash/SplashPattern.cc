//========================================================================
//
// SplashPattern.cc
//
// Copyright 2003-2013 Glyph & Cog, LLC
//
//========================================================================

#include "xpdf/xpdfbuild.h"

#include "../goo/gmempp.h"
#include "xpdf/splash/SplashMath.h"
#include "SplashScreen.h"
#include "SplashPattern.h"

//------------------------------------------------------------------------
// SplashPattern
//------------------------------------------------------------------------

SplashPattern::SplashPattern() {
}

SplashPattern::~SplashPattern() {
}

//------------------------------------------------------------------------
// SplashSolidColor
//------------------------------------------------------------------------

SplashSolidColor::SplashSolidColor(SplashColorPtr colorA) {
  splashColorCopy(color, colorA);
}

SplashSolidColor::~SplashSolidColor() {
}

void SplashSolidColor::getColor(int x, int y, SplashColorPtr c) {
  (void)x;
  (void)y;
  splashColorCopy(c, color);
}

