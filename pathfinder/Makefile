# note: gcc < 5.0 bug (https://stackoverflow.com/questions/29320315/error-use-of-deleted-function-stdbasic-ofstream-opencv-and-c11)
#PATH := /home/ctomasin/development/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf/bin:$(PATH)
PATH := /home/ctomasin/development/gcc-linaro-5.1-2015.08-x86_64_arm-linux-gnueabihf/bin:$(PATH)
CROSS_COMPILE ?= arm-linux-gnueabihf-

CXXFLAGS := -std=c++14 -Wall -fmessage-length=0 -fexceptions -fdiagnostics-color -pedantic 
ifeq ($(NDEBUG),)
	CXXFLAGS += -g 
else
	CXXFLAGS += -O2 
endif

ARCH := arm

OBJS :=	Pathfinder.o ProximitySensor.o MotorController.o PFStatus.o

TARGET := pathfinder

ifeq ($(MAKECMDGOALS),test)
CXXFLAGS += -I /home/ctomasin/development/github/googletest/googletest/include 
LDFLAGS = -L /home/ctomasin/development/github/googletest/googlemock/gtest
LIBS += -lgtest -lgtest_main
OBJS += test.o
TARGET := test
else
OBJS += main.o
endif
LIBS += -lpthread

USER := root
REMOTE := robottino-wifi

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CROSS_COMPILE)$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)
	
.PHONY: clean
clean:
	rm -f *.o $(TARGET) pathfinder-test
	
.PHONY: load	
load:
	scp $(TARGET) $(USER)@$(REMOTE):
