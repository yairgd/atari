/*
 * =====================================================================================
 *
 *       Filename:  display.c
 *
 *    Description:  simple text display class
 *
 *        Version:  1.0
 *        Created:  11/07/2016 10:25:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "display.h"
#include <stdlib.h>
#include <stdio.h>

void display_show_dir(struct filesystem *filesystem)
{
	struct entity entity;
	int i=0;
	while (1) {
		if (filesystem->get_entity(filesystem,&entity,i))
			return;
		printf("%d) %s.%s	%d\n",i,entity.filename,entity.ext,entity.size);
		i=i+1;
	}
	
}
