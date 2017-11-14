// Written by Nick Gammon
// February 2011


#include <SPI.h>
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>
//#include <BlynkSimpleStream.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c1278cf46a864a508bcb0ad73b772533";
//char auth[]="8009700fc4d846f087e8cf3b14416710";
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN0  250 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX0  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN4  1 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX4  450 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN7  10 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX7  4096 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN8  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX8  4000 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN10  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX10  450 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN12  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX12  900 // this is the 'maximum' pulse length count (out of 4096)
float SERVOFREQ = 50;
float pulseconstant;

int angle;                                                 // determines the direction/angle (0°-360°) that the robot will walk in
int rotate;                                                // rotate mode: -1 = anticlockwise, +1 = clockwise, 0 = no rotation
int Speed;                                                 // walking speed: -15 to +15
int Stride;
// our servo # counter
uint8_t servonum = 0;
uint16_t pulselen = SERVOMIN0;
double prevvalue0 = 400;
double prevvalue4 = 400;
double prevvalue7 = 400;
double prevvalue8 = 400;
double prevvalue10 = 400;
double prevvalue12 = 400;
double inc = 0.25;
int delay1 = 100;
int addr = 0;
bool initial = true;
bool Connected2Blynk = false;
BLEPeripheral blePeripheral;
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return i;
}


double testInt[12] = { -12.5, -10.00, -5.7, 0, 2.45, 2.90, 3.10, 4 , 5.6, 7.9, 5.5, 4};
byte noElem = 12;
unsigned int baseAddr = 0;
unsigned int n = 0;
void setup (void)
{
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  delay(1000);

  //  // ensure SS stays high for now
  blePeripheral.setLocalName("Blynk Hello");
  blePeripheral.setDeviceName("Blynk Hello");
  blePeripheral.setAppearance(384);
  Blynk.begin(blePeripheral, auth);
  blePeripheral.begin();
 // Serial.begin(9600);
 //  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
  //   Blynk.begin(Serial, auth);
 pwm.begin();

  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  //  digitalWrite(SS, HIGH);
  SPI.begin ();
  
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);


}  // end of setup

bool firsttime  = true;
void loop (void)
{
 
  Blynk.run();
  blePeripheral.poll();

  
 if(firsttime)
 {
   digitalWrite(SS, LOW);    // SS is pin 10
  SPI.transfer (247);
 
  // disable Slave Select
  digitalWrite(SS, HIGH);

   delay(50);
   digitalWrite(SS, LOW);    // SS is pin 10
  SPI.transfer (250);
  // disable Slave Select
  digitalWrite(SS, HIGH);
  firsttime = false;
 }

}  // end of loop
BLYNK_WRITE(V20)
{
  int x1 = EEPROM.read(0);
  int x2 = EEPROM.read(1);
  
  int value1 = x1*256 + x2;

 
  Blynk.virtualWrite(V0, value1);
    x1 = EEPROM.read(2);
   x2 = EEPROM.read(3);
  
   value1 = x1*256 + x2;
  Blynk.virtualWrite(V4, value1);
    x1 = EEPROM.read(4);
   x2 = EEPROM.read(5);
  
   value1 = x1*256 + x2;
  Blynk.virtualWrite(V7, value1);
    x1 = EEPROM.read(6);
   x2 = EEPROM.read(7);
  
   value1 = x1*256 + x2;
  Blynk.virtualWrite(V8, value1);
    x1 = EEPROM.read(8);
   x2 = EEPROM.read(9);
  
   value1 = x1*256 + x2;
  Blynk.virtualWrite(V10, value1);
    x1 = EEPROM.read(10);
   x2 = EEPROM.read(11);
  
   value1 = x1*256 + x2;
  Blynk.virtualWrite(V12, value1);
}
BLYNK_WRITE(V21)
{
  int value1 = (int)prevvalue0;
  int a = value1/256;
  int b = value1%256;
  
  EEPROM.write(0, a);
  EEPROM.write(1, b);
   value1 = (int)prevvalue4;
   a = value1/256;
   b = value1%256;
   EEPROM.write(2, a);
  EEPROM.write(3, b);
   value1 = (int)prevvalue7;
   a = value1/256;
   b = value1%256;
   EEPROM.write(4, a);
  EEPROM.write(5, b);
   value1 = (int)prevvalue8;
   a = value1/256;
   b = value1%256;
   EEPROM.write(6, a);
  EEPROM.write(7, b);
   value1 = (int)prevvalue10;
   a = value1/256;
   b = value1%256;
   EEPROM.write(8, a);
  EEPROM.write(9, b);
   value1 = (int)prevvalue12;
   a = value1/256;
   b = value1%256;
     EEPROM.write(10, a);
  EEPROM.write(11, b);
}
//joystick
BLYNK_WRITE(V1)
{
  int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255

   Serial.println(x_position);
   Serial.println(y_position);

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (x_position);
  SPI.transfer (y_position);
 
  // disable Slave Select
  digitalWrite(SS, HIGH);

  

}
//actuators
BLYNK_WRITE(V3)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (245);
  
  digitalWrite(SS, HIGH);

 
}
//actuators
BLYNK_WRITE(V5)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  
  SPI.transfer (246);
  
  // disable Slave Select
  digitalWrite(SS, HIGH);

 
}
//actuators
BLYNK_WRITE(V2)
{

  int y_position = param.asInt();
  
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  
  SPI.transfer (247);

 
  // disable Slave Select
  digitalWrite(SS, HIGH);


}
BLYNK_WRITE(V15)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (248);
 

  // disable Slave Select
  digitalWrite(SS, HIGH);

 
}
//actuators
BLYNK_WRITE(V16)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (249);
 
  // disable Slave Select
  digitalWrite(SS, HIGH);

  
}
//actuators
BLYNK_WRITE(V17)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10


  SPI.transfer (250);

 
  // disable Slave Select
  digitalWrite(SS, HIGH);

  
}
BLYNK_WRITE(V18)
{

  int y_position = param.asInt();
 
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  SPI.transfer (7);

  // disable Slave Select
  digitalWrite(SS, HIGH);

}
BLYNK_WRITE(V19)
{
  int y_position = param.asInt();
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI.transfer (8);

  // disable Slave Select
  digitalWrite(SS, HIGH);
}
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
 
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);

}

//servomotors
BLYNK_WRITE(V0)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
  delay (delay1);
 
  pulselen = param.asInt();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);

  if(prevvalue0 <= pinValue)
  {
  for( double pinValue1 = prevvalue0;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(0, pinValue1); 
  }
  else if(prevvalue0 >= pinValue)
  {
  for( double pinValue1 = prevvalue0;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(0, pinValue1); 
  }
  prevvalue0 = pinValue;
 
  delay (delay1);
}

//servomotors
BLYNK_WRITE(V4)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
 
  delay (delay1);  // 1 seconds delay
  pulselen = param.asInt();
  Serial.print("V2 Slider value is: ");
  Serial.println(pinValue);

   if(prevvalue4 <= pinValue)
  {
  for( double pinValue1 = prevvalue4;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(4, pinValue1); 
  }
  else if(prevvalue4 >= pinValue)
  {
  for( double pinValue1 = prevvalue4;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(4, pinValue1); 
  }
  prevvalue4 = pinValue;
  
  delay (delay1);  // 1 seconds delay

}
//servomotors
BLYNK_WRITE(V7)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
  delay (delay1);  // 1 seconds delay
 
  pulselen = param.asInt();
  Serial.print("V3 Slider value is: ");
  Serial.println(pinValue);

   if(prevvalue7 <= pinValue)
  {
  for( double pinValue1 = prevvalue7;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(7, pinValue1); 
  }
  else if(prevvalue7 >= pinValue)
  {
  for( double pinValue1 = prevvalue7;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(7, pinValue1); 
  }
  prevvalue7 = pinValue;
 
  delay (delay1);  // 1 seconds delay

}
//servomotors
BLYNK_WRITE(V8)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
  delay (delay1);  // 1 seconds delay
 
  pulselen = param.asInt();
  Serial.print("V4 Slider value is: ");
  Serial.println(pinValue);

     if(prevvalue8 <= pinValue)
  {
  for( double pinValue1 = prevvalue8;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(8, pinValue1); 
  }
  else if(prevvalue8 >= pinValue)
  {
  for( double pinValue1 = prevvalue8;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(8, pinValue1); 
  }
  prevvalue8 = pinValue;
 int value1 = (int)prevvalue8;
 
  delay (delay1);  // 1 seconds delay

}
//servomotors
BLYNK_WRITE(V10)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
  delay (delay1);  // 1 seconds delay
 
  pulselen = param.asInt();
  Serial.print("V5 Slider value is: ");
  Serial.println(pinValue);
  
    if(prevvalue10 <= pinValue)
  {
  for( double pinValue1 = prevvalue10;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(10, pinValue1); 
  }
  else if(prevvalue10 >= pinValue)
  {
  for( double pinValue1 = prevvalue10;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(10, pinValue1); 
  }
  prevvalue10 = pinValue;
  int value1 = (int)prevvalue10;
 
  delay (delay1);  // 1 seconds delay

}
//servomotors
BLYNK_WRITE(V12)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
  delay (delay1);  // 1 seconds delay
 
  pulselen = param.asInt();
  Serial.print("V6 Slider value is: ");
  Serial.println(pinValue);
  //setServoPulse(12, pulselen);
    if(prevvalue12 <= pinValue)
  {
  for( double pinValue1 = prevvalue12;pinValue1<pinValue;pinValue1+=inc)
  setServoPulse(12, pinValue1); 
  }
  else if(prevvalue12 >= pinValue)
  {
  for( double pinValue1 = prevvalue12;pinValue1>pinValue;pinValue1-=inc)
  setServoPulse(12, pinValue1); 
  }
  prevvalue12 = pinValue;
  int value1 = (int)prevvalue12;
 
  //pwm.setPWM(12, 0, pulselen);
  delay (delay1);  // 1 seconds delay
}
//for rotation clockwise
BLYNK_WRITE(V13)
{
  int y_position = param.asInt();
  
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (243);
  
  // disable Slave Select
  digitalWrite(SS, HIGH);

}
//for rotation anticlockwise
BLYNK_WRITE(V14)
{
  int y_position = param.asInt();

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

 
  SPI.transfer (244);
  
  // disable Slave Select
  digitalWrite(SS, HIGH);

}

