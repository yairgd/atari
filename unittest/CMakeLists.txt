

#set_target_properties(send_image PROPERTIES LINK_FLAGS "-lusb")
#include_directories( ${host_driver_H})
#add_executable (atr
#	./atr.c
#	)
#target_link_libraries (atr
#	atari	
#	)

#add_executable (dos2_disk_code
#	./dos2_disk_code.c
#	)
#target_link_libraries (dos2_disk_code
#	atari	
#	)


add_executable (copy_disk
	./copy_disk.c
	)

target_link_libraries (copy_disk
	atari	
	)


#install (TARGETS Common DESTINATION bin)
#install (FILES Common_HEADER DESTINATION include)
