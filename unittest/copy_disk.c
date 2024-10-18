/*
 * =====================================================================================
 *
 *       Filename:  copy_disk.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/18/2024 10:27:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */



#include <stdio.h>
#include "../libatari/atr.h"
#include "../libatari/drive1050.h"


extern  struct drive1050_state  drive1050_state;
//gcc -o a drive1050.c atr.c -O0 -g
int main()
{
	drive1050_open( "/dev/ttyUSB0");
	struct atr atr;
	unsigned char * b;
	
	uint32_t size = drive1050_size();
	printf ("start copy. disk size %d\n",size);



	atr_new_empty (&atr,size,"copy.atr");

	for (int i = 0; i < drive1050_num_of_sectors();i++) {
		b = drive1050_read_sector(i);
		if (drive1050_state.last_cmd_complete_status == 'C' &&  drive1050_state.last_cmd_ack=='A'
				&& drive1050_state.err == 0  ) {
			atr_write_sector(&atr,i,b);
			printf ("good %d\n",i);
		}
		else
		{printf("faile to read %d, err ID %d\n",i,drive1050_state.err );}
	}

	atr_flash(&atr);
}


