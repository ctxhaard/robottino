/*
 * Pathfinder.h
 *
 *  Created on: Oct 14, 2016
 *      Author: ctomasin
 */

#ifndef PATHFINDER_H_
#define PATHFINDER_H_


#define SENSORS_COUNT (3)

namespace ct {

class Pathfinder {
public:
	Pathfinder();
	virtual ~Pathfinder();

	int run();

protected:
	int nValidSensors();
	void closeSensors();
	int getSensorString(int fd,char *buffer);
	int formatData(char *inBuffer,char *outBuffer,int count) const;

protected:
	int _sensors[SENSORS_COUNT];
};

} /* namespace ct */

#endif /* PATHFINDER_H_ */
