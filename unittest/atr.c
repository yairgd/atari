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

//	filesystem_write_file(fs,8,data,file_size,"test1","bas");


	return 0;
}
