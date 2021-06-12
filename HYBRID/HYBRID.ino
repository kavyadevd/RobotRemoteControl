#include<PS2X_lib.h>
#include<Servo.h>
#include<NewPing.h>


/*----------------------------GIMBLE--------------------------------*/
#define GMotor1PWM 3                                                          //UP
#define GMotor1DIR 23
#define GMotor1BRK 25

 
#define GMotor2PWM 4                                                          //DOWN
#define GMotor2DIR 24
#define GMotor2BRK 26

#define SGimblePin 22                                                         //SERVO
#define SbldcPin 2                                                            //ESC

/*--------------------------HYBRID-------------------------------------*/
#define HMotor1PWM 4                                                          //MOTION
#define HMotor1DIR 26
#define HMotor1BRK 27

 
#define HMotor2PWM 5                                                          //MOTION
#define HMotor2DIR 28
#define HMotor2BRK 29


#define HCMotorPWM 6                                                          //CLIMBing
#define HCMotorDIR 32
#define HCMotorBRK 34

#define HGMotorPWM 7                                                          //GRIPPING
#define HGMotorDIR 
#define HGMotorBRK

/*--------------------------ARM------------------------------------------*/

#define ArmMotorPWM 5                                                         //BASE MOTOR
#define ArmMotorDIR 28
#define ArmMotorBRK 30
#define SArmBase 27                                                           //BASE SERVO
#define SArmDual 29                                                           //DUAL SHAFT SERVO
#define SGripperR 31                                                          //GRIPPER BASE ROTATION                                                       
#define SGripperG 33                                                          //GRIPPING SERVO

/*PS Controls: 46 48 50 52     -    DATA COMMAND ATTENTION CLOCK  */

Servo Sarm[4],SGimble,Sbldc;                                                  // Arm,    Gimble yoying,   Bldc 
PS2X ps;
//NewPing sonar;
int flag=0;
int BldcSpeed=1200;
int SGimbleAngle=90;
int HSpeedL=200;
int HSpeedR=200;

void setup() 
{
  Serial.begin(9600);
  
  ps.config_gamepad(52,46,48,50);                                   //(CLOCK,COMMAND,ATTENTION,DATA)
 
/*--------------------GIMBLE MOTORS --------------------------------------------------------*/  
  pinMode(GMotor1PWM,OUTPUT);                                       //Motor1 PWM
  pinMode(GMotor1DIR,OUTPUT);                                       //Motor1 DIR
  pinMode(GMotor1BRK,OUTPUT);                                       //Motor1 BREAK
    
  pinMode(GMotor2PWM,OUTPUT);                                       //Motor2 PWM
  pinMode(GMotor2DIR,OUTPUT);                                       //Motor2 DIR
  pinMode(GMotor2BRK,OUTPUT);                                       //Motor2 BREAK

  SGimble.attach(SGimblePin);                                       //22 for gimble Servo
  SGimble.write(90);
  Sbldc.attach(SbldcPin);                                          //PWM 2 pin for bldc
  Sbldc.write(900);

/*-----------------------MOTORS FOR HYBRID'S MOTION-----------------------------------------*/  
  pinMode(5,OUTPUT);                                       //Motor1 PWM
  pinMode(28,OUTPUT);                                       //Motor1 DIR
  pinMode(29,OUTPUT);                                       //Motor1 BREAK
    
  pinMode(6,OUTPUT);                                       //Motor2 PWM
  pinMode(31,OUTPUT);                                       //Motor2 DIR
  pinMode(32,OUTPUT);                                       //Motor2 BREAK
}

void loop() 
{
 ps.read_gamepad();

 /*-----------------------GIMBLE-------------------------------------------------------------*/
 digitalWrite(GMotor1BRK,HIGH);
 digitalWrite(GMotor2BRK,HIGH);
 SGimble.detach();

 if(ps.Button(PSB_R1))                                      //Increase bldc speed by 10.
 {
       Serial.println("R1 PRESSED");
       BldcSpeed+=10;
       Sbldc.writeMicroseconds(BldcSpeed);
       Serial.println(BldcSpeed);
       delay(150);
 }
 else if(ps.Button(PSB_R2))                                 //Decrease bldc speed by 10.
 {
       Serial.println("R2 PRESSED");
       BldcSpeed-=10;
       Sbldc.writeMicroseconds(BldcSpeed);
       Serial.println(BldcSpeed);
       delay(150);
  
 }
 if(ps.Button(PSB_L1))                                      //Gimble up.
 {
       Serial.println("L1 pressed Gimble up");
       digitalWrite(GMotor1BRK,LOW);
       digitalWrite(GMotor1DIR,HIGH);
          
       digitalWrite(GMotor2BRK,LOW);
       digitalWrite(GMotor2DIR,HIGH);
       
       analogWrite(GMotor1PWM,250);
       analogWrite(GMotor2PWM,80);
       delay(150);
 }
 else if(ps.Button(PSB_L2))                                 //Gimble Down.
 {
       Serial.println("L1 pressed Gimble Down");
       digitalWrite(GMotor1BRK,LOW);
       digitalWrite(GMotor1DIR,LOW);
          
       digitalWrite(GMotor2BRK,LOW);
       digitalWrite(GMotor2DIR,LOW);

       analogWrite(GMotor2PWM,250);
       analogWrite(GMotor1PWM,80);
       delay(150);  
 }

 if(ps.Button(PSB_PAD_RIGHT))                               //Gimble Servo Rotates towards 180.
 {
  SGimbleAngle+=1;
  SGimbleAngle= SGimbleAngle>180?180:SGimbleAngle;
  SGimble.attach(SGimblePin);
  SGimble.write(SGimbleAngle);
  Serial.println(SGimbleAngle);
  delay(10);
 }
 else if(ps.Button(PSB_PAD_LEFT))                           //Gimble Servo Rotates towards 0.
 {
  SGimbleAngle-=1;
  SGimbleAngle= SGimbleAngle<0?0:SGimbleAngle;
  SGimble.attach(SGimblePin);
  SGimble.write(SGimbleAngle);
  Serial.println(SGimbleAngle);
  delay(10);
 }


 /*----------------------HYBRID Motion------------------------------------------------------*/  
 digitalWrite(HMotor1BRK,HIGH);
 digitalWrite(HMotor2BRK,HIGH);
 
 if(ps.Analog(PSS_RY)==255 && ps.Analog(PSS_LY)==255)       //Hybrid Moves Forward
 {
   digitalWrite(HMotor1BRK,LOW);
   digitalWrite(HMotor1DIR,LOW);

   digitalWrite(HMotor2BRK,LOW);
   digitalWrite(HMotor2DIR,LOW);

   analogWrite(HMotor1PWM,100);
   analogWrite(HMotor2PWM,100);
   Serial.println("100,100 FORWARD");
   delay(150);
   
 }
 else if(ps.Analog(PSS_RY)==0 && ps.Analog(PSS_LY)==0)      //Hybrid Moves Backwards
 {
   digitalWrite(HMotor1BRK,LOW);
   digitalWrite(HMotor1DIR,HIGH);

   digitalWrite(HMotor2BRK,LOW);
   digitalWrite(HMotor2DIR,HIGH);

   analogWrite(HMotor1PWM,100);
   analogWrite(HMotor2PWM,100);
   Serial.println("100,100 BACKWARD");
   delay(150);

 } 
 if(ps.Analog(PSS_RX)==255 && ps.Analog(PSS_LX)==255)       //Hybrid turns Right
 {
   
   digitalWrite(HMotor1BRK,LOW);
   digitalWrite(HMotor1DIR,HIGH);

   digitalWrite(HMotor2BRK,LOW);
   digitalWrite(HMotor2DIR,HIGH);

   analogWrite(HMotor1PWM,150);                         //!!!NOT DECIDED YET
   analogWrite(HMotor2PWM,50);
   Serial.println("150,50 RIGHT");
   delay(150);
 }
 else if(ps.Analog(PSS_RX)==0 && ps.Analog(PSS_LX)==0)      //Hybrid turns Left
 {

   digitalWrite(HMotor1BRK,LOW);
   digitalWrite(HMotor1DIR,HIGH);

   digitalWrite(HMotor2BRK,LOW);
   digitalWrite(HMotor2DIR,HIGH);

   analogWrite(HMotor1PWM,50);                           //!!!NOT DECIDED YET
   analogWrite(HMotor2PWM,150);
   Serial.println("50,150 LEFT");
   delay(150);
 }

}
