
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Bitmap Drawer Directly on Screen (BDDOS)
 *  Programmer: Iman Abdollahzadeh
 *  19/12/2017; under MIT licence
 *  To get the object file, simply open a terminal, by going to the folder, run this command:
 *  g++ -std=c++0x drawer.cpp -o drawer.o
 *  Then open the X terminal, navigate to the same folder as above, and simply run: sudo ./drawer.o
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Blitter.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
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
	unsigned char* fbp = (unsigned char*)mmap(0, 
						  screensize, 
						  PROT_READ | PROT_WRITE, 
						  MAP_SHARED, 
						  fb, 
						  (off_t)0);

	fast_color_screen(fbp, 
			  0x00afcb0000afcb00, 
			  screensize);

	//for(int j = 0; j < 500; j++)
		display("SampleImage.bmp", fbp);

	return 0;
}
