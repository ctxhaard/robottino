#include <gtest/gtest.h>
#include "ProximitySensor.h"

struct ProximitySensorTest : public testing::Test
{
	ct::ProximitySensor sensor { "" };

	void SetUp() {

	}

	void TearDown()
	{

	}
};

TEST_F(ProximitySensorTest, ReturnNoIfEmpty) 
{
	ASSERT_FALSE(sensor.isNotDecreasing());	
}

TEST_F(ProximitySensorTest, ReturnNoIfNotEnoughValues) 
{
	sensor.setMm(10);
	sensor.setMm(100);
	ASSERT_FALSE(sensor.isNotDecreasing());	
}

TEST_F(ProximitySensorTest, IncreasingIsRecognized) 
{
	sensor.setMm(10);
	sensor.setMm(9);
	sensor.setMm(8);
	sensor.setMm(8);
	sensor.setMm(8);
	sensor.setMm(9);
	ASSERT_TRUE(sensor.isNotDecreasing());	
}

TEST_F(ProximitySensorTest, IncreasingIsRecognized2) 
{
	sensor.setMm(10);
	sensor.setMm(9);
	sensor.setMm(8);
	sensor.setMm(8);
	sensor.setMm(10);
	sensor.setMm(9);
	ASSERT_TRUE(sensor.isNotDecreasing());	
}
