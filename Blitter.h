
#ifndef B_L_I_T_T_E_R__H__
#define B_L_I_T_T_E_R__H__
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/fb.h>		
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>	
#include <iostream>	
#include <chrono>	
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_convert_24bit_to_32bit(const void*  _24bits,
	                                      void*        _32bits,
	                                      const size_t _32bits_bytes);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_color_screen(void*              addr,
			                        unsigned long long color,
	                            const size_t       bytes);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_blit(void*  src, 
		                  size_t src_bytes, 
		                  size_t number_of_lines, 
		                  size_t image_width_cutoff, 
		                  void*  screen_pointer);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void display(const char*      address, 
		                FixScreen*       pfinfo, 
		                VariableScreen*  pvinfo, 
		                unsigned char*   screen_start);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void display_movie(PMOVIE mov);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void load_movie(const char* address, PMOVIE mov);
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void release_movie(PMOVIE mov);
////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
