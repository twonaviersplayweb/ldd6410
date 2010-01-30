/*
 * LDD6410 USB mice test programs
 * Copyright 2009 LiHacker Computer Technology Inc.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/mman.h>

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

void draw_rect(int x_s, int x_e, int y_s, int y_e,  int color)
{
	int x, y;

	for (y = y_s; y < y_e; y++) {
		for (x = x_s; x < x_e; x++) {

			unsigned long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				(y+vinfo.yoffset) * finfo.line_length;

			*((unsigned short int*)(fbp + location)) = color;
		}
	}
}

int main()
{
	int fbfd = 0;
	int keyfd = 0;
	struct input_event event;

	long int screensize = 0;
	int x = 0, y = 0;
	int screen_x = 0 , screen_y = 0;

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	keyfd = open("/dev/event1", O_RDWR);
	if (!keyfd) {
		printf("Error: cannot open mice input device.\n");
		exit(1);
	}
	printf("The mice device was opened successfully.\n");


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

	draw_rect(0,vinfo.xres,0,vinfo.yres,0xffff);

	while(1) {
		int ret = read(keyfd, &event, sizeof(struct input_event));
		if (ret == sizeof(struct input_event))
			printf("type:%d code:%d value:%d\n", event.type, event.code, event.value);
		else {
			printf("read len:%d\n", ret);
			continue;
		}

		if(event.type == EV_ABS) {
			if(event.code == ABS_X)
				x = event.value;
			if(event.code == ABS_Y)
				y = event.value;
		}

		if(event.type == 0) {
			/*
			 * convert touchscreen x, y to screen x, y
			 * touchpad (x,y) at the left-top is (5917, 13148)
			 *                at the right-bottom is (10975, 2441)
			 */

			if(x<5917)  x=5917;  if(x>10975) x=10975;
			if(y>13148) x=13148; if(y<2441)  y=2441;

			screen_y = ((x - 5917) * 1.0 / (10975 - 5917)) * vinfo.yres;
			screen_x = ((y - 13148) * 1.0 / (2441 - 13148)) * vinfo.xres;

			if(screen_x >= vinfo.xres - 5) screen_x = vinfo.xres - 5;
			if(screen_y >= vinfo.yres - 5) screen_y = vinfo.yres - 5;
			if(screen_x<0) screen_x = 0;
			if(screen_y<0) screen_y = 0;

			printf("current touched touchpad(%d, %d), screen(%d, %d)\n", x, y, screen_x, screen_y);

			draw_rect(screen_x,screen_x+5,screen_y,screen_y+5, 0);
		}
	}
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
