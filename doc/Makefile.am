man1dir = $(mandir)/man1
man5dir = $(mandir)/man5

dist_man1_MANS = \
	pdfdetach.1 \
	pdffonts.1 \
	pdfimages.1 \
	pdfinfo.1 \
	pdftops.1 \
	pdftotext.1 \
	$(empty)
if XPDF_HAVE_LIBPNG
if XPDF_HAVE_FREETYPE
dist_man1_MANS += pdftohtml.1 pdftopng.1
endif
endif
if XPDF_HAVE_FREETYPE
dist_man1_MANS += pdftoppm.1
endif
if HAVE_QT
if XPDF_HAVE_FREETYPE
if MULTITHREADED
if USE_EXCEPTIONS
dist_man1_MANS += xpdf.1
endif
endif
endif
endif

dist_man5_MANS = \
	xpdfrc.5 \
	$(empty)

EXTRA_DIST = sample-xpdfrc xpdf.1 pdftohtml.1 pdftopng.1 pdftoppm.1
