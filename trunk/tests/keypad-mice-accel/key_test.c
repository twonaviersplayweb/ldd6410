/*
 * LDD6410 key test programs
 * Copyright 2009 LiHacker Computer Technology Inc.
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <sys/time.h>

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

void draw_rect(int x_s, int x_e, int y_s, int y_e,  int color)
{
	int x, y;

	/* for WANXIN LCD, let x,y smaller */

	x_s = (x_s * 1.0/1024) * vinfo.xres;
	x_e = (x_e * 1.0/1024) * vinfo.xres;
	y_s = (y_s * 1.0/768.0) * vinfo.yres;
	y_e = (y_e * 1.0/768.0) * vinfo.yres;

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

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	keyfd = open("/dev/input/event0", O_RDWR);
	if (!keyfd) {
		printf("Error: cannot open key input device.\n");
		exit(1);
	}
	printf("The key device was opened successfully.\n");


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

	draw_rect(0, 1024, 0, 768, 0);
	draw_rect(100,200,400,500,0x1f);
	draw_rect(350,450,320,420,0x1f);
	draw_rect(550,650,320,420,0x1f);
	draw_rect(350,450,520,620,0x1f);
	draw_rect(550,650,520,620,0x1f);
	draw_rect(800,900,400,500,0x1f);

	int left = 0,right = 0;

	while(1) {
		int ret = read(keyfd, &event, sizeof(struct input_event));
		if (ret == sizeof(struct input_event))
			printf("type:%d code:%d value:%d\n", event.type, event.code, event.value);
		else
			continue;

		if( event.type == 1) {
			if (event.code == KEY_MENU) {
				draw_rect(100,200,400,500,event.value > 0 ? (0x1f << 11):0x1f);
				left = event.value;
			}
			if (event.code == KEY_POWER)
				draw_rect(350,450,320,420,event.value > 0 ? (0x1f << 11):0x1f);
			if (event.code == KEY_BACK)
				draw_rect(350,450,520,620,event.value > 0 ? (0x1f << 11):0x1f);
			if (event.code == KEY_HOME)
				draw_rect(550,650,320,420,event.value > 0 ? (0x1f << 11):0x1f);
			if (event.code == KEY_VOLUMEUP)
				draw_rect(550,650,520,620,event.value > 0 ? (0x1f << 11):0x1f);
			if (event.code == KEY_VOLUMEDOWN) {
				draw_rect(800,900,400,500,event.value > 0 ? (0x1f << 11):0x1f);
				right = event.value;
			}
			if (left && right)
				exit(0);
		}
	}
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
