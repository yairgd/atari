/*
 * =====================================================================================
 *
 *       Filename:  dos2.c
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
#include "dos2.h"
#include "device.h"
#include <string.h>
#include <assert.h>

#define IS_BIT_ON(buf,size)\
	((buf[(size)/8]&(1<<((7-(size)%8))))!=0)
#define IS_BIT_OFF(buf,size)\
	((buf[(size)/8]&(1<<((7-(size)%8))))==0)

#define SET_BIT_ON(buf,size)\
	buf[(size)/8]|=(1<<((7-(size)%8)))

#define SET_BIT_OFF(buf,size)\
	buf[(size)/8]&=(~(1<<(7-((size)%8))))


static int get_free_sector(struct dos2 *dos2) 
{
	int i;
	for (i=0;i<8*118;i++)
	{
		if (IS_BIT_ON(dos2->vtoc.bit_map,i))
				return i;
	}
	return -1;
}
static int get_free_directory_entity(struct dos2 *dos2) 
{
	int i;
	for (i=0;i<64;i++)
	{
		if (dos2->directory_sectors[i].flags&DOS2_FLAGS_DELETE || dos2->directory_sectors[i].flags==0)
				return i;
	}
	return -1;
}

void dos2_init ( struct dos2 *dos2,struct device *device )
{
	dos2->filesystem.device = device;
	dos2->filesystem.type = DOS25;
	dos2->filesystem.read_directory = dos2_read_directory;
	dos2->filesystem.is_free_sector = dos2_is_free_sector;
	dos2->filesystem.get_entity = dos2_get_entity;
	dos2->filesystem.read_file = dos2_read_file;
	dos2->filesystem.delete_file = dos2_delete_file;
	dos2->filesystem.write_file = dos2_write_file;

}

void dos2_read_directory (struct filesystem *filesystem,struct device *device )
{
	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);
	int i;

	/* get VTOC */
	memcpy (&dos2->vtoc ,  device_read_sector (device,360-1) , device_sector_size(device) );
	
	/* read 8 sector of directory*/
	for (i=0;i<=7;i++)
		memcpy (&dos2->directory_sectors[i*8] ,  device_read_sector (device,i+361-1) , device_sector_size(device) );

}

/**
 * Created  11/07/2016
 * @brief   returns the value of the bit in sector 360 (VTOC) from byte 10 to byte 118 .
 * 	    '1': non-avalibale, '0' - avalibale
 * @param   
 * @return  
 */
char dos2_is_free_sector(struct filesystem *filesystem,int sector)
{
	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);
	return IS_BIT_ON(dos2->vtoc.bit_map,sector);
}

/**
 * Created  11/07/2016
 * @brief   returns entity information
 * @param   
 * @return  
 */
char dos2_get_entity(struct filesystem *filesystem,struct entity *entity,int i)
{
	if (i<0 || i>63)
		return -1;
	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);
	memcpy (&entity->filename,&dos2->directory_sectors[i].filename,8);
	memcpy (&entity->ext,&dos2->directory_sectors[i].ext,3);
	entity->size = dos2->directory_sectors[i].num_of_sectors;
	entity->filename[8]=0;
	entity->ext[3]=0;
	return 0;
}

int dos2_read_file(struct filesystem *filesystem,int i,char *data )
{
	int sector;
	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);

	assert (!(i<0 || i>63));
	/* find first sector and allocate data to the file*/
	sector = dos2->directory_sectors[i].start_sector;

	/* grub the file data into the prelocated buffer */
	char *p_data = data;
	char eof = 0;
	int tot=0;
//	int ii=0;
	while (!eof && sector!=0) {
		struct dos2_file_sector *file_sector = device_read_sector(filesystem->device,sector-1);
		sector = file_sector->forward_hi<<8 | file_sector->forward_low;
		eof = file_sector->eof;
		memcpy (p_data,file_sector->data,file_sector->num_of_data);
		p_data+=file_sector->num_of_data;
		tot+=file_sector->num_of_data;
	//	printf("%d\n",ii++);
	}
	return tot;
}


void dos2_init_fat(struct dos2 *dos2)
{
	int i;
	for (i=0;i<118;i++)
		dos2->vtoc.bit_map[i] = 0xff;
	for (i=360;i<=368;i++)
		SET_BIT_OFF(dos2->vtoc.bit_map,i);
	SET_BIT_OFF(dos2->vtoc.bit_map,0);
	SET_BIT_OFF(dos2->vtoc.bit_map,1);
	SET_BIT_OFF(dos2->vtoc.bit_map,2);
	dos2->vtoc.currently_unused_sectors = 707;

	
}



/**
 * Created  12/02/2016
 * @brief   		creates dos2 formratted disk 
 * @with_dos_copy	1: with dos copy, 0: without dos copy (dos command in basic mode switch to disk utils menu)
 * @return  
 */
void dos2_format (struct dos2 *dos2,int with_dos_copy)
{

}

int dos2_write_file(struct filesystem *filesystem,int i,char *data,int file_len,char *filename,char *ext )
{
	int idx=0;
	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);
	struct dos2_file_sector file_sector;	
	assert (!(i<0 || i>63));

	/* initilize the entity in the file directory table */
	idx = get_free_directory_entity(dos2);
	if (idx<0)
		return -1;	
	int sector = get_free_sector (dos2);
	if (sector<0)
		return -1; /* no free sectors */
	dos2->directory_sectors[idx].num_of_sectors = file_len/device_sector_size(filesystem->device)+ ( file_len%device_sector_size(filesystem->device)!=0) ;
	dos2->directory_sectors[idx].start_sector = sector+1;
	dos2->directory_sectors[idx].flags =  'B';// DOS2_FLAGS_INUSE;

	
	memcpy (&dos2->directory_sectors[idx].filename,filename,strlen(filename));
	for (i=strlen(filename);i<8;i++)
		dos2->directory_sectors[idx].filename [i]=32;
	memcpy (&dos2->directory_sectors[idx].ext,ext,3);

	/* write the sector directory with the new dile entity */
	device_write_sector (filesystem->device, idx/8 + 361-1 , (char*)&dos2->directory_sectors[idx/8]);

	/* set the number of the enity */
	file_sector.file_number = idx;
	
	/* write the file in the file sectors */
	idx =0;
	while (idx<file_len) {
		int c;
		if (file_len-idx>125) {
			c=125;
			file_sector.eof = 0;

		}
		else {
			c=file_len-idx;
			file_sector.eof = 1;
				
		}
		SET_BIT_OFF(dos2->vtoc.bit_map,sector);
		file_sector.num_of_data = c;
		int next_sector = get_free_sector (dos2);		
		if (next_sector<0)
			return -2;
		if (file_sector.eof)
			next_sector=-1;
		file_sector.forward_hi = (next_sector+1)>>8;
		file_sector.forward_low = (next_sector+1)&0xff;
		memcpy (&file_sector.data,&data[idx],c);
		/* the dup.sys MUST NOT have eof=1in the end of the file. 
		 * other wise the dos menu util is not loaded after writing DOS
		 * command in the basic prompt
		 */
		if (file_sector.file_number ==1 && file_sector.eof && strcmp (filename,"DUP")==0 && strcmp (ext,"SYS")==0 )
		{
			file_sector.eof=0;
			
		}

		device_write_sector (filesystem->device, sector,(char*)&file_sector);
		sector = next_sector;
		idx+=c;
		
	}

	/* change free sctors */
	dos2->vtoc.currently_unused_sectors-=file_len/device_sector_size(filesystem->device)+ ( file_len%device_sector_size(filesystem->device )!=0); 

	/* update vtoc */
	device_write_sector(filesystem->device,360-1,(char*)&dos2->vtoc);

	return idx;

}

int dos2_delete_file(struct filesystem *filesystem,int i )
{

//	struct dos2 *dos2= container_of (filesystem,struct dos2,filesystem);
	return 0;
}
