#include <unistd.h>
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

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");
	
	keyfd = open("/dev/event2", O_RDWR);
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

	draw_rect(300,400,400,500,0x1f);
	draw_rect(500,600,400,500,0x1f); 
	draw_rect(700,800,400,500,0x1f);
	
	int screen_x = 100, screen_y = 100;
	int left=0,right=0;

	while(1) {
		int ret = read(keyfd, &event, sizeof(struct input_event));
		if (ret == sizeof(struct input_event))
			printf("type:%d code:%d value:%d\n", event.type, event.code, event.value);
		else {
			printf("read len:%d\n", ret);
			continue;
		}

		if(event.type == EV_KEY) {
			if (event.code == BTN_LEFT) {
				draw_rect(300,400,400,500,event.value > 0 ? (0x1f << 11):0x1f);
				left = event.value;
			}
			if (event.code == BTN_MIDDLE)
				draw_rect(500,600,400,500,event.value > 0 ? (0x1f << 11):0x1f);	
			if (event.code == BTN_RIGHT) {
				draw_rect(700,800,400,500,event.value > 0 ? (0x1f << 11):0x1f);
				right = event.value;
			}
		}
		if (left && right)
			exit(0);

		if(event.type == EV_REL) {
			if(event.code == REL_X)
				x = event.value;	
			if(event.code == REL_Y)
				y = event.value;
		}
		if(event.type == 0) {
			screen_x += x;
			screen_y += y;
			if(screen_x>=1014) screen_x = 1014;
			if(screen_y>=758) screen_y = 758;		
			
			if(screen_x<0) screen_x = 0;
			if(screen_y<0) screen_y = 0;
			draw_rect(screen_x,screen_x+10,screen_y,screen_y+10, 0);
			x = y =0;
		}
	}
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
