/*
 * =====================================================================================
 *
 *       Filename:  drive1050.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2024 09:52:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef atari_1050drive
#define atari_1050drive 
#include <stdint.h>

struct disk_drive_status {
	//DriveStatus byte
	unsigned char DriveStatus_command_frame_error : 1;
	unsigned char DriveStatus_checksum_error : 1;
	unsigned char DriveStatus_write_error : 1;  //(Data frame error).
	unsigned char DriveStatus_write_protected : 1; //(Operation error/FDC error)
	unsigned char DriveStatus_motor_is_on : 1;
	unsigned char DriveStatus_sector_size : 1; //  (0=$80 1=$100)
	unsigned char DriveStatus_unused : 1; 
	unsigned char DriveStatus_medium_density  : 1;  //(MFM & $80 byte mode)
	
	// Inverted, contains WD2793 Status Register. Depends on command used prior status. 
	unsigned char WD2793_controller_busy : 1;  //
	unsigned char WD2793_data_request_or_index: 1;  //
	unsigned char WD2793_data_lost_or_track_0: 1;  //
	unsigned char WD2793_crc_error: 1;  //
	unsigned char WD2793_record_not_found: 1;  //
	unsigned char WD2793_record_type_or_head_loaded: 1;  //
	unsigned char WD2793_write_protected: 1;  // (Always falase upn read)
	unsigned char WD2793_not_ready: 1;  // (disk removed)

	// Timeout for format (always $E0 ??) - Time the drive will need to format a disk 			  
	unsigned char timeout_format; //Time the drive will need to format a disk

	//  Copy of WD2793 Master status register 
	unsigned char WD2793;
};

struct drive1050_state {
	int fd;
	char addr;
	char devname[32];
	struct disk_drive_status  disk_drive_status;
	char last_cmd_ack;
	char last_cmd_complete_status;
	unsigned char sector_buffer[256];
	int err;
};

struct disk_drive {
	int (*open) ();

};
//int drive1050_open() ;
uint32_t drive1050_size();
void * drive1050_read_sector(unsigned short sector_id);
 int drive1050_num_of_sectors();
int drive1050_open(const char* serial_device) ; 
#endif

