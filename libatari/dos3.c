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
	return 0;
}


int dos3_read_file(struct filesystem *filesystem,int i,char *data )
{
	return 0; /* file length*/
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
