/*
 * =====================================================================================
 *
 *       Filename:  filesystem.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2016 10:01:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"
#include "dos2.h"

extern  char dos_sys [DOS_SYS_SIZE];
extern  char dup_sys [DUP_SYS_SIZE];


union fs {
	struct dos2 dos2;
};


extern char boot_sector[384];
int atari_strncmp(char *d1,char *d2,int n)
{
	int i;
	for (i=0;i<n ;i++) {
		if (d1[i]!=d2[i] && d1[i]!=0 && d1[i]!=32)
			return d1[i]>d2[i] ? -1 : 1;
	}
	return 0;

}
void filesystem_read_directory (struct filesystem *filesystem )
{
	filesystem->read_directory (filesystem,filesystem->device);
}


char filesystem_get_entity(struct filesystem *filesystem,struct entity *entity,int i)
{
	return filesystem->get_entity (filesystem,entity,i);
	
}
int filesystem_read_file(struct filesystem *filesystem,int i,char *data)
{
	return filesystem->read_file (filesystem,i,data);
	
}
int filesystem_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext)
{
	return filesystem->write_file (filesystem,i,data,file_len,filename,ext);
	
}

int filesystem_delate_file(struct filesystem *filesystem,int i)
{
	return filesystem->delete_file (filesystem,i);
}


int filesystem_entity_filename(struct filesystem *filesystem,char *filename,char *ext)
{
	struct entity entity;	
	int i=0;
	while (1) {
		if (filesystem_get_entity(filesystem,&entity,i))
			return -1;
		if (atari_strncmp (entity.filename,filename,8)==0 &&
				atari_strncmp (entity.ext,ext,3)==0)
			return i;
		i=i+1;

	}
	return -1;
}
/**
 * Created  11/07/2016
 * @brief   this function detect the type of the file system
 * 	    TODO: impmenet file system detection. 
 * @param   
 * @return  
 */
struct filesystem * filesystem_init (struct device *device )
{
	struct dos2 *dos2  = (struct dos2 *) malloc (sizeof(struct dos2)); 
	dos2_init(dos2,device);
	return &dos2->filesystem;
		
}


struct filesystem *   filesystem_format (struct device *device,enum filesystem_type type,char with_dos_copy)
{
	union fs *fs  ; 
	switch (type) {
		case DOS25:
			fs  = (union fs *) malloc (sizeof(struct dos2)); 
			dos2_init((struct dos2*)fs,device);
			dos2_init_fat( (struct dos2*)fs);
			device_write_sector(device,360-1,(char*)&fs->dos2.vtoc );
			device_write_sector (device,0,&boot_sector[0]);
			device_write_sector (device,1,&boot_sector[128]);
			device_write_sector (device,2,&boot_sector[256]);
			filesystem_write_file( (struct filesystem * )fs,0,dos_sys,DOS_SYS_SIZE,"DOS","SYS");
			if (with_dos_copy)
				filesystem_write_file( (struct filesystem * )fs,1,dup_sys,DUP_SYS_SIZE,"DUP","SYS");

			return &fs->dos2.filesystem;
			break;
		case DOS3:
			break;
	}
	return 0;
}


