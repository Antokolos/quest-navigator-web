/* Copyright (C) 2005-2010 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "../../declarations.h"

#ifdef _EMSCRIPTEN

#include "../../text.h"
#include "../../ConvertUTF.h"

#ifdef _UNICODE
	#define QSP_WCSTOMBSLEN(a) (int)wcstombs(0, a, 0)
	#define QSP_WCSTOMBS wcstombs
#else
	#define QSP_WCSTOMBSLEN qspStrLen
	#define QSP_WCSTOMBS qspStrNCopy
#endif

char *qspToSysString(QSP_CHAR *s)
{
	int len = QSP_WCSTOMBSLEN(s) + 1;
	char *ret = (char *)malloc(len);
	QSP_WCSTOMBS(ret, s, len);
	return ret;
}

const UTF8 *QSPCharToUTF8(QSP_CHAR *source)
{
#ifdef _UNICODE
	char *errormessage;
	int len = qspStrLen(source) + 1;
	UTF16 *src = (UTF16*) source;
	UTF8 *tgt = malloc(len*sizeof(UTF8));
	UTF8 *result = tgt;
	ConversionResult cr = ConvertUTF16toUTF8(&src, &(src[len]), &tgt, &(tgt[len]), lenientConversion);
	switch (cr) {
		case conversionOK:
			result[len - 1] = 0;
			return result;
		case sourceExhausted:
			/* partial character in source, but hit end */
			errormessage = malloc(100 * sizeof(char));
			sprintf(errormessage, "QSPCharToUTF8 ERROR: sourceExhausted; len = %d", len);
			return errormessage;
		case targetExhausted:
			/* insuff. room in target for conversion */
			errormessage = malloc(100 * sizeof(char));
			sprintf(errormessage, "QSPCharToUTF8 ERROR: targetExhausted; len = %d", len);
			return errormessage;
		case sourceIllegal:
			return "QSPCharToUTF8 ERROR: sourceIllegal";
	}
	return "QSPCharToUTF8 ERROR: unknownError";
#else
	// TODO: examine non-unicode case
	return (UTF8*) source;
#endif
}

#endif
