
#include "Blitter.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned int pixel_color(unsigned char   r, 
				unsigned char   g, 
				unsigned char   b, 
				VariableScreen* vinfo)
{
  return (r << vinfo->red.offset) | (g << vinfo->green.offset) | (b << vinfo->blue.offset);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_convert_24bit_to_32bit(const void*  _24bits,
	                                void*        _32bits,
	                                const size_t _32bits_bytes)
{
	unsigned long long* src = (unsigned long long*)_24bits;
	unsigned long long* trg = (unsigned long long*)_32bits;
	size_t byte = _32bits_bytes;
	do {
		*trg++ = ((*src   & MASK_0_24_TO_32) << 8 ) | ((*src   & MASK_1_24_TO_32) << 16);
		*trg++ = ((*src++ & MASK_2_24_TO_32) >> 40) | ((*src   & MASK_3_24_TO_32) << 24) | ((*src & MASK_4_24_TO_32) << 32);
		*trg++ = ((*src   & MASK_5_24_TO_32) >> 24) | ((*src++ & MASK_6_24_TO_32) >> 16) | ((*src & MASK_7_24_TO_32) << 48);
		*trg++ = ((*src   & MASK_8_24_TO_32) >> 8 ) | (*src++  & MASK_9_24_TO_32);
	} while (byte -= 32);
	src  = (unsigned long long*)_32bits;
	byte = _32bits_bytes;
	do {
		*src++ = (*src >> 8);
		*src++ = (*src >> 8);
		*src++ = (*src >> 8);
		*src++ = (*src >> 8);
	} while (byte -= 32);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_color_screen(void*              addr,
			      unsigned long long color,
	                      const size_t       bytes)
{
	unsigned long long* trg = (unsigned long long*)addr;
	size_t byte = bytes;
	do { *trg++ = color; } while (byte -= 8);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void fast_blit(void*  src, 
		      size_t src_bytes, 
		      size_t number_of_lines, 
		      size_t image_width_cutoff, 
		      void*  screen_pointer)
{
	size_t              byte       = image_width_cutoff << 2;
	unsigned long long* trg        = (unsigned long long*)((char*)src + src_bytes - byte);
	unsigned long long* scr        = (unsigned long long*)screen_pointer;
	size_t              ln         = number_of_lines;
	size_t              bbyte      = byte;
	unsigned int        SCREEN_PAD = ((SCREEN_WD - image_width_cutoff)<<2)>>3;
	unsigned int        ind        = 0;
	do {
		do { *scr++ = *trg++; } while (bbyte -= 8);
		bbyte = byte;
		scr += SCREEN_PAD;
		trg = (unsigned long long*)((char*)src + src_bytes - (ind++ * byte));
	} while(ln--);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void display(const char*      address, 
		    unsigned char*   screen_start)
{
	BMP* bmp                       = nullptr;
	FILE* f                        = fopen(address, "rb");
	bmp                            = (BMP*)malloc(sizeof(BMP));
	bmp->width                     = 0;
	bmp->height            	       = 0;
	bmp->_24bits_pixels            = nullptr;
	bmp->_32bits_pixels            = nullptr;
	unsigned char info[54]         = { 0 };
	fread(info, sizeof(unsigned char), 54, f);
	bmp->width             = *(unsigned int*)&info[18];
	bmp->height            = *(unsigned int*)&info[22];
	unsigned int size24    = ((bmp->width * bmp->height * 3) + 31) & ~31;
	bmp->_24bits_pixels    = (unsigned char*)malloc(size24);
	unsigned int size32    = ((bmp->width * bmp->height * 4) + 31) & ~31;
	bmp->_32bits_pixels    = (unsigned char*)malloc(size32);
	fread(bmp->_24bits_pixels, sizeof(unsigned char), size24, f);
	fclose(f);

	fast_convert_24bit_to_32bit(bmp->_24bits_pixels, 
				                      bmp->_32bits_pixels, 
				                      size32);

	fast_blit(bmp->_32bits_pixels, 
		        size32, 
		        bmp->height, 
		        bmp->width, 
		        screen_start);

	free(bmp->_32bits_pixels);
	free(bmp->_24bits_pixels);
	free(bmp);
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
static void old_display(const char*     address, 
		        FixScreen*      pfinfo, 
		        VariableScreen* pvinfo, 
		        unsigned char*  screen_start)
{
	BMP* bmp               = nullptr;
	FILE* f                = fopen(address, "rb");
	bmp                    = (BMP*)malloc(sizeof(BMP));
	bmp->width             = 0;
	bmp->height            = 0;
	bmp->pixels            = nullptr;
	unsigned char info[54] = { 0 };
	fread(info, sizeof(unsigned char), 54, f);
	bmp->width             = *(unsigned int*)&info[18];
	bmp->height            = *(unsigned int*)&info[22];
	unsigned int size      = ((((bmp->width * bmp->height) + 31) & ~31) / 8) * bmp->height;
	bmp->pixels            = (unsigned char*)malloc(size);
	fread(bmp->pixels, sizeof(unsigned char), size, f);
	fclose(f);
	for (int x = 0; x < bmp->width; x++) {
		for (int y = 0; y < bmp->height; y++) {
			long location = x * (pvinfo->bits_per_pixel / 8) + y * pfinfo->line_length;
			*((unsigned int*)(screen_start + location)) = pixel_color(
				bmp->pixels[(((bmp->height - y) * bmp->width + x) * 3) + 2], 
				bmp->pixels[(((bmp->height - y) * bmp->width + x) * 3) + 1], 
				bmp->pixels[(((bmp->height - y) * bmp->width + x) * 3)    ], 
				pvinfo
			);
		}
	}
	free(bmp->pixels);
	free(bmp);
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
