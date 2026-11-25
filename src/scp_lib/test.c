/******************************************************************************
 *
 *    test.c
 *
 *    Copyright (C) 2004, 2005  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
 *
 *      http://www.gnu.org/copyleft/gpl.html
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

#include <stdio.h>
#include <stdlib.h>
#include "../misc_lib/misc_lib.h"
#include "scp_lib.h"

int main(void)
{
	char password[10] = "xxxxxxxx";

	echo_off();

	password[8] = 10;
	password[9] = 0;

	printf("\nCopying ssh_lib.022105.tar to olenska...\n\n");

	scp_file("/home/cnystrom/src/store/ssh_lib.022105.tar",
		 "olenska.newio.org:/export/home/cnystrom/", password);

	printf("\n");

	echo_on();

	exit(0);
}
