/*
 * =====================================================================================
 *
 *       Filename:  atr.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/04/2016 08:56:18 PM
 *       Revision:  none:tanm	`
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "atr.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define atr_num_sectors(header) ( (header)->wPars*16/(header)->wSecSize)
#define atr_disk_size(header)   ( atr_num_sectors(header) * (header)->wSecSize )
#define atr_disk_spt(header)    ( atr_disk_size(header) / (header)->wSecSize )

int atr_new_from_file(struct atr *atr,char *atrfile)
{
	int f  = open(atrfile,O_RDONLY);
	if (f<0)
		return -1;
	int  n = read (f , &atr->header , sizeof(struct atr_header));
	if (n<0)
		return n;
	atr->buffer = malloc( atr_disk_size(&atr->header) );
	return 0;
}

void * atr_read_sector(struct atr *atr,int sector)
{
	if (sector<atr_num_sectors(&atr->header))
		return (atr->buffer + sector*atr->header.wSecSize);
	return 0;
}

void  atr_write_sector(struct atr *atr,int sector,char *buff)
{
	if (sector<atr_num_sectors(&atr->header))
		memcpy (atr->buffer + sector*atr->header.wSecSize,buff,atr->header.wSecSize);

}


