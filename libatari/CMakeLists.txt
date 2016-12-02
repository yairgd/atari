
SET(CMAKE_C_COMPILER   	gcc)
SET(CMAKE_CXX_COMPILER 	g++)

set (CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -DPIC")
set (CMAKE_C_FLAGS_DEBUG "-Wall -g -O0 -DPIC")


set (atari_SRC
	dos2_format.c
	device.c
	atr.c
	filesystem.c
	dos2.c
	dos3.c
	display.c
)	





set (atari_H
	device.h
	atr.h
	filesystem.h
	dos2.h
	dos3.h
	display.h
	)


add_library(atari 
	${atari_SRC}
	${atari_H}
	)

#INSTALL(TARGETS atari
#        DESTINATION "lib"
#        )
#INSTALL(FILES ${atari_H}
#        DESTINATION "../include"
#        )

