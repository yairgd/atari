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
#include "device.h"
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
	/* buffer for disk data */
	atr->buffer = malloc( atr_disk_size(&atr->header) );
	n = read (f , atr->buffer , atr_disk_size(&atr->header) - sizeof(struct atr_header));
	if (n<0)
		return n;
	/* set absraction */
	atr->device.read_sector = atr_read_sector;
	atr->device.write_sector = atr_write_sector;
	atr->device.sector_size = atr_sector_size;
	return 0;
}

void * atr_read_sector(struct device *device,int sector)
{
	/*
	Each sector on the disk is marked with a reference number from 1 to
     	720.  Unfortunately, the writers of DOS 2.0 didn't know this so they
     	wrote the DOS to use sectors numbered from 0 to 719.  As a result, DOS
    	 2.0 cannot access sector 720.  The designers of the disk drive were
     	the guilty party in this case.  It is normal to number from 0 in
     	computers.  With DOS 2.5, sectors 720 - 1,024 can be accessed
     	normally.
     	*/
//	if (sector<1 || sector>720)
//		return NULL;
	struct atr *atr = container_of (device,struct atr,device);
	if (sector<atr_num_sectors(&atr->header))
		return (atr->buffer + (sector-1)*atr->header.wSecSize);
	return 0;
}

void  atr_write_sector(struct device *device,int sector,char *buff)
{
	struct atr *atr= container_of (device,struct atr,device);
	if (sector<atr_num_sectors(&atr->header))
		memcpy (atr->buffer + sector*atr->header.wSecSize,buff,atr->header.wSecSize);

}

int atr_sector_size(struct device *device)
{
	struct atr *atr= container_of (device,struct atr,device);	
	return atr->header.wSecSize;
	
}

