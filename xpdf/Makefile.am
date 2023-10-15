lib_LIBRARIES = libxpdf.a
noinst_LIBRARIES =
if HAVE_QT
if MULTITHREADED
if USE_EXCEPTIONS
noinst_LIBRARIES += libxpdf_widget.a
endif
endif
endif

pkgconfig_DATA = xpdf.pc

AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include
AM_CPPFLAGS += $(PNG_CFLAGS)
AM_CPPFLAGS += $(LCMS_CFLAGS)
AM_CPPFLAGS += $(FREETYPE2_CFLAGS)

libxpdf_a_SOURCES = \
	AcroForm.cc \
	Annot.cc \
	Array.cc \
	BuiltinFont.cc BuiltinFont.h \
	BuiltinFontTables.cc BuiltinFontTables.h \
	CMap.cc CMap.h \
	Catalog.cc \
	CharCodeToUnicode.cc CharCodeToUnicode.h \
	CompactFontTables.h \
	Decrypt.cc Decrypt.h \
	Dict.cc \
	Error.cc ErrorCodes.h \
	FontEncodingTables.cc FontEncodingTables.h \
	Function.cc \
	Gfx.cc \
	GfxFont.cc \
	GfxState.cc \
	GlobalParams.cc \
	JArithmeticDecoder.cc JArithmeticDecoder.h \
	JBIG2Stream.cc JBIG2Stream.h \
	JPXStream.cc JPXStream.h \
	Lexer.cc Lexer.h \
	Link.cc \
	NameToCharCode.cc NameToCharCode.h NameToUnicodeTable.h \
	Object.cc \
	OptionalContent.cc OptionalContent.h \
	Outline.cc Outline.h \
	OutputDev.cc \
	PDF417Barcode.cc PDF417Barcode.h \
	PDFDoc.cc \
	PDFDocEncoding.cc PDFDocEncoding.h \
	PSTokenizer.cc PSTokenizer.h \
	Page.cc \
	Parser.cc Parser.h \
	SecurityHandler.cc SecurityHandler.h \
	Stream.cc Stream-CCITT.h \
	TextOutputDev.cc \
	TextString.cc \
	UTF8.cc UTF8.h \
	UnicodeMap.cc UnicodeMapTables.h \
	UnicodeRemapping.cc UnicodeRemapping.h \
	UnicodeTypeTable.cc UnicodeTypeTable.h \
	XFAScanner.cc XFAScanner.h \
	XRef.cc \
	Zoox.cc Zoox.h \
	$(empty)
if XPDF_HAVE_FREETYPE
libxpdf_a_SOURCES += \
	ShadingImage.cc ShadingImage.h \
	SplashOutputDev.cc
endif

if HAVE_LCMS
#libxpdf_a_SOURCES += ColorManager.cc ColorManager.h
endif

libxpdf_a_LIBADD = \
	../goo/FixedPoint.o \
	../goo/GHash.o \
	../goo/GList.o \
	../goo/GString.o \
	../goo/gfile.o\
	../goo/gmem.o \
	../goo/gmempp.o \
	../goo/Trace.o \
	../goo/parseargs.o \
	\
	../fofi/FoFiBase.o \
	../fofi/FoFiEncodings.o \
	../fofi/FoFiIdentifier.o \
	../fofi/FoFiTrueType.o \
	../fofi/FoFiType1.o \
	../fofi/FoFiType1C.o \
	$(empty)
libxpdf_a_DEPENDENCIES = ../fofi/libfofi.a ../goo/libgoo.a

if XPDF_HAVE_FREETYPE
libxpdf_a_LIBADD += \
	../splash/Splash.o \
	../splash/SplashBitmap.o \
	../splash/SplashClip.o \
	../splash/SplashFTFont.o \
	../splash/SplashFTFontEngine.o \
	../splash/SplashFTFontFile.o \
	../splash/SplashFont.o \
	../splash/SplashFontEngine.o \
	../splash/SplashFontFile.o \
	../splash/SplashFontFileID.o \
	../splash/SplashPath.o \
	../splash/SplashPattern.o \
	../splash/SplashScreen.o \
	../splash/SplashState.o \
	../splash/SplashXPath.o \
	../splash/SplashXPathScanner.o \
	$(empty)
libxpdf_a_DEPENDENCIES += ../splash/libsplash.a
endif

libxpdf_widget_a_SOURCES =
if HAVE_QT
if MULTITHREADED
if USE_EXCEPTIONS
libxpdf_widget_a_SOURCES += \
	DisplayState.cc DisplayState.h \
	PDFCore.cc PDFCore.h \
	PreScanOutputDev.cc PreScanOutputDev.h \
	TileCache.cc TileCache.h \
	TileCompositor.cc TileCompositor.h \
	TileMap.cc TileMap.h \
	$(empty)
if XPDF_HAVE_FREETYPE
libxpdf_widget_a_SOURCES += PSOutputDev.cc PSOutputDev.h
endif
if HIGHLIGHTED_REGIONS
#libxpdf_widget_a_SOURCES += HighlightFile.cc HighlightFile.h
endif
if XPDFWIDGET_PRINTING
if OS_WIN32
#libxpdf_widget_a_SOURCES += GDIOutputDev.cc GDIOutputDev.h
#libxpdf_widget_a_SOURCES += WinPDFPrinter.cc WinPDFPrinter.h
endif
endif
endif
endif
endif
