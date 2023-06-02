#include <TimerOne.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//arduino nano
 
const int Data=7;
const int Clk=8;
const int Latch=6;
 
const int SEG0=5;
const int SEG1=4;
const int SEG2=3;
const int SEG3=2;

Adafruit_MPU6050 mpu;
sensors_event_t a1, g1, temp1;
sensors_event_t a2, g2, temp2;

int sens=1;   //sensitivity (the corresponding LED turns on when the difference between 2 consecutive readings is greater than this number)

int r=12;
int g=11;   //LED pins
int b=10;

//x=red
//y=green
//z=blue


int cc=0;
char Value[4];
 
const char SegData[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 

void setup() {                
  Serial.begin(115200);
  pinMode(Data, OUTPUT);
  pinMode(Clk, OUTPUT);
  pinMode(Latch, OUTPUT);  
  pinMode(SEG0, OUTPUT);  
  pinMode(SEG1, OUTPUT);  
  pinMode(SEG2, OUTPUT);
  pinMode(SEG3, OUTPUT);  
   cc=0;
   Timer1.initialize(5000);
   Timer1.attachInterrupt( timerIsr ); 
   
   if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
  mpu.getEvent(&a1, &g1, &temp1);
  pinMode(r,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(b,OUTPUT);
}
void loop() {
  mpu.getEvent(&a2, &g2, &temp2);
 if(abs(a1.acceleration.x-a2.acceleration.x)>sens)
  digitalWrite(r,HIGH);
 else
  digitalWrite(r,LOW);
 if(abs(a1.acceleration.y-a2.acceleration.y)>sens)
  digitalWrite(g,HIGH);
 else
  digitalWrite(g,LOW);
  if(abs(a1.acceleration.z-a2.acceleration.z)>sens)
  digitalWrite(b,HIGH);
 else
  digitalWrite(b,LOW);
a1=a2;
g1=g2;
temp1=temp2;
  char displayValue[4];
  sprintf(displayValue,"%04d", (int) (temp2.temperature * 100));  
  Serial.println(displayValue);  
  
  Value[0]=displayValue[0] & 0x0F;  
  Value[1]=displayValue[1] & 0x0F;   
  Value[2]=displayValue[2] & 0x0F;
  Value[3]=displayValue[3] & 0x0F;  
  //delay(20);
  delay(1000);
}

void DisplayDigit(char d)
{
  int i;
  
 for(i=0;i<8;i++)  
 {
  if((d & 0x80)==0x80)
  {
    digitalWrite(Data,HIGH);
  }
  else
  {
    digitalWrite(Data,LOW);
  }
  d=d<<1;
 
        digitalWrite(Clk,LOW);        
        digitalWrite(Clk,HIGH);
 } 
  digitalWrite(Latch,LOW);
  digitalWrite(Latch,HIGH);      
}

void timerIsr()
{
  cc++;
  if(cc==5)
  {cc=1;}
  Scanner();
  TCNT0=0xCC;
}

void Scanner()
{  
  switch (cc)  
  {
    case 1:
      digitalWrite(SEG3,HIGH);      
      DisplayDigit(SegData[Value[0]]);
      digitalWrite(SEG0,LOW);
    break;
    case 2:
      digitalWrite(SEG0,HIGH);
      DisplayDigit(SegData[Value[1]] | 0x80); //0x80 to turn on decimal point
      digitalWrite(SEG1,LOW);
    break;
    case 3:
      digitalWrite(SEG1,HIGH);
      DisplayDigit(SegData[Value[2]]);
      digitalWrite(SEG2,LOW);
    break;
    case 4:
      digitalWrite(SEG2,HIGH);    
      DisplayDigit(SegData[Value[3]]);
      digitalWrite(SEG3,LOW);      
    break;    
  }
}
