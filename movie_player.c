
#include "Blitter.h"

typedef struct _MOVIE {
	unsigned int       bmp_width;
	unsigned int       bmp_height;
	const char*        header;
	const char*        header_code;
	const char*        frames;
	unsigned int       frame_number;
	const char*        total_bytes;
	unsigned long long total_byte_number;
	unsigned int*      each_frame_24_byte_number;
	unsigned int*      each_frame_32_byte_number;
	unsigned char**    _24bit_data;
	unsigned char**    _32bit_data;
	const char*        end_of_file;
} MOVIE, *PMOVIE;

static void display_movie(PMOVIE mov) {
	if (!mov) 
		return;
	unsigned int index = mov->frame_number;
	
	do {
		fast_24_to_32_convert(mov->_24bit_data[mov->frame_number - index], 
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
	unsigned int index = mov->frame_number;
	for(int i = 0; i < index; i++) {
		free(_32bit_data[i]);
		free(_24bit_data[i]);
	}
	free(_32bit_data);
	free(_24bit_data);
	free(mov);
	printf("movie's memory released\n");
}

int main(void) {
	PMOVIE mov;
	load_movie("Spider_man", mov);
	display_movie(mov);
	release_movie(mov);
}
