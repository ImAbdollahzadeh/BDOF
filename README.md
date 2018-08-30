# BDDOS

This is a Bitmap decoder which gets the actual address of a .bmp file and then displays it directly on the video card's frame buffer.
To get access to frame buffer, one has to call the frame buffer device file with the use of ioctl call.

The monitoring itself, all takes place in X mode. 

This little package has been tested for various bitmap files, but feel free to test it for any other .bmp files and in case of 
"segmentation fault" or overflow of frame buffer, better to shrink the size of original file and again try to get it displayed on monitor.

Any bug or error can be reported directly to me via GitHub or sending e-mail to my yahoomail: iman_ab2013@yahoo.com



Edit:
with the fast blitting, fast converting, and fast displaying, it would be possible to map frames of a movie in a reasonable frquency without having flickr in viewing.

The time between execuation of old_display and fast_display functions is ~20x faster. With the fast blitting functionalities, we could blit an image into the entire display in  ~67 Hz.
