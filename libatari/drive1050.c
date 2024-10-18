/*
 * =====================================================================================
 *
 *       Filename:  d2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2024 01:11:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "drive1050.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <termio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

 struct drive1050_state  drive1050_state;



static int open_serial_port(const char* device) {
	int serial_port = open(device, O_RDWR | O_NOCTTY);
	if (serial_port < 0) {
		perror("Error opening serial port");
		return -1;
	}

	struct termios tty;
	memset(&tty, 0, sizeof(tty));

	if (tcgetattr(serial_port, &tty) != 0) {
		perror("Error getting tty attributes");
		close(serial_port);
		return -1;
	}

	// Set Baud Rate to 9600
	cfsetispeed(&tty, B19200);
	cfsetospeed(&tty, B19200);

	// Set 8N1 (8 data bits, No parity, 1 stop bit)
	tty.c_cflag &= ~PARENB; // No parity
	tty.c_cflag &= ~CSTOPB; // 1 stop bit
	tty.c_cflag &= ~CSIZE;  // Mask the character size bits
	tty.c_cflag |= CS8;     // 8 data bits

	tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control
	tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~(ECHO | ECHOE | ECHONL | ISIG); // Disable echo, signals

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off software flow control

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes

	tty.c_cc[VMIN]  = 0;   // Wait for at least one byte to be received
	tty.c_cc[VTIME] = 100;  // Wait up to 1 second (10 deciseconds) for data

	// Save tty settings
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		perror("Error setting tty attributes");
		close(serial_port);
		return -1;
	}

	return serial_port;
}

//https://www.atarimax.com/jindroush.atari.org/asio.html
// http://atariki.krap.pl/index.php/1050-2-PC_/_ProSys
static void
send_data( int fd, unsigned char *buf, int len )
{
	int i, sum = 0;
	int rts_flag = TIOCM_DTR; 

	// Set DTR after sending the data
	if (1 && ioctl(fd, TIOCMBIS, &rts_flag) == -1) {
		perror("ioctl: Unable to set RTS");
		close(fd);
	}

	// Write data + CRC
	for( i=0; i<len; i++ )
	{
		int n = write( fd, &buf[i], 1 );	
		(void)n;
		sum = sum + buf[i];
		sum = (sum & 0xff) + (sum >> 8);
		usleep(500);
	}
	write( fd, &sum, 1 );

	// Clear DTR after sending the data
	if (1 && ioctl(fd, TIOCMBIC, &rts_flag) == -1) {
		perror("ioctl: Unable to clear RTS");
		close(fd);
	}

}

//https://www.atarimax.com/jindroush.atari.org/asio.html
// http://atariki.krap.pl/index.php/1050-2-PC_/_ProSys
static int 
receive_data( int fd, unsigned char *buf, int len )
{
	int i, sum = 0,crc;

	drive1050_state.last_cmd_complete_status = 0;
	drive1050_state.last_cmd_ack = 0;

	read( fd, &drive1050_state.last_cmd_ack, 1 );
	if (drive1050_state.last_cmd_ack == 'N')
		return -2;
	read( fd, &drive1050_state.last_cmd_complete_status, 1 );
	if (drive1050_state.last_cmd_complete_status == 'E')
		return -3;
	// c == 'C'

	for( i=0; i<len; i++ )
	{
		read( fd, &buf[i], 1 );	
		sum = sum + buf[i];
		sum = (sum & 0xff) + (sum >> 8);
	}
	read( fd, &crc, 1 );
	if (crc == sum)
		return   0; 
	return -4;
}


static int drive1050_get_status() {
	unsigned char message[] = {0x31,0x53,0,0};

	send_data(drive1050_state.fd, message,sizeof(message));
	int n = receive_data(drive1050_state.fd,(unsigned char*)&drive1050_state.disk_drive_status,sizeof(struct disk_drive_status));
	return n;
};

static int drive1050_sector_size() {
	return drive1050_state.disk_drive_status.DriveStatus_sector_size == 0 ? 128 : 256;
}

static int drive1050_num_of_tracks() {
	return drive1050_state.disk_drive_status.DriveStatus_medium_density ? 26 : 18;
}




static int drive1050_sector_per_tracks() {
	return 40;
}


 int drive1050_num_of_sectors() {
	return  drive1050_sector_per_tracks() * drive1050_num_of_tracks();
}


 uint32_t drive1050_size() {
	return drive1050_num_of_sectors() * drive1050_sector_size() ; 
}
int drive1050_open(const char* serial_device) {

	drive1050_state.fd  = open_serial_port(serial_device);
	if (drive1050_state.fd < 0) {
		return -1;
	}

	drive1050_get_status();
	return 0;
}

void * drive1050_read_sector(unsigned short sector_id) {
	sector_id++;	
	unsigned char message[] = {0x31,0x52,sector_id % 256,sector_id/256};

	send_data(drive1050_state.fd, message,sizeof(message));
	usleep(1000);
	drive1050_state.err = receive_data (drive1050_state.fd,drive1050_state.sector_buffer, drive1050_sector_size());
	
	return drive1050_state.sector_buffer;

}
#if 0
#include "atr.h"

//gcc -o a drive1050.c atr.c -O0 -g
int main()
{
	drive1050_open( "/dev/ttyUSB0");
	struct atr atr;
	unsigned char * b;
	
	uint16_t size = drive1050_size();
	printf ("start copy. disk size %d\n",size);



	atr_new_empty (&atr,size,"copy.atr");

	for (int i = 0; i < drive1050_num_of_sectors();i++) {
		b = drive1050_read_sector(i);
		if (drive1050_state.last_cmd_complete_status == 'C' &&  drive1050_state.last_cmd_ack=='A') {
			atr_write_sector(&atr,i,b);
			printf ("good %d\n",i);
		}
		else
		{printf("faile to read %d\n",i);}
	}

	atr_flash(&atr);
}

#endif
