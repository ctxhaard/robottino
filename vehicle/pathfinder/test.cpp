/*
 * test.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: ctomasin
 */

#define BOOST_TEST_MODULE pathfindertest
#include <boost/test/included/unit_test.hpp>

#define protected public
#include "Pathfinder.h"

BOOST_AUTO_TEST_SUITE (pathfindertest)

BOOST_AUTO_TEST_CASE(test1)
{
	ct::Pathfinder p;
	char buffer[] = "1250";
	p.formatData(buffer,buffer,sizeof(buffer));
	BOOST_TEST_CHECK(0 == strcmp(buffer,"125 "));
}

BOOST_AUTO_TEST_CASE(test2)
{
	ct::Pathfinder p;
	char buffer[] = "90";
	p.formatData(buffer,buffer,sizeof(buffer));
	BOOST_TEST_CHECK(0 == strcmp(buffer,"  9*"));
}

BOOST_AUTO_TEST_SUITE_END()
