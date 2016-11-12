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

