/*
 * =====================================================================================
 *
 *       Filename:  dos3.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2016 10:03:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "dos3.h"
#include "device.h"
#include <string.h>
#include <assert.h>


void dos3_init ( struct dos3 *dos3,struct device *device )
{
	dos3->filesystem.device = device;
	dos3->filesystem.type = DOS25;
	dos3->filesystem.read_directory = dos3_read_directory;
	dos3->filesystem.is_free_sector = dos3_is_free_sector;
	dos3->filesystem.get_entity = dos3_get_entity;
	dos3->filesystem.read_file = dos3_read_file;
	dos3->filesystem.delete_file = dos3_delete_file;
	dos3->filesystem.write_file = dos3_write_file;

}

void dos3_read_directory (struct filesystem *filesystem,struct device *device )
{

	struct dos3 *dos3 = container_of (filesystem,struct dos3,filesystem);
	int i;

	/* read 8 sector of directory*/
	for (i=0;i<=7;i++)
		memcpy (&dos3->directory_sectors[i*8] ,  device_read_sector (device,i+0x10-1) , device_sector_size(device) );

	/* read FAT */
	memcpy (&dos3->fat ,  device_read_sector (device,i+0x18) , device_sector_size(device) );

}

/**
 * Created  11/07/2016
 * @brief   returns the value of the bit in sector 360 (VTOC) from byte 10 to byte 118 .
 * 	    '1': non-avalibale, '0' - avalibale
 * @param   
 * @return  
 */
char dos3_is_free_sector(struct filesystem *filesystem,int sector)
{
	int k,j,i;
	struct dos3 *dos3 = container_of (filesystem,struct dos3,filesystem);	
	/* mark free sectors */
	j=0;
	for (i=0;i<128;i++)
		if (dos3->fat[i]>=0xfe) 
		{
			for (k=25+i*8;k<25+(i+1)*8;k++,j++)
				return sector==k;
		}
	return 0;
	
}

/**
 * Created  11/07/2016
 * @brief   returns entity information
 * @param   
 * @return  
 */
char dos3_get_entity(struct filesystem *filesystem,struct entity *entity,int i)
{
	if (i<0 || i>63)
		return -1;
	struct dos3 *dos3= container_of (filesystem,struct dos3,filesystem);
	memcpy (&entity->filename,&dos3->directory_sectors[i].filename,8);
	memcpy (&entity->ext,&dos3->directory_sectors[i].ext,3);
	entity->size = dos3->directory_sectors[i].length;
	entity->filename[8]=0;
	entity->ext[3]=0;
	return 0;

}


int dos3_read_file(struct filesystem *filesystem,int i,char *data )
{
#if 0
	int file_handle,nbyte,nbyte_to_write,l;
	short int *psect;
	char *buf;
	nbyte=0;
	// create new file on linux hard disk
	file_handle = creat (filename,0xffff);
	if (file_handle==-1)
		return -1;
	
	// locate file entity in case that filename is given instead.
	if (entity==-1)
	{
		entity=GetEntityIdx(entity_filename);
		if (entity==-1) return -1; // file not found
	}
	ReadDir(*drv);
	SetEntity(entity);
	psect=GetSectorList();
	l=GetFileLength();
		
	// read file from the entity
	while (1)
	{
		if (*psect==-1) break;
	    buf=drv->ReadSector(*psect);
		if (GetFileLength()<nbyte)
			nbyte_to_write=nbyte-GetFileLength();
		else
			nbyte_to_write=128;
		nbyte+=128;
		write (file_handle,drv->buf,nbyte_to_write);
		*psect++;
	}
 	close (file_handle);
	return 1;
#endif
	return 0;
}




int dos3_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext )
{
	return 0;

}

int dos3_delete_file(struct filesystem *filesystem,int i )
{

	//	struct dos3 *dos3= container_of (filesystem,struct dos3,filesystem);
	return 0;
}
