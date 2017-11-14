// Written by Nick Gammon
// February 2011


#include <SPI.h>
//#include <MotorWheel.h>
//#include <Omni4WD.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_MotorShield.h>

int pin = 13;
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 


/*irqISR(irq1,isr1);
MotorWheel wheel1(5,4,2,3,&irq1,1,&AFMS);

irqISR(irq2,isr2);
MotorWheel wheel2(6,7,4,5,&irq2,2,&AFMS);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,6,9,&irq3,3,&AFMS);

irqISR(irq4,isr4);
MotorWheel wheel4(10,11,7,8,&irq4,4,&AFMS);*/

// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
int minRange = 25;
int maxRange = 250;
 
// analog speeds from 0 (lowest) - 1023 (highest)
// 3 speeds used -- 0 (noSpeed), 350 (minSpeed), 850 (maxSpeed).
// use whatever speeds you want...too fast made it a pain in the ass to control
int minSpeed = 25;
int maxSpeed = 255;
int noSpeed = 0;
int pinValue = 255; // assigning incoming value from pin V4 to a variable for Motor Speed (0-255)
int pinValueAdjusted = pinValue ; 
int E1 = 3;     //M1 Speed Control
int E2 = 9;     //M2 Speed Control
int M1 = 8;    //M1 Direction Control
int M2 = 2;    //M1 Direction Control
int encoder0PinA = 3;
int encoder0PinB = 4;
 volatile byte pos;
 byte val1 = 0;
byte val2 = 0;
byte c = 0;
byte in1 = 0;
char buf [100];

volatile boolean process_it;
volatile boolean process;

//Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

 

  void clockwise()
 {

        motor1->setSpeed(127); //Turning Side slower than opposing side
        motor2->setSpeed(127); //Turning Side slower than opposing side
        motor3->setSpeed(127);
        motor4->setSpeed(127);
  
        motor1->run(FORWARD);
        motor2->run(FORWARD);
        motor3->run(FORWARD); 
        motor4->run(FORWARD);
  
 }
 void anticlockwise()
 {
        motor1->setSpeed(127); //Turning Side slower than opposing side
        motor2->setSpeed(127); //Turning Side slower than opposing side
        motor3->setSpeed(127);
        motor4->setSpeed(127);
  
        motor1->run(BACKWARD);
        motor2->run(BACKWARD);
        motor3->run(BACKWARD); 
        motor4->run(BACKWARD);
 }
void motorcontrol2(int x_position , int  y_position) {
  
  const int X_THRESHOLD_LOW = 88; //x: 128 - 20
  const int X_THRESHOLD_HIGH = 168; //x: 128 + 20   
  
  const int Y_THRESHOLD_LOW = 88; //y: 128 - 20
  const int Y_THRESHOLD_HIGH = 168; //y: 128 + 20
  
  //int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  //int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255
  
  int x_direction = 2;  //Variable for Direction of Joystick Movement: x= -1, 0, 1
  int y_direction = 2;  //Variable for Direction of Joystick Movement: y= -1, 0, 1
  int speed = 2*(121 - x_position);
  if(speed < 0)
  speed = speed * -1;
  speed= 150;
  Serial.print("x_position: ");
  Serial.print(x_position);
  Serial.print("  y_position: ");
  Serial.println(y_position);
  
  //Determine the direction of the Joystick Movement
  
  
  
  if (x_position > X_THRESHOLD_HIGH) {
  x_direction = 1;
  } else if (x_position <= X_THRESHOLD_LOW) {
  x_direction = -1;
  }
  if (y_position > Y_THRESHOLD_HIGH) {
  y_direction = 1;
  } else if (y_position <= Y_THRESHOLD_LOW) {
  y_direction = -1;
  }
  if(x_position > X_THRESHOLD_LOW && x_position < X_THRESHOLD_HIGH)
  x_direction = 0;
  if(y_position > Y_THRESHOLD_LOW && y_position < Y_THRESHOLD_HIGH)
  y_direction = 0;
  //if x and y are within the threshold their values then x_direction = 0 and y_direction = 0
  
  //Move the Rover (Rover will move in the direction of the slower wheels)
  //0,0(Stop); 0,1(Forward); 0,-1(Backward); 1,1(Right up diagonal); 1,0(Right); 1,-1(Right down diagonal);
  //-1,0(Left); -1,1(Left up diagonal); -1,-1(Left down diagonal)
  
  
  //**********BACKWARD DIAGONAL (SOFT) LEFT**********
  //x = -1 and y = -1 
  if (x_direction == -1) {
  if (y_direction == -1) {
  Serial.print("JOYSTICK: left-down DIRECTION: BACKWARD SOFT LEFT");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction); 
  
  motor1->setSpeed(speed); //Turning Side slower than opposing side
  motor2->setSpeed(speed); //Turning Side slower than opposing side
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(BACKWARD);
  motor2->run(RELEASE);
  motor3->run(FORWARD); 
  motor4->run(RELEASE);
  
  //**********HARD LEFT TURN**********
  //x = -1 and y = 0      
  } else if (y_direction == 0) {
  Serial.print("JOYSTICK: left DIRECTION: HARD LEFT ");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->setSpeed(speed);
  motor2->setSpeed(speed);
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(BACKWARD);
  motor2->run(BACKWARD);
  motor3->run(FORWARD);
  motor4->run(FORWARD);
  
  //FORWARD DIAGONAL (SOFT) LEFT
  //x = -1 and y = 1   
  } else {
  //y_direction == 1
  Serial.print("JOYSTICK left-up DIRECTION: FORWARD SOFT LEFT");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);  
  
  motor1->setSpeed(speed); //Turning Side slower than opposing side
  motor2->setSpeed(speed); //Turning Side slower than opposing side
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(RELEASE);
  motor2->run(BACKWARD);
  motor3->run(RELEASE);
  motor4->run(FORWARD);
  
  //**********BACKWARD**********
  //x = 0 and y = -1 
  } }else 
  if (x_direction == 0) {
  if (y_direction == -1) {
  Serial.print("JOYSTICK down DIRECTION BACKWARD");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->setSpeed(speed);
  motor2->setSpeed(speed);
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(BACKWARD);
  motor2->run(FORWARD);
  motor3->run(FORWARD); 
  motor4->run(BACKWARD);
  
  //**********STOP**********
  //x = 0 and y = 0 
  } else if (y_direction == 0) {
  Serial.print("JOYSTICK: centered DIRECTION: STOP");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->run(RELEASE);
  motor2->run(RELEASE);
  motor3->run(RELEASE);
  motor4->run(RELEASE);
  
  //**********FORWARD**********
  //x = 0 and y = 1 
  } else {
  //y_direction == 1
  Serial.print("JOYSTICK: up DIRECTION: FORWARD");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->setSpeed(speed);
  motor2->setSpeed(speed);
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(FORWARD);
  motor2->run(BACKWARD);
  motor3->run(BACKWARD);
  motor4->run(FORWARD);
  //**********BACKWARD DIAGONAL (SOFT) RIGHT**********
  //x = 1 and y = -1 
  } }else 
  if (x_direction == 1){
  if (y_direction == -1){ 
  Serial.print("JOYSTICK right-down DIRECTION: BACKWARD SOFT RIGHT");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->setSpeed(speed);
  motor2->setSpeed(speed);
  motor3->setSpeed(speed);//Turning Side slower than opposing side
  motor4->setSpeed(speed);//Turning Side slower than opposing side
  
  motor1->run(RELEASE);
  motor2->run(FORWARD);
  motor3->run(RELEASE); 
  motor4->run(BACKWARD);
  //**********HARD RIGHT TURN**********
  //x = 1 and y = 0 Right on x-axis 
  } 
  else if (y_direction == 0)
  { 
  Serial.print("JOYSITCK: right DIRECTION: HARD RIGHT");
  Serial.print("  x_direction  ");
  Serial.print(x_direction);
  Serial.print("  y_direction  ");
  Serial.println(y_direction);
  
  motor1->setSpeed(speed);
  motor2->setSpeed(speed);
  motor3->setSpeed(speed);
  motor4->setSpeed(speed);
  
  motor1->run(FORWARD);
  motor2->run(FORWARD);
  motor3->run(BACKWARD);
  motor4->run(BACKWARD); 
  
  //**********FORWARD DIAGONAL (SOFT) RIGHT**********
  //x = 1 and y = 1 
  } 
    else
    { 
      //y_direction == 1
      Serial.print("JOYSTICK: right-up DIRECTION: SOFT RIGHT"); 
      Serial.print("  x_direction  ");
      Serial.print(x_direction);
      Serial.print("  y_direction  ");
      Serial.println(y_direction);
      
      motor1->setSpeed(speed);
      motor2->setSpeed(speed);
      motor3->setSpeed(speed);//Turning Side slower than opposing side
      motor4->setSpeed(speed);//Turning Side slower than opposing side
      
      motor1->run(FORWARD);
      motor2->run(RELEASE);
      motor3->run(BACKWARD);
      motor4->run(RELEASE); 
    }
  }
  }
 /* void clockwise()
 {

       Omni.setCarRotateLeft(255);
  
 }
 void anticlockwise()
 {
       Omni.setCarRotateRight(255);
 }
void motorcontrol2(int x_position , int  y_position) {
  
  const int X_THRESHOLD_LOW = 88; //x: 128 - 20
  const int X_THRESHOLD_HIGH = 168; //x: 128 + 20   

  const int Y_THRESHOLD_LOW = 88; //y: 128 - 20
  const int Y_THRESHOLD_HIGH = 168; //y: 128 + 20
      
  //int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  //int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255

  int x_direction;  //Variable for Direction of Joystick Movement: x= -1, 0, 1
  int y_direction;  //Variable for Direction of Joystick Movement: y= -1, 0, 1
  int speed = 2*(121 - x_position);
  if(speed < 0)
  speed = speed * -1;
  Serial.print("x_position: ");
  Serial.print(x_position);
  Serial.print("  y_position: ");
  Serial.println(y_position);

//Determine the direction of the Joystick Movement

  x_direction = 0;
  y_direction = 0;

  if (x_position > X_THRESHOLD_HIGH) {
    x_direction = 1;
  } else if (x_position < X_THRESHOLD_LOW) {
    x_direction = -1;
  }
  if (y_position > Y_THRESHOLD_HIGH) {
    y_direction = 1;
  } else if (y_position < Y_THRESHOLD_LOW) {
    y_direction = -1;
  }
//if x and y are within the threshold their values then x_direction = 0 and y_direction = 0

//Move the Rover (Rover will move in the direction of the slower wheels)
//0,0(Stop); 0,1(Forward); 0,-1(Backward); 1,1(Right up diagonal); 1,0(Right); 1,-1(Right down diagonal);
//-1,0(Left); -1,1(Left up diagonal); -1,-1(Left down diagonal)

//**********BACKWARD DIAGONAL (SOFT) LEFT**********
//x = -1 and y = -1 
  if (x_direction == -1) 
    if (y_direction == -1) {
        Serial.print("JOYSTICK: left-down DIRECTION: BACKWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction); 
 
  Omni.setCarLowerLeft(speed);

//**********HARD LEFT TURN**********
//x = -1 and y = 0      
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: left DIRECTION: HARD LEFT ");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
Omni.setCarLeft(speed);


//FORWARD DIAGONAL (SOFT) LEFT
//x = -1 and y = 1   
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK left-up DIRECTION: FORWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);  
Omni.setCarUpperLeft(speed);
        
//**********BACKWARD**********
//x = 0 and y = -1 
      } else 
        if (x_direction == 0) 
        if (y_direction == -1) {
        Serial.print("JOYSTICK down DIRECTION BACKWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

 Omni.setCarBackoff(speed);

//**********STOP**********
//x = 0 and y = 0 
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: centered DIRECTION: STOP");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
        
 Omni.setCarStop(speed);   

//**********FORWARD**********
//x = 0 and y = 1 
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK: up DIRECTION: FORWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
Omni.setCarAdvance(speed);
 
//**********BACKWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = -1 
      } else 
        //x_direction == 1
        if (y_direction == -1){ 
        Serial.print("JOYSTICK right-down DIRECTION: BACKWARD SOFT RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
Omni.setCarLowerRight(speed);
 
//**********HARD RIGHT TURN**********
//x = 1 and y = 0 Right on x-axis 
      } else 
        if (y_direction == 0){ 
        Serial.print("JOYSITCK: right DIRECTION: HARD RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

Omni.setCarRight(speed);

//**********FORWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = 1 
      } else { 
        //y_direction == 1
        Serial.print("JOYSTICK: right-up DIRECTION: SOFT RIGHT"); 
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
        
 Omni.setCarUpperRight(speed);
      }
  }*/
 void stop1(void)                    //Stop
{
  digitalWrite(E1,LOW);  
   delay(2000);                //Pause for 2 sec to disallow rapidfire 
 
}   
void advance1(char a,char b)          //Move forward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
 
}  
void back_off1 (char a,char b)          //Move backward
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
 
}
void stop2(void)                    //Stop
{
  digitalWrite(E2,LOW);      
   delay(2000);                //Pause for 2 sec to disallow rapidfire
}   
void advance2(char a,char b)          //Move forward
{ 
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  
void back_off2 (char a,char b)          //Move backward
{
 
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
 // analogWrite (E2,b);    
 // digitalWrite(M2,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
 // analogWrite (E2,b);    
 // digitalWrite(M2,LOW);
}


void turn_L1 (char a,char b)             //Turn Left
{
  //analogWrite (E1,a);
  //digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}
void turn_R1 (char a,char b)             //Turn Right
{
  //analogWrite (E1,a);
  //digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}

int i  = 0;
void setup (void)
{
   
  Serial.begin (115200);   // debugging
 
  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;
  process = false;
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  //  Omni.PIDEnable(0.31,0.01,0,10);
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(250);
  motor2->setSpeed(250);
  motor3->setSpeed(250);
  motor4->setSpeed(250);  
  motor1->run(RELEASE);
  motor2->run(RELEASE);
  motor3->run(RELEASE);
  motor4->run(RELEASE);

   digitalWrite(E1,HIGH);   
  digitalWrite(E2,HIGH); 
  digitalWrite(M1,HIGH);   
  digitalWrite(M2,HIGH);    

/*  for(i=4;i<=7;i++)
  {
  pinMode(i, OUTPUT);  
 
  }*/
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);  
  pinMode(8, OUTPUT);  
  pinMode(9, OUTPUT);  

  
  //   pinMode (encoder0PinA,INPUT);
  // pinMode (encoder0PinB,INPUT);
  



  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup

ISR (SPI_STC_vect)
{
   c = SPDR;  // grab byte from SPI Data Register
  
   Serial.println(c);
  
   if(c< 243)
   {
    if(pos == 0)
    val1 = c;
    if(pos == 1)
    val2 = c;
    // example: newline means time to process buffer
    if (pos  == 1)
    process_it = true;
    //  else
    pos++;
   }
  else
  {
    pos =0;
    val1 = c;
    process_it= false;
     Serial.print (val1);
  }
 
} 
// main loop - wait for flag set in interrupt routine
void loop (void)
{
//digitalWrite(E1,HIGH);   
//digitalWrite(E2,HIGH); 
//digitalWrite(M1,HIGH);   
//digitalWrite(M2,HIGH);    
//if(val1<245)
{
 
  if (process_it)
  {
    stop1();
    stop2();

  
   if(c ==  243)
   {
      clockwise();
   }
   else if(c==244)
   {
    anticlockwise();
   }
   else
   {
    
    Serial.println ("x= ");
    Serial.print (val1);
    Serial.println ("y= ");
    Serial.println (val2);
  
    motorcontrol2((int)val1,(int)val2);
   }
    c = 0;
    
    pos = 0;
    process_it = false;
  }  // end of flag set

}
  if(val1 >= 245 && val1 <= 250)
  {

   
    switch(val1)
    {
    case 245://Move Forward
    advance1 (255,255);   //move forward in max speed
    break;
    case 246://Move Backward
    back_off1 (255,255);   //move back in max speed
    break;
    
    case 247:
    stop1();
    break;
    
    case 248://Move Forward
    advance2(255,255);   //move forward in max speed
    break;
    case 249://Move Backward
    back_off2 (255,255);   //move back in max speed
    break;
    
    case 250:
    stop2();
    break;
    
    }
  }
 
  
    
}  // end of loop
