lib_LIBRARIES = libxpdf.a
noinst_LIBRARIES =
if HAVE_QT
if MULTITHREADED
if USE_EXCEPTIONS
noinst_LIBRARIES += libxpdf_widget.a
endif
endif
endif

bin_PROGRAMS = pdftops pdftotext pdfinfo pdffonts pdfdetach pdfimages
if HAVE_LIBPNG
if HAVE_FREETYPE
bin_PROGRAMS += pdftohtml pdftopng
endif
endif
if HAVE_FREETYPE
bin_PROGRAMS += pdftoppm
endif

AM_CPPFLAGS = -I$(top_srcdir)/include
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
if HAVE_FREETYPE
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

if HAVE_FREETYPE
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
if HAVE_FREETYPE
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

pdftops_SOURCES = pdftops.cc \
	PreScanOutputDev.cc PreScanOutputDev.h \
	PSOutputDev.cc PSOutputDev.h \
	SplashOutputDev.cc \
	ShadingImage.cc ShadingImage.h \
	xpdftools.h \
	$(empty)
pdftops_LDADD = -L. -lxpdf
pdftops_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a
if HAVE_FREETYPE
pdftops_LDADD += -L../splash -lsplash
pdftops_DEPENDENCIES += ../splash/libsplash.a
endif
pdftops_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdffonts_SOURCES = pdffonts.cc xpdftools.h
pdffonts_LDADD = -L. -lxpdf
pdffonts_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a
pdffonts_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdfdetach_SOURCES = pdfdetach.cc xpdftools.h
pdfdetach_LDADD = -L. -lxpdf
pdfdetach_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a
pdfdetach_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftoppm_SOURCES = pdftoppm.cc \
	SplashOutputDev.cc \
	ShadingImage.cc ShadingImage.h \
	xpdftools.h
pdftoppm_LDADD = -L. -lxpdf
pdftoppm_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a ../splash/libsplash.a
pdftoppm_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftopng_SOURCES = pdftopng.cc \
	SplashOutputDev.cc \
	ShadingImage.cc ShadingImage.h \
	xpdftools.h
pdftopng_LDADD = -L. -lxpdf -L../goo -lgoo -L../fofi -lfofi -L../splash -lsplash
pdftopng_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a ../splash/libsplash.a
pdftopng_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdfinfo_SOURCES = pdfinfo.cc xpdftools.h
pdfinfo_LDADD = -L. -lxpdf -L../goo -lgoo -L../fofi -lfofi
pdfinfo_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a
pdfinfo_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftohtml_SOURCES = pdftohtml.cc HTMLGen.cc HTMLGen.h \
	SplashOutputDev.cc \
	ShadingImage.cc ShadingImage.h \
	WebFont.cc WebFont.h \
	xpdftools.h
pdftohtml_LDADD = -L. -lxpdf -L../goo -lgoo -L../fofi -lfofi -L../splash -lsplash
pdftohtml_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a ../splash/libsplash.a
pdftohtml_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdftotext_SOURCES = pdftotext.cc xpdftools.h
pdftotext_LDADD = -L. -lxpdf -L../goo -lgoo -L../fofi -lfofi -L../splash -lsplash
pdftotext_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a ../splash/libsplash.a
pdftotext_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdfimages_SOURCES = pdfimages.cc ImageOutputDev.cc ImageOutputDev.h xpdftools.h
pdfimages_LDADD = -L. -lxpdf -L../goo -lgoo -L../fofi -lfofi
pdfimages_DEPENDENCIES = libxpdf.a ../goo/libgoo.a ../fofi/libfofi.a
pdfimages_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)
