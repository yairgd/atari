//
// File: atr.h
// Created by: Yair Gadelov <yair.gadelov@gmail.com>
// Created on: Fri Sep  1 22:18:48 2006
//

#ifndef _ATR_H_
#define _ATR_H_
#include <stdint.h>

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
	uint16_t	wMagic;	   	 	
	uint16_t   	wPars 	;  	  	
	uint16_t  	wSecSize ;	 	
	uint8_t 	btParsHigh;   	 	
	uint32_t 	dwCRC 	;		
	uint32_t	dwUnused ; 		
	uint8_t 	btFlags; 	
};


/*
 * @device	abstarction class for devices
 * @header	atr_header struct
 * @type	type.  ED: Enhanced-Density, 128K/disk side , SD: Single-Density, 90K/disk side
 */
struct atr {
	struct atr_header header;
	char *buffer;
	int fd;
};

#pragma pack(pop)
int atr_new_from_file(struct atr *atr,char *atrfile);
int atr_new_empty (struct atr *atr,uint32_t diskSize,char *atrfile);

void * atr_read_sector(struct atr *atr,int sector);
void  atr_write_sector(struct atr *atr,int sector,unsigned char *buff);
int atr_sector_size(struct atr *atr);
int atr_flash (struct atr *atr);




#endif	//_ATR_H_
