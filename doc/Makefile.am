man1dir = $(mandir)/man1
man5dir = $(mandir)/man5

man1_FILES = \
	pdfdetach.1 \
	pdffonts.1 \
	pdfimages.1 \
	pdfinfo.1 \
	pdftops.1 \
	pdftotext.1 \
	$(empty)
if XPDF_HAVE_LIBPNG
if XPDF_HAVE_FREETYPE
man1_FILES += pdftohtml.1 pdftopng.1
endif
endif
if XPDF_HAVE_FREETYPE
man1_FILES += pdftoppm.1
endif
if HAVE_QT
if XPDF_HAVE_FREETYPE
if MULTITHREADED
if USE_EXCEPTIONS
man1_FILES += xpdf.1
endif
endif
endif
endif

man5_FILES = \
	xpdfrc.5 \
	$(empty)

EXTRA_DIST = sample-xpdfrc
