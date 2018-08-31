
#ifndef B_L_I_T_T_E_R__H__
#define B_L_I_T_T_E_R__H__
//////////////////////////////////////////////////////////////////////
#include <linux/fb.h>		
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>	
#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////
#define MASK_0_24_TO_32 0x0000000000FFFFFF
#define MASK_1_24_TO_32 0x0000FFFFFF000000
#define MASK_2_24_TO_32 0xFFFF000000000000
#define MASK_3_24_TO_32 0x00000000000000FF
#define MASK_4_24_TO_32 0x00000000FFFFFF00
#define MASK_5_24_TO_32 0x00FFFFFF00000000
#define MASK_6_24_TO_32 0xFF00000000000000
#define MASK_7_24_TO_32 0x000000000000FFFF
#define MASK_8_24_TO_32 0x000000FFFFFF0000
#define MASK_9_24_TO_32 0xFFFFFF0000000000

#define ALIGN_64(X)     ((X) + 63) & ~63
#define ALIGN_32(X)     ((X) + 31) & ~31
#define ALIGN_16(X)     ((X) + 15) & ~15
#define ALIGN_8(X)      ((X) + 7 ) & ~7
//////////////////////////////////////////////////////////////////////
#define CHECK_DOUBLE_NUMBERS(_0, _1)     ((_0) == (_1))
#define CHECK_TRIPLE_NUMBERS(_0, _1, _2) (((_0) == (_1)) == (_2))
//////////////////////////////////////////////////////////////////////
typedef struct fb_var_screeninfo VariableScreen;
typedef struct fb_fix_screeninfo FixScreen;
//////////////////////////////////////////////////////////////////////
typedef struct _BMP {
	unsigned int   width;
	unsigned int   height;
	unsigned char* pixels;
} BMP;
//////////////////////////////////////////////////////////////////////
typedef struct _MOVIE {
	unsigned int       bmp_width;
	unsigned int       bmp_height;
	unsigned char      header[2];
	unsigned int       frame_number;
	unsigned long long each_frame_24_byte_number;
	unsigned long long each_frame_32_byte_number;
	unsigned char**    _24bit_data;
	unsigned char**    _32bit_data;
} MOVIE, *PMOVIE;
//////////////////////////////////////////////////////////////////////
static void fast_convert_24bit_to_32bit(const void*  _24bits,
	                                void*        _32bits,
	                                const size_t _32bits_bytes);
//////////////////////////////////////////////////////////////////////
static void fast_color_screen(void*              addr,
			      unsigned long long color,
	                      const size_t       bytes);
//////////////////////////////////////////////////////////////////////
static void fast_blit(void*  src, 
		      size_t src_bytes, 
		      size_t number_of_lines, 
		      size_t image_width_cutoff, 
		      void*  screen_pointer);
//////////////////////////////////////////////////////////////////////
static void display(const char*      address, 
		    unsigned char*   screen_start);
//////////////////////////////////////////////////////////////////////
static void display_movie(PMOVIE mov);
//////////////////////////////////////////////////////////////////////
static void load_movie(const char* address, PMOVIE mov);
//////////////////////////////////////////////////////////////////////
static void release_movie(PMOVIE mov);
//////////////////////////////////////////////////////////////////////
static unsigned int pixel_color(unsigned char   r, 
				unsigned char   g, 
				unsigned char   b, 
                                VariableScreen* vinfo);
//////////////////////////////////////////////////////////////////////

#endif
