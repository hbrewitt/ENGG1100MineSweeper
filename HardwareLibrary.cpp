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
	byte mLeftSaved[2]={0,0};
	byte mRightSaved[2]={0,0};
	int targetHeading;
	const int defaultSpeed=200;

	int directionDiff(int target, int current){
		//Stolen from Xander's RobotC drivers for the HiTechnic compass. He calls it "black voodoo magic"
		//I did a deconstruction back in grade 9, but the versioning from then is disgusting and I don't want to go through it
		int _tmpHeading = target - current + 180;
		return (_tmpHeading >= 0 ? _tmpHeading % 360 : 359 - (-1 - _tmpHeading)%360) - 180;
	}

	public:
		Drive(int gyroAddr, int mLeftDir, int mLeftPwm, int mRightDir, int mRightPwm){
			leftMotor=Motor(mLeftDir,mLeftPwm);
			rightMotor=Motor(mRightDir,mRightPwm);
			gyro=Gyro(gyroAddr);
		}

		void init(void){
			leftMotor.init();
			rightMotor.init();
			gyro.init();
			delay(500);
			targetHeading=gyro.read();
		}

		void forward(void){
			int currHeading=gyro.read();
			int dDiff=directionDiff(targetHeading,currHeading);
			if( !(dDiff > 2 || dDiff < -2) ){
				mLeftSaved[0]+=defaultSpeed;
				mLeftSaved[0]/=2;
				mRightSaved[0]+=defaultSpeed;
				mRightSaved[0]/=2;
			}else{
				if(dDiff > 2){
					mLeftSaved[0]+=2;
					mRightSaved[0]-=2;
				} else { //dDiff < -2
					mLeftSaved[0]-=2;
					mRightSaved[0]+=2;
				}
			}
			mLeftSaved[1]=0;
			mRightSaved[1]=0;
			leftMotor.setMotor(mLeftSaved[0], 0);
			rightMotor.setMotor(mRightSaved[0], 0);
		}

		void turn(int turnAngle){
			targetHeading+=turnAngle;
			
			mLeftSaved[0]=defaultSpeed;
			mRightSaved[0]=defaultSpeed
			if (turnAngle > 0){
				mLeftSaved[1]=0;
				mRightSaved[1]=1;
			}else{
				mLeftSaved[1]=1;
				mRightSaved[1]=0;
			}
			leftMotor.setMotor(mLeftSaved[0], mLeftSaved[1]);
			rightMotor.setMotor(mRightSaved[0], mRightSaved[1]);
			while(dDiff > 2 || dDiff < -2){
				int currHeading=gyro.read();
				int dDiff=directionDiff(targetHeading,currHeading);
			}
			stop();
		}

		void stop(void){
			mLeftSaved[0]=0;
			mRightSaved[0]=0;
			leftMotor.stop();
			rightMotor.stop()
		}
}