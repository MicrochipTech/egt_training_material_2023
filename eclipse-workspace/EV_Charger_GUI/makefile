CXX=~/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++
CXXFLAGS=`~/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/pkg-config libegt --cflags --libs`

ev_charging_app : main.o time_management.o
	$(CXX) $(CXXFLAGS) -o ev_charger_target/ev_charging_app main.o time_management.o

main.o : main.cpp 
		$(CXX) $(CXXFLAGS)-c main.cpp

time_management.o : time_management.cpp time_management.h 
		$(CXX) $(CXXFLAGS)-c time_management.cpp
