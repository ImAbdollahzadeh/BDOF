
#include "Blitter.h"

static void display_movie(PMOVIE mov) {
	if (!mov) 
		return;
	unsigned int index = mov->frame_number;
	
	do {
		fast_convert_24bit_to_32bit(mov->_24bit_data[mov->frame_number - index], 
		                            mov->_32bit_data[mov->frame_number - index], 
		                            mov->each_frame_32_byte_number[mov->frame_number - index]);
	} while(index--);
	do {
		fast_blit(mov->_32bit_data[mov->frame_number - index], 
		          mov->each_frame_32_byte_number[mov->frame_number - index]);
		wait();
	} while(index--);
}

static void load_movie(const char* address, PMOVIE mov) {
	mov     = (PMOVIE) malloc(sizeof(MOVIE));
	FILE* f = fopen(address, "rb");
	
	/* all allocation stuff here */
	
	fclose(f);
}

static void release_movie(PMOVIE mov) {
	unsigned int i;
	unsigned int index = mov->frame_number;
	for(i = 0; i < index; i++) {
		free(_32bit_data[i]);
		free(_24bit_data[i]);
	}
	free(_32bit_data);
	free(_24bit_data);
	free(mov);
	printf("movie's memory released\n");
}
