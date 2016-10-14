/*
 * Pathfinder.h
 *
 *  Created on: Oct 14, 2016
 *      Author: ctomasin
 */

#ifndef PATHFINDER_H_
#define PATHFINDER_H_

namespace ct {

class Pathfinder {
public:
	Pathfinder();
	virtual ~Pathfinder();

	int run();

protected:
	int nValidSensors();
	void closeSensors();

protected:
	int _sr_fr; // sensor front-right
	int _sr_fl; // sensor front-left
	int _sr_rr; // sensor rear
};

} /* namespace ct */

#endif /* PATHFINDER_H_ */
