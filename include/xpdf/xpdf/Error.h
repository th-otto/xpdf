//========================================================================
//
// Error.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef XPDF_ERROR_H
#define XPDF_ERROR_H

#include <xpdf/aconf.h>

#include <stdio.h>
#include "xpdf/xpdf/config.h"
#include "xpdf/goo/gfile.h"

enum ErrorCategory {
  errSyntaxWarning,	// PDF syntax error which can be worked around;
			//   output will probably be correct
  errSyntaxError,	// PDF syntax error which cannot be worked around;
			//   output will probably be incorrect
  errConfig,		// error in Xpdf config info (xpdfrc file, etc.)
  errCommandLine,	// error in user-supplied parameters, action not
			//   allowed, etc. (only used by command-line tools)
  errIO,		// error in file I/O
  errNotAllowed,	// action not allowed by PDF permission bits
  errUnimplemented,	// unimplemented PDF feature - display will be
			//   incorrect
  errInternal		// internal error - malfunction within the Xpdf code
};

extern const char *const errorCategoryNames[];

extern void setErrorCallback(void (*cbk)(void *data, ErrorCategory category,
					 int pos, char *msg),
			     void *data);

extern void *getErrorCallbackData();

extern void error(ErrorCategory category, GFileOffset pos,
			const char *msg, ...);

#endif
