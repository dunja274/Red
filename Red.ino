/*
Name:		Robot.ino
Created:	8/18/2015 10:31:36 PM
Author:	Damir
*/

#include <NewPing.h>
#include <Servo.h> //include Servo library

#define trigPin 13
#define echoPin 12
#define MAX_DISTANCE 400

const int dangerThresh = 20; //threshold for obstacles (in cm)
const int steer = dangerThresh + 20; //threshold for obstacles (in cm)
const int distSlow = 80;
bool left = false;
bool right = false;
String motorSet = "";
String steerSet = "";
int left90, right90, neutral; //distances on either side
Servo panMotor;
long duration; //time it takes to recieve PING))) signal
long distance;
int pos;

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);
// wired connections

#define HG7881_A_IA 8 // D8 --> Motor A Input A --> MOTOR A +
#define HG7881_A_IB 9 // D9 --> Motor A Input B --> MOTOR A -

#define HG7881_B_IA 10 // D10 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 11 // D11 --> Motor B Input B --> MOTOR B -


// functional connections

#define MOTOR_A_PWM HG7881_A_IA // Motor B PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor B Direction

#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 50  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 800 // brief delay for abrupt motor changes
#define MOVE_DELAY 800 // brief delay for abrupt motor changes
#define STEER_DELAY 800

void setup()
{
	Serial.begin(9600);
	panMotor.attach(6); //attach motors to proper pins
	panMotor.write(90); //set PING pan to center

	pinMode(MOTOR_B_DIR, OUTPUT);
	pinMode(MOTOR_B_PWM, OUTPUT);
	digitalWrite(MOTOR_B_DIR, LOW);
	digitalWrite(MOTOR_B_PWM, LOW);

	pinMode(MOTOR_A_DIR, OUTPUT);
	pinMode(MOTOR_A_PWM, OUTPUT);
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
	delay(5000);
	motorSet = "FORWARD";
        steerSet = "NEUTRAL";
	moveForward();
}

void loop()
{
	checkPath();
}

void checkPath()
{
 int curLeft = 0; int curRight = 0; int curFront = 0;
  distance = 0;
 Serial.println("checkPath");
  checkForward();
 /* panMotor.write(135);
   delay(100);
  for (pos = 135; pos >= 45; pos -= 45) {
    panMotor.write(pos);
    distance = readPing();
   delay(170);
*/
  distance = readPing();
   if (distance < dangerThresh) { 
  Serial.println("Danger");
     stopMove();
     checkCourse();
   }
   
 //  if (distance < steer) { checkCourse(); } 
   //if (distance < steer) { changePath(); } 
  

  
  
  
/*	int distanceFwd = ping();
	Serial.print(distanceFwd);
	if (distanceFwd<dangerThresh)//if path is blocked
	{
		/*Serial.println("dst < danger :");
		Serial.println(distanceFwd);
		Serial.println();
		stopMove();
		checkCourse();

	}
	else if (distanceFwd < steer)//if path is blocked
	{
		//Serial.println("dst < steer :");
		checkCourse();
	}
	checkForward();*/
}

void checkForward()
{
	Serial.println("Check forward");
        Serial.println(motorSet);
	if (motorSet == "FORWARD") {
		//Serial.println("move forward");
		moveForward();
	}

}     // turn it on going forward


void checkCourse()
{
	//steerNeutral();
	//Serial.println();
	Serial.println("CheckCourse");
	panMotor.write(10);
	delay(400);
	right90 = readPing(); //scan to the right
	delay(50);


	panMotor.write(180);
	delay(400);
	left90 = readPing(); //scan to the left
	delay(50);

	panMotor.write(90); //return to center
	delay(400);
	neutral = readPing();
	delay(50);

	compareDistance();
}

void compareDistance()
{
   if (neutral<10)
        {
          moveBackward();
          delay (MOVE_DELAY);
          stopMove();
          checkCourse();
        }
        	
  if (neutral < dangerThresh)
	{
		if (left90 < dangerThresh && right90 < dangerThresh)
			{
				moveBackward();
				delay(600);
				stopMove();
            	checkCourse();
				motorSet = "FORWARD";
			}
		if (left90 > right90)
			{
				steerRightBackward();
			}
			else
			{
				steerLeftBackward();
			}
	}
	if (left90 > right90) //if left is less obstructed 
		{
			moveForward();
			steerLeft();
			delay(STEER_DELAY);
			steerNeutral();
		}
		else
		{
			moveForward();
			steerRight();
			delay(STEER_DELAY);
			steerNeutral();
		}

	

	//left = false;
	//right = false;
}

 void changePath()
 {
   if (pos < 90) { steerLeftForward(); } 
   if (pos > 90) { steerRightForward(); }
   motorSet = "FORWARD";
 
 }
void moveForward()
{
        Serial.println("moveForward");
	// set the motor speed and direction
	digitalWrite(MOTOR_A_DIR, LOW); // direction = forward
	digitalWrite(MOTOR_A_PWM, HIGH); // PWM speed = slow
	motorSet = "FORWARD";
}

void moveBackward()
{
	 Serial.println("moveBackward");
        // backward drive on
	// set the motor speed and direction
	digitalWrite(MOTOR_A_DIR, HIGH); // direction = backward
	digitalWrite(MOTOR_A_PWM, LOW); // PWM speed = slow
	motorSet = "BACKWARD";
}

void stopMove()
{
   Serial.println("stopMove");
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
        if (motorSet=="FORWARD")
        {
        digitalWrite(MOTOR_A_DIR, HIGH); // direction = backward
	digitalWrite(MOTOR_A_PWM, LOW); // PWM speed = slow
        }
        else
        {
        digitalWrite(MOTOR_A_DIR, LOW); // direction = forward
	digitalWrite(MOTOR_A_PWM, HIGH); // PWM speed = slow
        }
        delay (400);
        digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
        motorSet = "STOP";
        Serial.println();
	Serial.println("Motor stop");
}

void moveSlowForward()
{
	// forward drive on
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
	delay(DIR_DELAY);
	// set the motor speed and direction
	digitalWrite(MOTOR_A_DIR, LOW); // direction = forward
	digitalWrite(MOTOR_A_PWM, HIGH); // PWM speed = slow
	delay(MOVE_DELAY);
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
}

void moveSlowBackward()
{
	// forward drive on
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
	delay(DIR_DELAY);
	// set the motor speed and direction
	digitalWrite(MOTOR_A_DIR, HIGH); // direction = backward
	digitalWrite(MOTOR_A_PWM, LOW); // PWM speed = slow
	delay(DIR_DELAY);
	digitalWrite(MOTOR_A_DIR, LOW);
	digitalWrite(MOTOR_A_PWM, LOW);
}

void steerNeutral()
{
   Serial.println("steerNeutral");
	digitalWrite(MOTOR_B_DIR, LOW);
	digitalWrite(MOTOR_B_PWM, LOW);
        steerSet = "NEUTRAL";
}

void steerLeft()
{
   Serial.println("steerLeft");
	digitalWrite(MOTOR_B_DIR, LOW);
	digitalWrite(MOTOR_B_PWM, HIGH);
        steerSet = "LEFT";
}

void steerLeftForward()
{
   Serial.println("steerLeftForward");
	steerLeft();
	moveForward();
	delay(MOVE_DELAY);
	steerNeutral();
	motorSet = "FORWARD";
}

void steerLeftBackward()
{
   Serial.println("steerLeftBackward");
	steerRight();
	moveBackward();
	delay(MOVE_DELAY);
	steerNeutral();
	motorSet = "FORWARD";
}

void steerRight()
{
   Serial.println("steerRight");
	digitalWrite(MOTOR_B_DIR, HIGH);
	digitalWrite(MOTOR_B_PWM, LOW);
        steerSet = "RIGHT";
}

void steerRightForward()
{
   Serial.println("steerRightForward");
	moveForward();
	steerRight();
	delay(MOVE_DELAY);
        steerNeutral();
}

void steerRightBackward()
{
   Serial.println("steerRightBackward");
	steerLeft();
	moveBackward();
	delay(DIR_DELAY);
        steerNeutral();
	motorSet = "FORWARD";
}

int readPing()
{
        Serial.println();
	Serial.println("Ping!");
	int cm = 0;
	while (cm < 2) {
		int uS = sonar.ping();
		cm = uS / US_ROUNDTRIP_CM;
	}
        Serial.println(cm);
	return cm;
}


