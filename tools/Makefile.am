bin_PROGRAMS = pdftops pdftotext pdfinfo pdffonts pdfdetach pdfimages
if XPDF_HAVE_LIBPNG
if XPDF_HAVE_FREETYPE
bin_PROGRAMS += pdftohtml pdftopng
endif
endif
if XPDF_HAVE_FREETYPE
bin_PROGRAMS += pdftoppm
endif

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS += $(PNG_CFLAGS)
AM_CPPFLAGS += $(LCMS_CFLAGS)
AM_CPPFLAGS += $(FREETYPE2_CFLAGS)

pdftops_SOURCES = pdftops.cc \
	PreScanOutputDev.cc PreScanOutputDev.h \
	PSOutputDev.cc PSOutputDev.h \
	xpdftools.h \
	$(empty)
pdftops_LDADD = -L../xpdf -lxpdf
pdftops_DEPENDENCIES = ../xpdf/libxpdf.a
pdftops_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdffonts_SOURCES = pdffonts.cc xpdftools.h
pdffonts_LDADD = -L../xpdf -lxpdf
pdffonts_DEPENDENCIES = ../xpdf/libxpdf.a
pdffonts_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdfdetach_SOURCES = pdfdetach.cc xpdftools.h
pdfdetach_LDADD = -L../xpdf -lxpdf
pdfdetach_DEPENDENCIES = ../xpdf/libxpdf.a
pdfdetach_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftoppm_SOURCES = pdftoppm.cc xpdftools.h
pdftoppm_LDADD = -L../xpdf -lxpdf
pdftoppm_DEPENDENCIES = ../xpdf/libxpdf.a
pdftoppm_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftopng_SOURCES = pdftopng.cc xpdftools.h
pdftopng_LDADD = -L../xpdf -lxpdf
pdftopng_DEPENDENCIES = ../xpdf/libxpdf.a
pdftopng_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdfinfo_SOURCES = pdfinfo.cc xpdftools.h
pdfinfo_LDADD = -L../xpdf -lxpdf
pdfinfo_DEPENDENCIES = ../xpdf/libxpdf.a
pdfinfo_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdftohtml_SOURCES = pdftohtml.cc HTMLGen.cc HTMLGen.h \
	WebFont.cc WebFont.h \
	xpdftools.h
pdftohtml_LDADD = -L../xpdf -lxpdf
pdftohtml_DEPENDENCIES = ../xpdf/libxpdf.a
pdftohtml_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS) $(PNG_LIBS)

pdftotext_SOURCES = pdftotext.cc xpdftools.h
pdftotext_LDADD = -L../xpdf -lxpdf
pdftotext_DEPENDENCIES = ../xpdf/libxpdf.a
pdftotext_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

pdfimages_SOURCES = pdfimages.cc ImageOutputDev.cc ImageOutputDev.h xpdftools.h
pdfimages_LDADD = -L../xpdf -lxpdf
pdfimages_DEPENDENCIES = ../xpdf/libxpdf.a
pdfimages_LDADD += $(LCMS_LIBS) $(FREETYPE2_LIBS)

