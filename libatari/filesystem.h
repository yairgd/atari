/*
 * =====================================================================================
 *
 *       Filename:  filesystem.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2016 10:00:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include"device.h"

struct entity {
	char flag;
	char filename[9];
	char ext[4];
	unsigned short size;
};

enum filesystem_type {
	DOS25,
	DOS3
};
struct filesystem {
	struct device *device;
	enum filesystem_type type;

	void (*read_directory) 		(struct filesystem *filesystem, struct device *device );
	char (*is_free_sector) 		(struct filesystem *filesystem,int sector);
	int (*read_file) 		(struct filesystem *filesystem,int i,char *data);
	char (*get_entity) 		(struct filesystem *filesystem,struct entity *entity,int i);
	int (*write_file) 		(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext);
	int (*delete_file) 		(struct filesystem *filesystem,int i);
};

void filesystem_read_directory (struct filesystem *filesystem );
char filesystem_get_entity(struct filesystem *filesystem,struct entity *entity,int i);
int filesystem_entity_filename(struct filesystem *filesystem,char *filename,char *ext);
int filesystem_read_file(struct filesystem *filesystem,int i,char *data);
int filesystem_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext);
int filesystem_delate_file(struct filesystem *filesystem,int i);


struct filesystem * filesystem_init (struct device *device );
struct filesystem *   filesystem_format (struct device *device,enum filesystem_type type,char with_dos_copy);




#endif
