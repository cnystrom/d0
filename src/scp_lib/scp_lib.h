/******************************************************************************
 *
 *    scp_lib.h
 *
 *    Copyright (C) 2004, 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU Lessor General Public License as published
 *    by the Free Software Foundation; either version 2.1 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
 *
 *      http://www.gnu.org/copyleft/lessor.html
 *
 *    Contact Author at:
 *
 *    Chris Nystrom
 *
 *    E-Mail: cnystrom@gmail.com
 *    Blog:   http://conversazione.blogspot.com
 *    AIM:    nystromchris
 *
 *    Soli Deo Gloria
 *
 */

#ifndef _SCP_LIB_H
#define _SCP_LIB_H

#ifndef PRIVATE
#define PRIVATE static
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#define USE_READ  (0)
#define USE_UIO   (1)

#define LOGIN_BAD  (1)
#define NO_PROG    (2)

#ifndef BUFSIZ
#define BUFSIZ (8192)
#endif

PUBLIC int scp_file(const char *from_file, const char *to_file,
		    const char *passwd);

#endif				// _SCP_LIB_H
