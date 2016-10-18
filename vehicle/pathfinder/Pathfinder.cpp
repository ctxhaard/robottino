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
#include <string.h>
#include <unistd.h>

#define PROXIMITY_ALERT_MM (100)

#define _sr_fr _sensors[0] // sensor front-right
#define _sr_fl _sensors[1] // sensor front-left
#define	_sr_rr _sensors[2] // sensor rear

namespace ct {

Pathfinder::Pathfinder() {
	memset(_sensors,0x00,sizeof(_sensors));

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
		if (fd > 0) {
			_sensors[i] = fd;
		} else {
			_sensors[i] = 0;
			const char *_sensorNames[] = {"Front-left sensor", "Front right sensor", "Rear sensor" };
			perror(_sensorNames[i]);
		}
	}

	int nSensors;
	if ((nSensors = nValidSensors())) {

		const auto line_template = "\rFL xxx* FR xxx* RR xxx*";
		const auto len = strlen(line_template) + 1;
		char line[len];
		memcpy(line,line_template,len);

		while (1) {
			fd_set rfds;
			FD_ZERO(&rfds);

			for (int i=0; i < SENSORS_COUNT; ++i) {
				if (_sensors[i]) FD_SET(_sensors[i],&rfds);
			}

			auto fds = select(FD_SETSIZE,&rfds,NULL,NULL,NULL);
			//fprintf(stderr,"select exited\n");
			if(fds < 0) {
				perror("Error waiting for sensors input");
				result = -1;
				break;
			} else if (fds > 0) {
				//fprintf(stderr,"%d sensors ready to read\n",fds);
				for (auto i = 0; i < SENSORS_COUNT; ++i) {
					if (FD_ISSET(_sensors[i],&rfds)) {
						char buffer[6];
						int count;
						if((count = getSensorString(_sensors[i],buffer))) {
							memcpy(line + 4 + (i * 8),buffer,count);
							fputs(line,stderr);

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

int Pathfinder::getSensorString(int fd, char* buffer) {
	if(!buffer) {
		return 0;
	}
	ssize_t count = 0;
	if((count = read(fd,buffer,6))) {
		//fprintf(stderr,"read %d bytes\n",count);
		buffer[count] = '\0';
		char *endptr;
		int val =  strtol(buffer,&endptr,10);
		if(endptr > buffer) {
			//char *pline = line_template;
			count = sprintf(buffer,"%3d%s",
					(val / 10),
					(val < PROXIMITY_ALERT_MM ? "*" : " "));
		}
	}
	return count;
}

} /* namespace ct */
