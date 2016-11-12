/*
 * =====================================================================================
 *
 *       Filename:  atr.c
 *
 *    Description:  
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

#define DOS_SYS_SIZE 4625
#define DUP_SYS_SIZE 5126

extern  char boot_sector [384];
extern  char dos_sys [DOS_SYS_SIZE];
extern  char dup_sys [DUP_SYS_SIZE];


int main()
{
	struct atr atr;
	char data[120*1024];
	char *atrfile = "/home/yair/atari/atari_old/games/88_SEP.ATR";
	if (atr_new_from_file(&atr,atrfile)<0)
	{
		printf("error reading ATR file named %s\n",atrfile);
		return -1;
	}

	struct filesystem *fs = filesystem_init (&atr.device);
	filesystem_read_directory (fs);
	display_show_dir(fs);

	int entity = filesystem_entity_filename(fs,"KASONTWR","BAS");
	if (entity<0)
		return -1;

	int file_size = filesystem_read_file(fs,17,data);
	printf("file size is %d\n",file_size);

	struct atr new_atr;
	atr_new_empty (&new_atr,"atr5.atr");
	struct filesystem *new_fs = filesystem_format (&new_atr.device,DOS25,1);
	device_flash  (&new_atr.device);
	(void)new_fs;
	return 0;
}
