
#include "Blitter.h"

static void display_movie(PMOVIE mov) {
	if (!mov) return;
	unsigned int index = mov->frame_number;

	do {
		fast_blit(mov->_32bit_data[mov->frame_number - index], 
		          mov->each_frame_32_byte_number[mov->frame_number - index]);
		wait();
	} while(index--);
}

static void load_movie(const char* address, PMOVIE mov) {
	unsigned int       frame_number       =  0;
	unsigned long long each_frame_24_data =  0;
	unsigned long long each_frame_32_data =  0;
	unsigned int       bmp_width          =  0;
	unsigned int       bmp_height         =  0;
	unsigned int       counter            =  0;
	unsigned char      header[2]          = {0};
	unsigned int       assertion_check    =  0;
	
	mov     = (PMOVIE) malloc(sizeof(MOVIE));
	FILE* f = fopen(address, "rb");
	
	fread(header, 2, 1, f);
	fread(&bmp_width,  4, 1, f);
	fread(&bmp_height, 4, 1, f);
	fread(&frame_number, 4, 1, f);
	fread(&each_frame_24_data, 8, 1, f);
	each_frame_32_data             = each_frame_24_data * 4 / 3;
	
	assertion_check = CHECK_TRIPLE_NUMBERS(bmp_width * bmp_width * 3,
					       each_frame_24_data,
					       each_frame_32_data * 3 / 4);
	if (! assertion_check) {
		fclose(f);
		free(mov);
		mov = NULL;
		return;
	}
	
	mov->header[0]                 = header[0];
	mov->header[1]                 = header[1];
	mov->bmp_width                 = bmp_width;
	mov->bmp_height                = bmp_height;
	mov->frame_number              = frame_number;
	mov->each_frame_24_byte_number = each_frame_24_data;
	mov->each_frame_32_byte_number = each_frame_32_data;
	mov->_24bit_data               = (unsigned char**)malloc(frame_number * sizeof(unsigned char*));
	mov->_32bit_data               = (unsigned char**)malloc(frame_number * sizeof(unsigned char*));
	
	for(counter = 0; counter < frame_number; counter++) {
		mov->_24bit_data[counter] = (unsigned char*)malloc(each_frame_24_data);
		fread(mov->_24bit_data[counter], each_frame_24_data, 1, f);
		mov->_32bit_data[counter] = (unsigned char*)malloc(each_frame_32_data);
		
		fast_24_to_32_convert(mov->_24bit_data[counter], 
		                      mov->_32bit_data[counter], 
		                      each_frame_32_data);
	}
	
	fclose(f);
}

static void release_movie(PMOVIE mov) {
	if (!mov) return;
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
