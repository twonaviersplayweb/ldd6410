/*
 * LDD6410 lsm303dlh accel/magne test program
 * Copyright 2009 LiHacker Computer Technology Inc.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/mman.h>

int main()
{
	struct input_event acc_event;
	struct input_event mag_event;
	int acc_fd = 0, mag_fd = 0;

	long int screensize = 0;
	int acc_x = 0, acc_y = 0, acc_z = 0;
	int mag_x = 0, mag_y = 0, mag_z = 0;
	int count = 0;

	acc_fd = open("/dev/event2", O_RDWR);
	if (acc_fd < 0)
		acc_fd = open("/dev/input/event2", O_RDWR);
	mag_fd = open("/dev/event3", O_RDWR);
	if (mag_fd < 0)
		mag_fd = open("/dev/input/event3", O_RDWR);

	while(1) {
		int ret;
		while(1) {
			ret = read(acc_fd, &acc_event, sizeof(struct input_event));
			if (ret != sizeof(struct input_event))
				continue;

			if(acc_event.type == EV_ABS) {
				if(acc_event.code == ABS_X)
					acc_x = acc_event.value;
				if(acc_event.code == ABS_Y)
					acc_y = acc_event.value;
				if(acc_event.code == ABS_Z)
					acc_z = acc_event.value;
			}

			if(acc_event.type == 0)
				break;
		}

		while(1) {
			ret = read(mag_fd, &mag_event, sizeof(struct input_event));
			if (ret != sizeof(struct input_event))
				continue;

			if(mag_event.type == EV_ABS) {
				if(mag_event.code == ABS_X)
					mag_x = mag_event.value;
				if(mag_event.code == ABS_Y)
					mag_y = mag_event.value;
				if(mag_event.code == ABS_Z)
					mag_z = mag_event.value;
			}
			if(mag_event.type == 0)
				break;
		}

		if ((++count%5) == 0)
			printf("accel(%5d, %5d, %5d), magne(%5d, %5d, %5d)\n", acc_x, acc_y, acc_z,
				mag_x, mag_y, mag_z);
	}

	close(acc_fd);
	close(mag_fd);
	return 0;
}
