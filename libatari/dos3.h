/*
 * =====================================================================================
 *
 *       Filename:  dos3.h
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

#ifndef _DOS3_H
#define _DOS3_H
#include "device.h"
#include "filesystem.h"

/*
information took from: http://www.atarimax.com/jindroush.atari.org/afsdos3.html

Sectors $01-$09 is boot sector.
$0A-$0F are empty.
Directory
$10-$17 are directory. I don't know what's first entry, containing $0E empty bytes and $7F $A5 at the end (for MD) and $57 $A5 for SD.

Offset 	Description
FAT
Sector $18 is FAT. Maximum number of blocks that could be allocated is $7F.

    * $FF - can't be allocated
    * $FE - free
    * $FD - eof
    * other - link to other block 

This format is just guessed.
*/

#pragma pack (push,1)
/*
@flags			$00 	Flags // file status - 80 file eased , c0- record fulled , c2 protected record
				bit 0 	Unknown
				bit 1 	Unknown
				bit 2 	Unknown
				bit 3 	Unknown
				bit 4 	Unknown
				bit 5 	Unknown
				bit 6 	Unknown
				bit 7 	Unknown
@filenam		$01-$08 	Filename (padded by spaces)
@ext			$09-$0B 	Extension (padded by spaces)
@number_of_blocks	$0C 		Number of blocks (8 sectors)
@start_balock		$0D 		Starting block
 @length			$0E-$0F 	Length of file in bytes
*/
struct dos3_directory_sector {
	char flags;
	char filename[8];
	char ext[3];
	char number_of_clock;
	char start_block;
	short length;
	
};


struct dos3 {
	struct filesystem filesystem;
	struct dos3_directory_sector directory_sectors[64];
	char fat[128];
	char free_sectors[128];

};



#pragma pack (pop)

/* virtual function */
void dos3_read_directory (struct filesystem *filesystem,struct device *device );
char dos3_is_free_sector(struct filesystem *filesystem,int sector);
char dos3_get_entity(struct filesystem *filesystem,struct entity *entity,int i);
int dos3_read_file(struct filesystem *filesystem,int i,char *data );
int dos3_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext );
int dos3_delete_file(struct filesystem *filesystem,int i );

void dos3_init ( struct dos3 *dos3,struct device *device );



#endif

