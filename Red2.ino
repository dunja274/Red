#include <Servo.h>
#include <NewPing.h>

#define MAX_DISTANCE 400

NewPing sonar (12, 11, MAX_DISTANCE);
NewPing sonarRight (6, 5, MAX_DISTANCE);
NewPing sonarLeft (4, 3, MAX_DISTANCE);

int DangerTresh = 15;

#define motorDriver_A1 7
#define motorDriver_A2 8
#define motorDriver_B3 9
#define motorDriver_B4 10

#define motor_A_dir motorDriver_A1
#define motor_A_speed motorDriver_A2
#define motor_B_dir motorDriver_B3
#define motor_B_speed motorDriver_B4

Servo myservo;
int servoPos=0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(motor_A_dir, OUTPUT );
  pinMode(motor_A_speed, OUTPUT );
  pinMode(motor_B_dir, OUTPUT );
  pinMode(motor_B_speed, OUTPUT );
  digitalWrite(motor_A_dir, LOW);
  digitalWrite(motor_A_speed, LOW);
  digitalWrite(motor_B_dir, LOW);
  digitalWrite(motor_B_speed, LOW);

  myservo.attach(4);

  delay (2000);
  motorStart();
  
}

void loop() {
  // put your main code here, to run repeatedly:

 Serial.print("Dis=");
 Serial.print(readPing());
 Serial.println("cm");
 if(readPing() < DangerTresh)
    { 
      motorStop();
    }
   else 
   {
     if(!motorStatus){motorStart();} 
   }
}

void motorStart()
{
  digitalWrite(motor_A_dir, LOW);
  digitalWrite(motor_A_speed, HIGH);
  digitalWrite(motor_B_dir, LOW);
  digitalWrite(motor_B_speed, HIGH); 
  motorStatus=true;
}

void motorStop()
{
  digitalWrite(motor_A_dir, LOW);
  digitalWrite(motor_A_speed, LOW);
  digitalWrite(motor_B_dir, LOW);
  digitalWrite(motor_B_speed, LOW);  
  motorStatus=false;
}

void checkPath()
{
    myservo.write(90);              
    delay(400);
    int right=readPingRight();   
    myservo.write(270);              
    delay(400);   
    int left=readPingLeft();    

    if(left<DangerTresh && right<DangerTresh)
    {
      turnRound();  
    }
    else if(left<right)
    {
      right();  
    }
    else
    {
      left();  
    }

}

void left()
{
  
}

void right()
{
  
}

void turnRound()
{
  
}

int readPing() {
  int cm = 0;
  while (cm < 2) {
    int uS = sonar.ping();
    cm = uS/US_ROUNDTRIP_CM;
  }
  return cm;
}

int readPingRight() {
  int cm = 0;
  while (cm < 2) {
    int uS = sonarRight.ping();
    cm = uS/US_ROUNDTRIP_CM;
  }
  return cm;
}

int readPingLeft() {
  int cm = 0;
  while (cm < 2) {
    int uS = sonarLeft.ping();
    cm = uS/US_ROUNDTRIP_CM;
  }
  return cm;
}
