#include "../HardwareLibrary.cpp"

Motor mLeft(7,6);
Motor mRight(4, 5);

void setup(){
	mLeft.init();
	mRight.init();
}

void loop(){
	//Full forwards
	mLeft.setMotor(255,0);
	mRight.setMotor(255,0);
	delay(1000);
	//Full backwards
	mLeft.setMotor(255,1);
	mRight.setMotor(255,1);
	delay(1000);
	//Decreased forwards
	mLeft.setMotor(150,0);
	mRight.setMotor(150,0);
	delay(1000);
	//Decreased backwards
	mLeft.setMotor(150,1);
	mRight.setMotor(150,1);
	delay(1000);

	//Full spin left
	mLeft.setMotor(255,0);
	mRight.setMotor(255,1);
	delay(1000);

	//Full spin right
	mLeft.setMotor(255,1);
	mRight.setMotor(255,0);
	delay(1000);
}