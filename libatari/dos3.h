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

#ifndef DOS3
#define DOS3
#include "device.h"
#include "filesystem.h"

//#define DOS_SYS_SIZE 4625
//#define DUP_SYS_SIZE 5126



#pragma pack (push,1)

struct dos3 {
	struct filesystem filesystem;

};



#pragma pack (pop)

/* virtual function */
void dos3_read_directory (struct filesystem *filesystem,struct device *device );
char dos3_is_free_sector(struct filesystem *filesystem,int sector);
char dos3_get_entity(struct filesystem *filesystem,struct entity *entity,int i);
int dos3_read_file(struct filesystem *filesystem,int i,char *data );
int dos3_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext );
int dos3_delete_file(struct filesystem *filesystem,int i );




#endif

