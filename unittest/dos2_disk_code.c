/*
 * =====================================================================================
 *
 *       Filename:  atr.c
 *
 *    Description:  This SW takes dos2 disk and extract the 1-3 boot sectors and dos.sy and dup.sys
 *    		    and embbed the data in ascii form in c code in order to allow dos2 format command
 *
 *        Version:  1.0
 *        Created:  11/05/2016 10:50:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "../libatari/atr.h"
#include "../libatari/filesystem.h"
#include "../libatari/display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	struct atr atr;
	char dos[120*1024];
	char dup[120*1024];


	char *atrfile = "/home/yair/atari/atari_old/games/88_SEP.ATR";
	if (atr_new_from_file(&atr,atrfile)<0)
	{
		printf("error reading ATR file named %s\n",atrfile);
		return -1;
	}

	struct filesystem *fs = filesystem_init (&atr.device);
	filesystem_read_directory (fs);

	int dos_sys = filesystem_entity_filename(fs,"DOS","SYS");
	if (dos_sys<0)
		return -1;
	int dos_file_size = filesystem_read_file(fs,dos_sys,dos);


	int dup_sys = filesystem_entity_filename(fs,"DUP","SYS");
	if (dup_sys<0)
		return -1;
	int dup_file_size = filesystem_read_file(fs,dup_sys,dup);



//	int file_size = filesystem_read_file(fs,17,data);
//	printf("file size is %d\n",file_size);

//	filesystem_write_file(fs,8,data,file_size,"test1","bas");

	char boot_sector[3*128];
	memcpy (&boot_sector[0], device_read_sector(fs->device,0),128);
	memcpy (&boot_sector[128], device_read_sector(fs->device,1),128);
	memcpy (&boot_sector[256], device_read_sector(fs->device,2),128);

	int i,j;
	FILE *f = fopen ("dos2_format.h","w");
	fprintf(f,"#ifndef DOS2_FORMAT\n");
	fprintf(f,"#define DOS2_FORMAT\n");

	fprintf(f,"#define DOS_SYS_SIZE %d\n",dos_file_size);
	fprintf(f,"#define DUP_SYS_SIZE %d\n",dup_file_size);
	/* print boot sector */
	fprintf(f,"char boot_sector[384]={\n");
	for (i=0;i<24;i++) {
		for (j=0;j<15;j++) {
			fprintf(f,"0x%02x,",(unsigned char)boot_sector[i*16+j]);
		}
		fprintf(f,"0x%02x,\n",(unsigned char)boot_sector[i*16+15]);
	}
	fprintf(f,"}\n");
	/* printf dos.sys */
	fprintf(f,"char dos_sys[%d]={\n",dos_file_size);
	for (i=0;i<dos_file_size/16+1;i++) {
		for (j=0;j<15  && i*16+j<dos_file_size-1;j++) {
			fprintf(f,"0x%02x,",(unsigned char)dos[i*16+j]);
		}
		fprintf(f,"0x%02x,\n",(unsigned char)dos[i*16+j]);
	}
	fprintf(f,"}\n");
	/* print dup_sys */
	fprintf(f,"char dup_sys[%d]={\n",dup_file_size);
	for (i=0;i<dup_file_size/16+1;i++) {
		for (j=0;j<15 && i*16+j<dup_file_size-1;j++) {
			fprintf(f,"0x%02x,",(unsigned char)dup[i*16+j]);
		}

		fprintf(f,"0x%02x,\n",(unsigned char)dup[i*16+j]);
	}
	fprintf(f,"}\n");
	fprintf(f,"#endif\n");


	fclose(f);
	return 0;
}
