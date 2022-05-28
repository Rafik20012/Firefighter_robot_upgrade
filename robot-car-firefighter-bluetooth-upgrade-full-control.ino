#include <Servo.h>

Servo servo;

//Motor A
const int motorPin1  = 5; 
const int motorPin2  = 6; 
//Motor B
const int motorPin3  = 9; 
const int motorPin4  = 10;  
//PUMP
const int PUMP = 7;

int vSpeed;
int state;

int FL = 13;
int BL = 12;

void setup() 
{
   Serial.begin(9600);
   pinMode(PUMP,OUTPUT);
   pinMode(motorPin1, OUTPUT);
   pinMode(motorPin2, OUTPUT);
   pinMode(motorPin3, OUTPUT);
   pinMode(motorPin4, OUTPUT);     
   pinMode(FL,OUTPUT);
   pinMode(BL,OUTPUT);

    servo.attach(11);   
    servo.write(0);
    delay(2000);
    servo.write(35);
    delay(2000);      
    servo.write(65); 
    delay(2000);
    servo.write(35);
    delay(2000);
    
    digitalWrite(PUMP, HIGH);
    delay(2000);
    digitalWrite(PUMP, LOW);   
}

void loop()
{ 
if(Serial.available() >0)
  {
    state = Serial.read();
  }
   if (state == '0'){
      vSpeed=0; }
    else if (state == '1'){
      vSpeed=25;}
    else if (state == '2'){
      vSpeed=50;}
    else if (state == '3'){
      vSpeed=75;}
    else if (state == '4'){
      vSpeed=100;}
    else if (state == '5'){
      vSpeed=125;}
    else if (state == '6'){
      vSpeed=150;}
    else if (state == '7'){
      vSpeed=175;}
    else if (state == '8'){
      vSpeed=200;}
    else if (state == '9'){
      vSpeed=225;}
    else if (state == 'q'){
      vSpeed=255;}


if(state=='F')
{
  servo.write(35);
  analogWrite(motorPin1, vSpeed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, vSpeed);
  analogWrite(motorPin4, 0);
}
else if(state=='B')
{
  servo.write(35);
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, vSpeed);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, vSpeed);
}
else if(state=='R')
{
  servo.write(35);
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, vSpeed);
  analogWrite(motorPin3, vSpeed);
  analogWrite(motorPin4, 0);  
}
else if(state=='L')
{
  servo.write(35);
  analogWrite(motorPin1, vSpeed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, vSpeed);
}
else if(state == 'S')
{
  servo.write(35);
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);  
}
else if(state=='W')
{
  digitalWrite(FL,HIGH);
}
else if(state=='w')
{
  digitalWrite(FL,LOW);
}
else if(state=='U')
{
  digitalWrite(BL,HIGH);
}
else if(state=='u')
{
  digitalWrite(BL,LOW);
}
else if(state=='I')
{
  servo.write(0);
}
else if(state=='G')
{
  servo.write(65);
}
else if(state=='H')
{
  servo.write(35);
}
else if(state=='J')
{
  servo.write(35);
}
else if(state=='V')
{
  digitalWrite(PUMP,HIGH);
}
else if(state=='v')
{
  digitalWrite(PUMP,LOW);
}

}
