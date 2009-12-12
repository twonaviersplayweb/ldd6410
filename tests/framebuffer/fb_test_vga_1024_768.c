#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main()
{
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	int x = 0, y = 0;
	long int location = 0;

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		printf("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	// Figure out where in memory to put the pixel
	for (y = 0; y < 200; y++) {
		for (x = 0; x < 1024; x++) {

			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				(y+vinfo.yoffset) * finfo.line_length;

			int b = x / 32;
			int g = 0;
			int r = 0;
			unsigned short int t = r<<11 | g << 5 | b;
			*((unsigned short int*)(fbp + location)) = t;
		}
	}	
	for (y = 200; y < 400; y++) {
		for (x = 0; x < 1024; x++) {

			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				(y+vinfo.yoffset) * finfo.line_length;

			int b = 0;
			int g = x / 16;
			int r = 0;
			unsigned short int t = r<<11 | g << 5 | b;
			*((unsigned short int*)(fbp + location)) = t;
		}
	}	
	for (y = 400; y < 600; y++) {
		for (x = 0; x < 1024; x++) {

			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				(y+vinfo.yoffset) * finfo.line_length;

			int b = 0;
			int g = 0;
			int r = x / 32;
			unsigned short int t = r<<11 | g << 5 | b;
			*((unsigned short int*)(fbp + location)) = t;
		}
	}
	for (y = 600; y < 768; y++) {
		for (x = 0; x < 1024; x++) {

			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				(y+vinfo.yoffset) * finfo.line_length;

			int b = 0x1f;
			int g = 0x3f;
			int r = 0x1f;
			unsigned short int t = r<<11 | g << 5 | b;
			*((unsigned short int*)(fbp + location)) = t;
		}
	}

	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
