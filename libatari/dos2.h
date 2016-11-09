/*
 * =====================================================================================
 *
 *       Filename:  dos2.h
 *
 *    Description:  i
 *
 *        Version:  1.0
 *        Created:  11/06/2016 10:03:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef DOS2
#define DOS2
#include "device.h"
#include "filesystem.h"

/*
http://www.atarimax.com/jindroush.atari.org/afsdos2.html
http://www.emulatronia.com/doctec/consolas/atari8/atariref.txt


Boot Sector
Sectors from 1 to 3 contain boot record.


File sectors
First 125 bytes of sector contain data. Byte 126 contains file number in high six bits. Byte 127 and low two bits of byte 126 contain forward pointer. Byte 128 contains number of bytes in sector, highest bit flags end of file.
*/



#pragma pack (push,1)

/*
Sectors 361 thru 368 contain the disk directory. Each sector holds eight file names.
Offset 	Description
$00 	Flags
	bit 0 	Opened for output
	bit 5 	File locked
	bit 6 	File in use
	bit 7 	File deleted
$01-$02 	Number of sectors in file (little endian)
$03-$04 	Starting sector of file (little endian)
$05-$0C 	Filename (padded by spaces)
$0D-$0F 	Extension (padded by spaces)
*/
struct dos2_directory_sector {
	unsigned char 			flags;
#define DOS2_FLAGS_LOCK		(char)1<<5
#define DOS2_FLAGS_INUSE	(char)1<<6
#define DOS2_FLAGS_DELETE	(char)1<<7

	unsigned short 		num_of_sectors;
	unsigned short 		start_sector;
	char 			filename[8];
	char 			ext[3];
};

/*
 * @data 		the sector data
 * @file_namber		the index of the file in the table 0-63 (max 64 files)
 * @next_sector		pointer to next sector maximu 720. dos 2.0 has no access to sector 720
 * @num_of_data		the amout of data in the current sector
 * @eof			"1" is end of file
 */ 
struct dos2_file_sector {
	char 			data[125];
	unsigned int 		forward_hi:2;	
	unsigned int	 	file_number:6;
	unsigned int 		forward_low:8;	
	unsigned int 		num_of_data:7;
	unsigned int		eof:1;
};

/* 
 * Sector 360 is called the Volume Table of Contents or VTOC.
 * @avalibel_sectors 		Bytes $3 and $4 contain number of sectors available
 * @free_sectors		At byte 10 there's Volume Bitmap. Each byte tells 
 * 				status of 8 sectors. When bit is on, sector is available.
 *				Sector 720 cannot be accessed with DOS 2.0
 * 				
 */
struct dos2 {
	struct filesystem filesystem;
	struct dos2_directory_sector directory_sectors[64];
	unsigned short num_free_sectors;
	char free_sectors[118];

};

#pragma pack (pop)

void dos2_init ( struct dos2 *dos2,struct device *device );

/* virtual function */
void dos2_read_directory (struct filesystem *filesystem,struct device *device );
char dos2_is_free_sector(struct filesystem *filesystem,int sector);
char dos2_get_entity(struct filesystem *filesystem,struct entity *entity,int i);
int dos2_read_file(struct filesystem *filesystem,int i,char *data );
int dos2_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext );
int dos2_delete_file(struct filesystem *filesystem,int i );






#endif

