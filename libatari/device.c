/*
 * =====================================================================================
 *
 *       Filename:  device.c
 *
 *    Description:  abstaction class fot ATR and 1050 devices
 *
 *        Version:  1.0
 *        Created:  11/06/2016 04:10:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "device.h"
#include <string.h>

void device_write_sector(struct device *device,int sector,char *buff)
{
	device->write_sector(device,sector,buff);
}

void *device_read_sector(struct device *device,int sector)
{
	return device->read_sector(device,sector);
	
}

int device_sector_size(struct device *device)
{
	return  device->sector_size(device);

}

int device_flash(struct device *device)
{
	return  device->flash(device);

}
/**
 * Created  11/12/2016
 * @brief   this function should get empty device (atr of 1050 disk) and craete 
 * 	    boot sector with the givven file (usually exe game)
 * 	    see http://atari.kensclassics.org/dos.htm for mare details
 * @param   
 * @return  
 */
int device_create_boot_sector (struct device *device,char *buff,int len)
{

	char sector[128];
	int c,i,tot=0;
	/* build first sector */
	memset (sector,0,128);
	sector[0] = 0;
	sector[1] = (len/128) + ( (len%128)>0);
	sector[2] = 1;
	sector[3] = 0x07;
	sector[4] = 0x11;
	c = (len>128-16) ? 128-16 : len;
	memcpy (&sector[16],&buff[tot], c);
	len-=c;
	tot+=c;
	device_write_sector(device, 0 ,sector);

	/* write other sectors */
	i=1;
	while (len>0) {
		c = (len>128) ? 128 : len;
		memcpy (sector,&buff[tot], c);
		tot+=c;
		len-=c;
		device_write_sector(device, i++ ,sector);	
	};
	return 0;
}
