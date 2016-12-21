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
#include "dos3.h"


static struct filesystem_desc filesystem_desc[]=  {
	{DOS25,"DOS25"},
	{DOS3,"DOS3"},
	{0,0}
};
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
#if 0
	struct dos2 *dos2  = (struct dos2 *) malloc (sizeof(struct dos2)); 
	dos2_init(dos2,device);
	return &dos2->filesystem;
#else 
	struct dos3 *dos3  = (struct dos3 *) malloc (sizeof(struct dos3)); 
	dos3_init(dos3,device);
	return &dos3->filesystem;
#endif
		
}


/**
 * Created  12/09/2016
 * @brief   		filesystem factory
 * @device 		device instane: device types: 1. 1050 device  2. atr file format
 * @type		filesyem type
 * @flag		private flag to factory function	
 * @return  
 */
struct filesystem *   filesystem_format (struct device *device,enum filesystem_type type,char flag)
{
	switch (type) {
		case DOS25:
			return dos2_format ( device, flag);
			break;
		case DOS3:
			break;
	}
	return 0;
}


