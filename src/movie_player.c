
#include "Blitter.h"

static void write_movie_to_file(const char*   source_file,
	                        const char*   target_file,
	                        unsigned char first_frame)
{
	FILE* f = fopen(source_file, "rb");
	unsigned int width, height, frame_number;
	unsigned char* data;
	unsigned char info[54] = { 0 };
	fread(info, sizeof(unsigned char), 54, f);
	width  = *(unsigned int*)&info[18];
	height = *(unsigned int*)&info[22];
	unsigned int size = ((width * height * 3) + 31) & ~31;
	data = (unsigned char*)malloc(size);
	fread(data, 1, size, f);
	fclose(f);
	f = fopen(target_file, "a");
	if (first_frame) {
		const char* header = "IMAN_ABDOLLAHZADEH_MOVIE_FRAMES\n";
		fwrite(header, 1, 32, f);
		fwrite(&width,  1, 4, f);
		fwrite(&height, 1, 4, f);
		fwrite(&frame_number, 1, 4, f);
	}
	fwrite(data, 1, size, f);
	fclose(f);
}

static PMOVIE display_movie(PMOVIE mov, void* screen_start) {
	if (!mov) {
		printf("NULL ERROR\n");
		return NULL;
	}
	unsigned int index = mov->frame_number;
	while(index) {
		fast_blit(mov->_32bit_data[mov->frame_number - index], 
		          mov->each_frame_32_byte_number,
		          mov->bmp_height, 
		      	  mov->bmp_width, 
		          screen_start);
		wait();
		index--;
	}
	return mov;
}

static PMOVIE load_movie(const char* address, PMOVIE mov) {
	unsigned int       frame_number       =  0;
	unsigned long long each_frame_24_data =  0;
	unsigned long long each_frame_32_data =  0;
	unsigned int       bmp_width          =  0;
	unsigned int       bmp_height         =  0;
	unsigned int       counter            =  0;
	unsigned char      header[32]         = {0};
	unsigned int       assertion_check    =  0;
	
	mov     = (PMOVIE) malloc(sizeof(MOVIE));
	FILE* f = fopen(address, "rb");
	
	fread(header,        32, 1, f);
	fread(&bmp_width,    4, 1,  f);
	fread(&bmp_height,   4, 1,  f);
	fread(&frame_number, 4, 1,  f);
	each_frame_24_data = ((bmp_width * bmp_height * 3) + 31) & ~31;
	each_frame_32_data = each_frame_24_data * 4 / 3;
		
	mov->header                    = header;
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
		fast_convert_24bit_to_32bit(mov->_24bit_data[counter], 
		                            mov->_32bit_data[counter], 
		                            each_frame_32_data);
	}
	fclose(f);
	return mov;
}

static void release_movie(PMOVIE mov) {
	if (!mov) {
		printf("NULL ERROR DURING FREEING\n");
		return;
	}
	unsigned int index = mov->frame_number;
	for(int i = 0; i < index; i++) {
		free(mov->_32bit_data[i]);
		free(mov->_24bit_data[i]);
	}
	free(mov->_32bit_data);
	free(mov->_24bit_data);
	free(mov);
	printf("movie's memory released\n");
}
