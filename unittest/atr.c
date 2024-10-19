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

//#define DOS_SYS_SIZE 4625
//#define DUP_SYS_SIZE 5126

int read_file(char *filename,char **buffer_out,int *length)
{
	char * buffer = 0;
	FILE * f = fopen (filename, "rb");

	if (f)
	{
		fseek (f, 0, SEEK_END);
		*length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = malloc (*length);
		if (buffer)
		{
			fread (buffer, 1, *length, f);
		}
		fclose (f);
		*buffer_out = buffer;
		return 0;
	}
	return -1;
}


int main1()
{
	struct atr atr;
	char data[120*1024];
	//	char *atrfile = "/home/yair/atari/atari_old/games/88_SEP.ATR";

	char *atrfile = "/home/yair/atari/atari_old/mydisks/disk3_2016.atr";
	if (atr_new_from_file(&atr,atrfile)<0)
	{
		printf("error reading ATR file named %s\n",atrfile);
		return -1;
	}

	struct filesystem *fs = filesystem_init (&atr.device);
	filesystem_read_directory (fs);
	display_show_dir(fs);
	//return 0;

	int entity = filesystem_entity_filename(fs,"KASONTWR","BAS");
	if (entity<0)
		return -1;

	int file_size = filesystem_read_file(fs,17,data);
	printf("file size is %d\n",file_size);
	char *filename = "/home/yair/atari/hello";

	char *buff;
	int len;
	read_file(filename,&buff,&len);

	/* create dos2 formatted disk */
	struct atr new_atr;
	atr_new_empty (&new_atr,"atr5b.atr");
	struct filesystem *new_fs = filesystem_format (&new_atr.device,DOS25,1);
	filesystem_write_file( new_fs,2,buff,len,"HELLO","EXE");

	device_flash  (&new_atr.device);
	(void)new_fs;

	/* create stabd alone SW disk */
	char buf1[256];
	int i;
	for (i=0;i<256;i++)
		buf1[i]= i%256;
	atr_new_empty (&new_atr,"atr6.atr");
	if ( read_file(filename,&buff,&len)<0)
	{
		printf("cannot read file %s",filename);
	}

	device_create_boot_sector (&new_atr.device,buff, len);
	device_flash  (&new_atr.device);


	return 0;
}



#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

	int
main2 (int argc, char **argv)
{
	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt (argc, argv, "abc:")) != -1)
		switch (c)
		{
			case 'a':
				aflag = 1;
				break;
			case 'b':
				bflag = 1;
				break;
			case 'c':
				cvalue = optarg;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
				return 1;
			default:
				abort ();
		}

	printf ("aflag = %d, bflag = %d, cvalue = %s\n",
			aflag, bflag, cvalue);

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	return 0;
}

	unsigned char
hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return (unsigned char)hash;
}

struct params {
	int start_sector;
	int end_sector;
};
void (*command[256])(struct params*, char **argv,char argc);


void hexdump(unsigned char *buf_in,int len)
{
	int i,j;
	unsigned char buf[16];
	for (i=0;i<len/16;i+=1) {
		memcpy (buf,&buf_in[i*16],16);
		for (j=0;j<16;j++)
			if (buf[j]<32)
				buf[j]=32;
		printf("%04x     %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",i*16,buf_in[i*16+0],buf_in[i*16+1],buf_in[i*16+2],buf_in[i*16+3],buf_in[i*16+4],buf_in[i*16+5],buf_in[i*16+6],buf_in[i*16+7],
	     buf_in[i*16+8],buf_in[i*16+9],buf_in[i*16+10],buf_in[i*16+11],buf_in[i*16+12],buf_in[i*16+13],buf_in[i*16+14],buf_in[i*16+15],
	     buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],
	     buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15]);
	}
}
void ls(struct params  *params, char **argv,char argc)
{
//./unittest/atr ls  /home/yair/atari/atari_old/mydisks/disk3_2016.atr
	char *usage = "\
atr-ls - manipulate atari file format. By yair gadelov yair.gadelov@gmail.com 	\n\n\
Usage:	atr ls [arguments] file		shows atr/1050 disk content		\n\
Arguments: 									\n\
-h,--help			Display this help				\n\
";
	if (optind < argc) {
		struct atr atr;
		char *atrfile = argv[2];
		if (atr_new_from_file(&atr,atrfile)<0)
		{
			printf("error reading ATR file named %s\n",atrfile);
			exit(-1);
		}
		struct filesystem *fs = filesystem_init (&atr.device);
		printf("Current file: %s, Filesystem: %s\n",atrfile,"dos3");
		filesystem_read_directory (fs);
		display_show_dir(fs);

	}

}


void dump(struct params  *params, char **argv,char argc)
{
		char *usage = "\
atr-dump - manipulate atari file format. By yair gadelov yair.gadelov@gmail.com 	\n\n\
Usage:	atr dump [arguments] file		dump  sectort atr/1050 disk content		\n\
Arguments: 									\n\
-h,--help			Display this help				\n\
--first-sector			fitst sector to dump				\n\
-n				number of sectors to dump			\n\
";

	int i,j,n_sector;
	char *buf,*atrfile;
	struct filesystem *fs;
	struct atr atr;	
	if (optind < argc) {
		atrfile = argv[2];
		if (atr_new_from_file(&atr,atrfile)<0)
		{
			printf("error reading ATR file named %s\n",atrfile);
			return -1;
		}

		n_sector = 3;
		buf =  malloc(device_sector_size (&atr.device)*n_sector);
		fs = filesystem_init (&atr.device);
		for (i=0;i<n_sector;i++) {
			memcpy (&buf[i*device_sector_size (&atr.device)],  device_read_sector(fs->device,i),device_sector_size (&atr.device) );
		
		}
	}
	hexdump (buf,n_sector*device_sector_size (&atr.device));
	free(buf);
}


/**
 * Created  12/17/2016
 * @brief   setup array of commands
 * @param   
 * @return  
 */
void setup_command()
{
	memset (command,0,256);
	command[hash("ls")] = ls;
	command[hash("dump")] = dump;
}

char *usage = "\
atr - manipulate atari file format. By yair gadelov yair.gadelov@gmail.com 	\n\n\
Usage:	atr [arguments] [file]		manipulate atr file 			\n\
or:  atr [arguments] -		read from stdin					\n\
Arguments: 									\n\
-h,--help			Display this help				\n\
-d,--dir				Display disk directory 				\n\
--dump-sector 			Dumps disk sector				\n\
--first				First sector to dump				\n\
--last				Last sector to dump				\n\
--dump										\n\
";

#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>    /* for getopt_long; POSIX standard getopt is in unistd.h */
//https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Options.html
int main (int argc, char **argv) {


	main1();
	int c;
	int digit_optind = 0;
	int aopt = 0, bopt = 0;
	char *copt = 0, *dopt = 0;
	int *flag=123;
	struct params  params;
	static struct option long_options[] = {
		{"dir", no_argument, 0, 'd'},
		{"dump-sector", no_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{"add", optional_argument, 0, 0},
		{"append", no_argument, 0, 0},
		{"delete", 1, 0, 0},
		{"verbose", optional_argument,  0,0},
		{"create", 1, 0, 'c'},
		{"file", required_argument,0, 0},
		{NULL, 0, NULL, 0}
	};
	setup_command();
	int option_index = 0;
	if (command[hash(argv[1])]==0)
	{
		printf("invalid command uption\n%s",usage);
		exit(-1);
	}
	unsigned char idx = hash(argv[1]);
	while ((c = getopt_long(argc, argv, "hdabc:R:012v:",
					long_options, &option_index)) != -1) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
			case 'd':
				if (0 && optind < argc) {
					struct atr atr;
					char *atrfile = argv[optind++];
					if (atr_new_from_file(&atr,atrfile)<0)
					{
						printf("error reading ATR file named %s\n",atrfile);
						exit(-1);
					}
					struct filesystem *fs = filesystem_init (&atr.device);
					printf("Current file: %s, Filesystem: %s\n",atrfile,"dos3");
					filesystem_read_directory (fs);
					display_show_dir(fs);

				}
				else
					printf("wrong usage: atr -d FILE\n"); 
				break;
			case 'h':
				printf("%s\n",usage);
				break;
			case 0:
				printf ("option %s %d", long_options[option_index].name,long_options[option_index].flag);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;
			case '0':
			case '1':
			case '2':
				if (digit_optind != 0 && digit_optind != this_option_optind)
					printf ("digits occur in two different argv-elements.\n");
				digit_optind = this_option_optind;
				printf ("option %c\n", c);
				break;
			case 'a':
				printf ("option a\n");
				aopt = 1;
				break;
			case 'b':
				printf ("option b\n");
				bopt = 1;
				break;
			case 'c':
				printf ("option c with value '%s'\n", optarg);
				copt = optarg;
				break;
			case 'v':
				printf ("Option v with value '%s'\n", optarg);
				copt = optarg;

				break;
			case 'R':
				printf ("option R with value '%s'\n", optarg);
				dopt = optarg;
				break;
			case '?':
				break;
			default:
				printf ("?? getopt returned character code 0%o ??\n", c);
		}
	
	}
	/* skip to command to parameter list */
	command[idx](&params,argv,argc);
	
	/*
	if (optind < argc) {
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf ("\n");
	}*/
	exit (0);
}

