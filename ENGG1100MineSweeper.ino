#include "HardwareLibrary.cpp"

Drive driveSystem(0x25, 7, 6, 4, 5);
Magnetometer magSensor(0x1E, 500); //Not sure what the actual threshold will be - testing required
IRSensor irSensor(A0);
MarkerSystem marker(9, 13);

void setup(){
	driveSystem.init();
	magSensor.init();
	irSensor.init();
	marker.init();
}

void loop(){
	
}