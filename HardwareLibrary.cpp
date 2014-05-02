#include <Wire.h>

class Gyroscope{
	int address;
	public:
		Gyroscope(int i2cAddress){ address=i2cAddress; }
		void init(){
			Wire.beginTransmission(address);
			//Wire.write(0x00);
			//Configuration registers for gyro go here
			Wire.endTransmission();
		}
		int read(){
			int returnVal;
			Wire.beginTransmission(address);

			Wire.endTransmission();

			//Assuming from previous experience that value is 2 byte
			Wire.requestFrom(address, 2);
			while(Wire.available() < 2);
			returnVal=Wire.read() <<8;
			returnVal |= Wire.read();
		}
};

class Magnetometer{
	//
};

class IRSensor{
	//
};

class Motor{

};

class MarkerSystem{

};

class Drive{
	Gyroscope gyro;
	Motor leftMotor;
	Motor rightMotor;
	//
}