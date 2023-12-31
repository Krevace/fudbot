#include <HCSR04.h> //HCSR04 by Martin Sosic

#define MOT_A1_PIN 5
#define MOT_A2_PIN 6
#define MOT_B1_PIN 9
#define MOT_B2_PIN 10
#define LEFT_IR_PIN 2
#define CENTER_IR_PIN 3
#define RIGHT_IR_PIN 4
#define OUTPUT_IR_PIN 11

UltraSonicDistanceSensor distanceSensor(7, 8);  

#define MIN_DISTANCE 30
#define TURN_SPEED 200
#define FORWARD_SPEED 180

enum Command
{
  LEFT, RIGHT, FORWARD
};

Command command = FORWARD;

void setup(void)
{
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);

  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(CENTER_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(OUTPUT_IR_PIN, OUTPUT);

  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);
  digitalWrite(OUTPUT_IR_PIN, HIGH);
}

void setMotorPWM(int pwm, int in1_pin, int in2_pin)
{
  if (pwm < 0)
  {  
    analogWrite(in1_pin, -pwm);
    digitalWrite(in2_pin, LOW);
  }
  else
  {
    digitalWrite(in1_pin, LOW);
    analogWrite(in2_pin, pwm);
  }
}

void setSpeeds(int pwm_A, int pwm_B)
{
  //-255 to 255
  setMotorPWM(pwm_A, MOT_A1_PIN, MOT_A2_PIN);
  setMotorPWM(pwm_B, MOT_B1_PIN, MOT_B2_PIN);
}

void slightRight()
{
  setSpeeds(TURN_SPEED, TURN_SPEED - 30);
  command = RIGHT;
}

void slightLeft()
{
  //Adjusted for behavior of motors
  setSpeeds(0, TURN_SPEED);
  command = LEFT;
}

void forward()
{
  setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  command = FORWARD;
}

void loop(void)
{
  double distance = distanceSensor.measureDistanceCm();
  bool left = digitalRead(LEFT_IR_PIN);
  bool right = digitalRead(RIGHT_IR_PIN);
  bool center = digitalRead(CENTER_IR_PIN);
 
  if (distance < MIN_DISTANCE && distance > 0)
  {
    setSpeeds(0, 0);
  }
  else
  {
    if (right)
    {
      slightRight();
    }
    else if (left)
    {
      slightLeft();
    }
    else if (!center)
    {
      switch (command)
      {
        case LEFT:
          slightLeft();
          break;
        case RIGHT:
          slightRight();
          break;
        case FORWARD:
          forward();
          break;
      }
    }
    else
    {
      forward();
    }
  }
}
