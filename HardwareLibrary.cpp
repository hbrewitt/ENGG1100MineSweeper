#include <Wire.h>

class Gyroscope{
	//Tris10 TrackMotion
	int address;
	public:
		Gyroscope(int i2cAddress){ address=i2cAddress; }
		void init(void){
			Wire.beginTransmission(address);
			//Wire.write(0x00);
			//Configuration registers for gyro go here
			Wire.endTransmission();
		}
		int read(void){
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
	int address;
	int threshold;
	public:
		Magnetometer(int i2cAddress, int mineThreshold){
			threshold=mineThreshold;
			address=i2cAddress;
		}

		void init(void){
			//Writes to registers for setup;

			Wire.beginTransmission(address);
			//Continuous mode
			Wire.send(0x02);
			Wire.send(0x00);
			Wire.endTransmission();

			Wire.beginTransmission(address);
			//ODR=30Hz, 4 samples averaged per outpu
			Wire.send(0x00);
			Wire.send(0x34);
			Wire.endTransmission();
		}

		int read(void){
			int val=0;
			Wire.beginTransmission(address);
			Wire.send(0x03); //x=3, y=5, z=7
			Wire.endTransmission();
			Wire.requestFrom(address, 2);
			while (Wire.available() < 2);
			val = Wire.receive()<<8; //msb
			val |= Wire.receive(); //lsb
			return val;
		}

		int binRead(void){return (read > threshold ? 1 : 0);}
};

class IRSensor{
	int pin;
	int threshold;
	public:
		IRSensor(int inputPin, int wallThreshold){
			pin=inputPin;
			threshold=wallThreshold;
		}
		void init(void){pinMode(inputPin,INPUT);}
		int read(void){return analogRead(pin);}
		int binRead(void){( read() > threshold ? return 1 : return 0);}
};

class Motor{
	int direction, PWM, reversed;
	public:
		Motor(int dirPin, int PWMPin){
			dir=dirPin;
			PWM=PWMPin;
		}

		void init(){
			pinMode(dir,OUTPUT);
			pinMode(PWM,OUTPUT);
		}

		void setMotor(int speed, int direction){
			analogWrite(speed);
			if(reversed) direction= !direction;
			digitalWrite(dir, (direction ? HIGH : LOW));
		}

		void reverse(void){
			reversed = !reversed;
		}

		void stop(void){
			digitalWrite(PWM, LOW);
		}
};

class MarkerSystem{

};

class Drive{
	Gyroscope gyro;
	Motor leftMotor;
	Motor rightMotor;
	//
}