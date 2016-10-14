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
#include <unistd.h>

namespace ct {

Pathfinder::Pathfinder() {
	// TODO Auto-generated constructor stub

}

Pathfinder::~Pathfinder() {
	closeSensors();
}

int Pathfinder::run() {
	int result = 0;
	// TODO: look for /dev/proximity* files
	// select on them
	_sr_fl = open("/dev/proximity0",O_RDONLY);
	if (_sr_fl < 0) perror("Can't open front left proximity sensor");

	_sr_fr = open("/dev/proximity1",O_RDONLY);
	if (_sr_fr < 0) perror("Can't open front right proximity sensor");

	_sr_rr = open("/dev/proximity2",O_RDONLY);
	if (_sr_rr < 0) perror("Can't open rear proximity sensor");

	int nSensors;
	if ((nSensors = nValidSensors())) {

		while (1) {
			fd_set read;

			if (_sr_fl > 0) FD_SET(_sr_fl,&read);
			if (_sr_fr > 0) FD_SET(_sr_fr,&read);
			if (_sr_rr > 0) FD_SET(_sr_rr,&read);

			int fds = select(nSensors+1,&read,nullptr,nullptr,nullptr);
			if(fds < 0) {
				perror("Error waiting for sensors input");
				result = -1;
				break;
			}
		}
	} else {
		result = -2;
	}
	return result;
}

int Pathfinder::nValidSensors() {
	int result = 0;
	if (_sr_fl > 0) ++result;
	if (_sr_fr > 0) ++result;
	if (_sr_rr > 0) ++result;

	return result;
}

void Pathfinder::closeSensors() {
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
