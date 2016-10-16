/*
 * Pathfinder.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: ctomasin
 */

#include "Pathfinder.h"
#include <sys/select.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROXIMITY_ALERT_MM (100)

#define _sr_fr _sensors[0] // sensor front-right
#define _sr_fl _sensors[1] // sensor front-left
#define	_sr_rr _sensors[2] // sensor rear

namespace ct {

Pathfinder::Pathfinder() {
	// TODO Auto-generated constructor stub

}

Pathfinder::~Pathfinder() {
	closeSensors();
}

int Pathfinder::run() {
	int result = 0;

	for (int i=0; i < SENSORS_COUNT; ++i) {

		char path[32];
		sprintf(path,"/dev/proximity%d",i);
		int fd = open(path,O_RDONLY);
		if (fd > 0) _sensors[i] = fd;
		else {
			const char *_sensorNames[] = {"Front-left sensor", "Front right sensor", "Rear sensor" };
			perror(_sensorNames[i]);
		}
	}

	int nSensors;
	if ((nSensors = nValidSensors())) {

		while (1) {
			fd_set rfds; FD_ZERO(&rfds);

			for (int i=0; i < SENSORS_COUNT; ++i) {
				if (_sensors[i]) FD_SET(_sensors[i],&rfds);
			}

			int fds = select(nSensors+1,&rfds,nullptr,nullptr,nullptr);
			if(fds < 0) {
				perror("Error waiting for sensors input");
				result = -1;
				break;
			} else if (fds > 0) {

				for(int i = 0; i < SENSORS_COUNT; ++i) {

					if (FD_ISSET(_sensors[i],&rfds)) {
						char buffer[16];
						if(read(_sensors[i],buffer,16)) {
							const char *padding[] = {"FL", "      FR", "            RR"};

							int val = atoi(buffer);

							printf("\r%s%3d%s",
									padding[i],
									(val / 10),
									(val < PROXIMITY_ALERT_MM ? "*": ""));
						}
					}
				}
			}
		}
	} else {
		result = -2;
	}
	return result;
}

int Pathfinder::nValidSensors() {
	int result = 0;

	for (int i = 0; i < SENSORS_COUNT; ++i) {

		if (_sensors[i] > 0) ++result;
	}
	return result;
}

void Pathfinder::closeSensors() {
	// TODO: iterate trhough_sensors[] instead of repeat the operation
	if (_sr_fl > 0) {
		close(_sr_fl); _sr_fl = 0;
	}
	if (_sr_fr > 0) {
		close(_sr_fr); _sr_fr = 0;
	}
	if (_sr_rr > 0) {
		close(_sr_rr); _sr_rr = 0;
	}
}

} /* namespace ct */
