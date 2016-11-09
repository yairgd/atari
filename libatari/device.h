//
// File: atr.h
// Created by: Yair Gadelov <yair.gadelov@gmail.com>
// Created on: Sun Nov  6 22:18:48 2016
//

#ifndef _DEVICE_H_
#define _DEVICE_H_


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({            \
 const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
 (type *)( (char *)__mptr - offsetof(type,member) );})

/*
* http://www.atarimax.com/jindroush.atari.org/afmtatr.html
* @read_sector		sum of 'NICKATARI' = 0x0296
* @write_sector		size of this disk image, in paragraphs (size/$10)
* @wSecSize		sector size. ($80 or $100) bytes/sector
* @dwCRC		32bit CRC of file (added by APE?)
* @dwUnused		unused
* @btFlags		bit 0 (ReadOnly) (added by APE?)
*
*/ 
#pragma pack(push,1)
struct device  {
	void * (*read_sector) (struct device *device,int sector);
	void (*write_sector)(struct device *device,int sector,char *buff);
	int (*sector_size)(struct device *device);

};
#pragma pack(pop)
void *device_read_sector(struct device *device,int sector);
void device_write_sector(struct device *device,int sector,char *buff);
int device_sector_size(struct device *device);





#endif	//_ATR_H_
