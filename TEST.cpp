////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Bitmap Drawer Directly on Screen (BDDOS)
 *  Programmer: Iman Abdollahzadeh
 *  19/12/2017; under MIT licence
 *  To get the object file, simply open a terminal, by going to the folder, run this command:
 *  g++ -std=c++0x TEST.cpp -o tst.o
 *  Then open the X terminal, navigate to the same folder as above, and simply run: sudo ./tst.o
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/fb.h>		// to get access to variable and fix screen structs
#include <fcntl.h>		// to open a device file
#include <sys/mman.h>		// get access to mmap
#include <sys/ioctl.h>		// access to ioctl calls
#include <iostream>		// standard C++ library
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct fb_var_screeninfo VariableScreen;
typedef struct fb_fix_screeninfo FixScreen;
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct __BMP {
	int            width;
	int            height;
	unsigned char* pixels;
} BMP;
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int pixel_color(
	unsigned char   r, 
	unsigned char   g, 
	unsigned char   b, 
	VariableScreen* vinfo){
	return (r << vinfo->red.offset) | (g << vinfo->green.offset) | (b << vinfo->blue.offset);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void display(
	const char*     address, 
	FixScreen*      pfinfo, 
	VariableScreen* pvinfo, 
	unsigned char*  screen_start){
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
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
int main( void ){
	FixScreen      finfo;
	VariableScreen vinfo;
	int fb = open("/dev/fb0", O_RDWR);
	if(fb < 0) {
		printf("ERROR!\n");
		return -1;	
	}
	ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
	vinfo.grayscale      = 0;
	vinfo.bits_per_pixel = 32;
	ioctl(fb, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
	long screensize = vinfo.yres_virtual * finfo.line_length;
	unsigned char* fbp = (unsigned char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, (off_t)0);
	for (int x = 0; x < vinfo.xres; x++) {
		for (int y = 0; y < vinfo.yres; y++) {
			long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
			*((unsigned int*)(fbp + location)) = pixel_color(0x00, 0x00, 0x00, &vinfo);
		}
	}
	display("SampleImage.bmp", 
		/* by default; don't change */ &finfo, &vinfo, fbp);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
