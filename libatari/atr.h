//
// File: atr.h
// Created by: Yair Gadelov <yair.gadelov@gmail.com>
// Created on: Fri Sep  1 22:18:48 2006
//

#ifndef _ATR_H_
#define _ATR_H_

#include "device.h"
/*
* http://www.atarimax.com/jindroush.atari.org/afmtatr.html
* @wMagic		sum of 'NICKATARI' = 0x0296
* @wPars		size of this disk image, in paragraphs (size/$10)
* @wSecSize		sector size. ($80 or $100) bytes/sector
* @dwCRC		32bit CRC of file (added by APE?)
* @dwUnused		unused
* @btFlags		bit 0 (ReadOnly) (added by APE?)
*
*/ 
#pragma pack(push,1)
struct atr_header  {
	short int	wMagic;	   	 	
	short int   	wPars 	;  	  	
	short int  	wSecSize ;	 	
	char 		btParsHigh;   	 	
	int 		dwCRC 	;		
	int 		dwUnused ; 		
	char  		btFlags; 	
};


/*
 * @device	abstarction class for devices
 * @header	atr_header struct
 * @type	type.  ED: Enhanced-Density, 128K/disk side , SD: Single-Density, 90K/disk side
 */
struct atr {
	struct device device;
	struct atr_header header;
	char *buffer;
};

#pragma pack(pop)
int atr_new_from_file(struct atr *atr,char *atrfile);
void * atr_read_sector(struct device *device,int sector);
void  atr_write_sector(struct device *device,int sector,char *buff);
int atr_sector_size(struct device *device);




#endif	//_ATR_H_
