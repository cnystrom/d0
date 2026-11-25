/******************************************************************************
 *
 *    uio.h
 *
 *    Copyright (C) 2002, 2005  Chris Nystrom
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

#ifndef _UIO_H
#define _UIO_H

#ifndef _TYPES_H
#include "types.h"
#endif

#ifndef PRIVATE
#define PRIVATE static
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

PUBLIC int uio_open_file_rd(const char *pathname);
PUBLIC int uio_open_file_wr(const char *pathname);
PUBLIC int uio_open_file_rw(const char *pathname);

PUBLIC int uio_write_byte(int fd, byte b);
PUBLIC byte uio_read_byte(int fd);

PUBLIC int uio_get_line(int fd, byte * line, int lim);
PUBLIC int uio_put_line(int fd, const byte * line);
PUBLIC int uio_put_linef(int fd, const char *format, ...);

#endif				// _UIO_H
