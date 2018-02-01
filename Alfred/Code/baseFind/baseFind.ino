/*
 * SINGLE LINE FOLLOWER WITH ULTRASONIC SENSOR
 * 
 * Uses 5 line sensors.
 * Best used on a white surface ground with black line as tracks.
 * Follows a single track line.
 * Uses an ultrasonic sensor to STOP the Robot when there's an object in front.
 *
 *
 */

#include "sensorsDRV.h" // These files include the mappings
#include <avr/wdt.h>
#include "TBMotor.h" // These files include the mappings
#include <Servo.h>

// Set the motor location on arduino
OseppTBMotor Motor1(12, 11);
OseppTBMotor Motor2(8, 3);

// Variables for the motors
#define leftMotor Motor1
#define rightMotor Motor2

// Set ultrsonic sensor location on arduino
Ultrasonic ults(4,2);

// Set servo location on arduino and initialize position
const int servoPin = 9;
const int servoBais = 180;
Servo sv;

// Set initial motor speed
int leftSpeed = 0;
int rightSpeed = 0;

// emergency button
int stopped = 0;

// home base status
int homeb = 0;


// floor starting colour (dark == 1) (all lights off)
int ondark = 1;

// if it successfully followed line for a bit
int followed = 0;


//  reached white after following if ==1
int rwaf = 0;

//  on dark after white if ==1
int onDarkAfterWhite = 0;


//  give motor delay if ==1
int giveDelay = 0;


// Running some setup stuff
void setup()
{
  for (int i = A0; i < A5; i++)pinMode(i, INPUT_PULLUP);
  sv.attach(servoPin);
  sv.write(servoBais);


Serial.begin(9600); 

  //Setup a watchdog
  //When the battery voltage is insufficient / program unexpected
  //Watchdog reset chip
  wdt_enable(WDTO_4S);

}

// Array for lineTracker status
bool lineTracker[5];

void loop()
{
  const float threshold = 10;



  //If in 4 seconds,The program is not coming back here.
  //Chip will reset
  wdt_reset();
  
  sv.write(servoBais);
  
  for (int i = 0; i < 5; i++)lineTracker[i] = !digitalRead(i + A0); //delete "!" to follow a white line.


  if (ults.Detect() < threshold)
  {
    leftSpeed = 0;
    rightSpeed = 0;
    SetMotor();
    return;//to the beginning of the loop function.
  }

  // Use this for better white on black
  //int left = lineTracker[0] * 3 + lineTracker[1] * 2 + lineTracker[2]  - lineTracker[3] * 2 - lineTracker[4] * 3;
 // int right = -lineTracker[0] * 3 - lineTracker[1] * 2 + lineTracker[2]  + lineTracker[3] * 2 + lineTracker[4] * 3;


  int right = lineTracker[0] * 3 + lineTracker[1] * 2 + lineTracker[2]  - lineTracker[3] * 2 - lineTracker[4] * 3;
  int left = -lineTracker[0] * 3 - lineTracker[1] * 2 + lineTracker[2]  + lineTracker[3] * 2 + lineTracker[4] * 3;





  //Line tracker
  if (left > right)
  {
    leftSpeed = 90;
    rightSpeed = -90;
  } else if (left < right)
  {
    leftSpeed = -90;
    rightSpeed = 90;
  } else {
    leftSpeed = rightSpeed = 255;
  
  }

//robot will ignore linetrack drive straight till its on white base
if (ondark == 1){
  leftSpeed = rightSpeed = 255;
}

//if followed
if (followed == 1){
     //if reaches all white
  if (lineTracker[0] == 0 && lineTracker[1] == 0 && lineTracker[2] == 0 && lineTracker[3] == 0 && lineTracker[4] == 0){
rwaf=1;
//turn
    leftSpeed = 255;
    rightSpeed = -255;

   }else{
    rwaf=0;

   }

}

  
//emergency stop check
  if (digitalRead(6) == 0){
stopped=1;
   }

   //if reaches all white
  if (lineTracker[0] == 0 && lineTracker[1] == 0 && lineTracker[2] == 0 && lineTracker[3] == 0 && lineTracker[4] == 0){
ondark=0;
//slowdown motors
  //if (leftSpeed > 60)leftSpeed = 60; else if (leftSpeed < -60)leftSpeed = -60;
 // if (rightSpeed > 60)rightSpeed = 60; else if (rightSpeed < -60)rightSpeed = -60;

   }
       giveDelay =0;
   //if reaches dark again (after being on white) [this means its off course]
if (ondark==0){
    if (lineTracker[0] == 1 && lineTracker[1] == 1 && lineTracker[2] == 1 && lineTracker[3] == 1 && lineTracker[4] == 1){
      onDarkAfterWhite=1;
    leftSpeed = 255;
    rightSpeed = -255;
    giveDelay =1;
    }
}



//homebase check
  if (lineTracker[0] == 1 && lineTracker[1] == 0 && lineTracker[2] == 1 && lineTracker[3] == 0 && lineTracker[4] == 1){
homeb=1;
   }


   //followed line check
  if (lineTracker[0] == 0 && lineTracker[1] == 0 && lineTracker[2] == 1 && lineTracker[3] == 0 && lineTracker[4] == 0){
followed=1;
   }
  
  
  SetMotor();
//test();

  
}


// This sets a maximum speed for the motor 
void SetMotor()
{
  if (leftSpeed > 100)leftSpeed = 100; else if (leftSpeed < -100)leftSpeed = -100;
  if (rightSpeed > 100)rightSpeed = 100; else if (rightSpeed < -100)rightSpeed = -100;

if (stopped==1){
  leftSpeed = 0;
  rightSpeed = 0;
}

if (homeb==1){
  leftSpeed = 0;
  rightSpeed = 0;
}

  //To switch motor directions (if wired backwards)
  leftMotor.SetSpeed(leftSpeed);
  rightMotor.SetSpeed(rightSpeed);

     if (giveDelay==1){
    delay(2000);   

}

delay(100);   //to stop hump loops // turn this OFF if using test()
 
}

//for outputing code
void test()
{

  Serial.println("Testing:");


//   Serial.print("Linetracker 0 status: ");    
//Serial.println(lineTracker[0] );
//
//   Serial.print("Linetracker 1 status: ");    
//Serial.println(lineTracker[1] );
//
//   Serial.print("Linetracker 2 status: ");    
//Serial.println(lineTracker[2] );
//
//  Serial.print("Linetracker 3 status: ");    
//Serial.println(lineTracker[3] );
//   Serial.print("Linetracker 4 status: ");    
//Serial.println(lineTracker[4] );


    Serial.print(" Stop Button status:  ");    
Serial.println(digitalRead(6));

    Serial.print(" Stopped if 1==");    
Serial.println(stopped);

    Serial.print(" Home if 1==");    
Serial.println(homeb);

    Serial.print(" hasnt reached white if 1==");    
Serial.println(ondark);


    Serial.print(" followed line if 1==");    
Serial.println(followed);


    Serial.print(" on white after followed line if 1==");    
Serial.println(rwaf);

    Serial.print(" on Dark After White if 1==");    
Serial.println(onDarkAfterWhite);







// Serial.print(" /n Full digital/analog read /n");   
//
//
//    Serial.print(" - Pin 0  ");    
//Serial.print(digitalRead(0));
//
//    Serial.print(" - Pin 1  ");    
//Serial.print(analogRead(1));
//
    Serial.print(" - Pin 2  ");    
Serial.print(analogRead(2));
//
//    Serial.print(" - Pin 3  ");    
//Serial.print(analogRead(3));
//
//
    Serial.print(" - Pin 4  ");    
Serial.print(analogRead(4));
//
//    Serial.print(" - Pin 5  ");    
//Serial.print(analogRead(5));
//
//
//
//
//    Serial.print(" - Pin 7  ");    
//Serial.print(analogRead(7));
//
//
//    Serial.print(" - Pin 8  ");    
//Serial.print(analogRead(8));
//
//
//    Serial.print(" - Pin 9  ");    
//Serial.print(analogRead(9));
//
//
//    Serial.print(" - Pin 10  ");    
//Serial.print(analogRead(10));




}



