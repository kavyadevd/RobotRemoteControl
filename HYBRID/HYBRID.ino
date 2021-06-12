#include <PS2X_lib.h>
#include<NewPing.h>
#include<Servo.h>
#define maxping 400
#define esc 2               //BLDC
#define GM1pwm 3            //Gimbal Motor 1 PWM
#define GM2pwm 4            //Gimbal Motor 2 PWM
#define AMpwm 5             //Arm Motor PWM
#define climberpwm 6        //Pole Climber motor PWM (2 motors shorted on same signal)
#define clampingpwm 7       //Pole Clamping motor PWM (2 motors shorted on same signal)
#define locoleftpwm 8       //Hybrid Wheels Motor PWM (Left)
#define locorightpwm 9      //Hybrid Wheels Motor PWM(right)

#define GSpin 22            //Gimbal Servo
#define GM1Dir 23           //Gimbal Motor 1 Direction
#define GM1Break 25         //Gimbal Motor 1 Break
#define GM2Dir 24           //Gimbal Motor 2 Direction
#define GM2Break 26         //Gimbal Motor 2 Break
#define ABS 27              //Arm Base Servo
#define ADSS 29             //Arm Dual Shaft Servo
#define AGRS 31             //Arm Gripper Rotation Servo
#define AGGS 33             //Arm Gripper Gripping Servo
#define AMDir 28            //Arm Motor Direction
#define AMBreak 30          //Arm Motor Break
#define climberDir 32       //Pole Climber motor Direction
#define climberBreak 34     //Pole Climber motor Break
#define clampingDir 35      //Pole Clamper motor Direction
#define clampingBreak 37    //Pole Clamper motor Break
#define locoleftDir 36      //Hybrid Wheel Motor Direction (left)
#define locoleftBreak 38    //Hybrid Wheel Motor Break (left)
#define locorightDir 39     //Hybrid Wheel Motor Direction (right)
#define locorightBreak 41   //Hybrid Wheel Motor Break (right)
#define trig1 42            //1st Ping Sensor Trigger
#define echo1 43            //1st Ping Sensor Echo
#define trig2 44            //2nd Ping Sensor Trigger
#define echo2 45            //2nd Ping Sensor Echo
#define psdata 46           //PS2 Data Pin
#define pscommand 48        //PS2 Command Pin
#define psatt 50            //PS2 Attention Pin
#define psclock 52          //PS2 Clock Pin

PS2X ps;
Servo bldc,GServo,ArmServo[4];
NewPing sonar1(trig1,echo1,maxping),sonar2(trig2,echo2,maxping);

int bldcSpeed=500, gimbalm1Val=250, gimbalm2Val=90, armVal=50, gServoAngle=90, armBaseAngle=90, armDualAngle=90, armGripperAngle=90, armGrippingAngle=0;

int locoleftVal=100, locorightVal=100, gripmotorVal=255, climbmotorVal=255;

int mode=0;


void setup() {

  Serial.begin(9600);

  ps.config_gamepad(psclock,pscommand,psatt,psdata);

  /*----------------------------------Gimbal Initialization-------------------------------------*/
  pinMode(GM1pwm,OUTPUT);
  pinMode(GM1Dir,OUTPUT);
  pinMode(GM1Break,OUTPUT);

  pinMode(GM2pwm,OUTPUT);
  pinMode(GM2Dir,OUTPUT);
  pinMode(GM2Break,OUTPUT);

  GServo.attach(GSpin);
  GServo.write(gServoAngle);
  bldc.attach(esc);
  bldc.writeMicroseconds(bldcSpeed);

  /*-------------------------------------Hybrid Locomotion Init-------------------------------------*/
  pinMode(locoleftpwm,OUTPUT);
  pinMode(locoleftDir,OUTPUT);
  pinMode(locoleftBreak,OUTPUT);

  pinMode(locorightpwm,OUTPUT);
  pinMode(locorightDir,OUTPUT);
  pinMode(locorightBreak,OUTPUT);

  /*---------------------------------------Pole Climbing Init-----------------------------------------*/
  pinMode(climberpwm,OUTPUT);
  pinMode(climberDir,OUTPUT);
  pinMode(climberBreak,OUTPUT);

  pinMode(clampingpwm,OUTPUT);
  pinMode(clampingDir,OUTPUT);
  pinMode(clampingBreak,OUTPUT);

  /*--------------------------------------Arm Initialization------------------------------------------*/
  pinMode(AMpwm,OUTPUT);
  pinMode(AMDir,OUTPUT);
  pinMode(AMBreak,OUTPUT);

  ArmServo[0].attach(ABS);
  ArmServo[1].attach(ADSS);
  ArmServo[2].attach(AGRS);
  ArmServo[3].attach(AGGS);

  for(int i=0;i<3;i++){
    ArmServo[i].write(90);
  }
  ArmServo[3].write(armGrippingAngle);

  /*------------------------------------Everything Initialized-------------------------------------------*/

}

void loop() {

  ps.read_gamepad();

  /*-------------------------------------Breaks for all motors------------------------------------------*/
  digitalWrite(GM1Break,HIGH);
  digitalWrite(GM2Break,HIGH);
  digitalWrite(locoleftBreak,HIGH);
  digitalWrite(locorightBreak,HIGH);
  digitalWrite(climberBreak,HIGH);
  digitalWrite(clampingBreak,HIGH);
  digitalWrite(AMBreak,HIGH);

  if(mode==0)
  {

       if(ps.Button(PSB_R1))                                      //Increase bldc speed by 10.
       {
             Serial.println("R1 PRESSED");
             bldcSpeed+=5;
             bldc.writeMicroseconds(bldcSpeed);
             Serial.println(bldcSpeed);
             delay(10);
       }
       else if(ps.Button(PSB_R2))                                 //Decrease bldc speed by 10.
       {
             Serial.println("R2 PRESSED");
             bldcSpeed-=5;
             bldc.writeMicroseconds(bldcSpeed);
             Serial.println(bldcSpeed);
             delay(10);
        
       }
       if(ps.Button(PSB_L1))                                      //Gimble up.
       {
             Serial.println("L1 pressed Gimble up");
             digitalWrite(GM1Break,LOW);
             digitalWrite(GM1Dir,HIGH);
                
             digitalWrite(GM2Break,LOW);
             digitalWrite(GM2Dir,HIGH);
             
             analogWrite(GM1pwm,250);
             analogWrite(GM2pwm,80);
             delay(10);
       }
       else if(ps.Button(PSB_L2))                                 //Gimble Down.
       {
             Serial.println("L1 pressed Gimble Down");
             digitalWrite(GM1Break,LOW);
             digitalWrite(GM1Dir,LOW);
                
             digitalWrite(GM2Break,LOW);
             digitalWrite(GM2Dir,LOW);
      
             analogWrite(GM2pwm,250);
             analogWrite(GM1pwm,80);
             delay(10);  
       }
       if(ps.Button(PSB_PAD_RIGHT))                               //Gimble Servo Rotates towards 180.
       {
        gServoAngle+=1;
        gServoAngle= gServoAngle>180?180:gServoAngle;
        GServo.attach(GSpin);
        GServo.write(gServoAngle);
        Serial.println(gServoAngle);
        delay(10);
       }
       else if(ps.Button(PSB_PAD_LEFT))                           //Gimble Servo Rotates towards 0.
       {
        gServoAngle-=1;
        gServoAngle= gServoAngle<0?0:gServoAngle;
        GServo.attach(GSpin);
        GServo.write(gServoAngle);
        Serial.println(gServoAngle);
        delay(10);
       }
       if(ps.Analog(PSS_LY)==255)       //Left Wheels Forward
       {
         digitalWrite(locoleftBreak,LOW);
         digitalWrite(locoleftDir,LOW);
         analogWrite(locoleftpwm,100);
         Serial.println("Left Wheels Forward");
         delay(15);
         
       }
       else if(ps.Analog(PSS_LY)==0)    //Left Wheels Backward
       {
         digitalWrite(locoleftBreak,LOW);
         digitalWrite(locoleftDir,HIGH);
         analogWrite(locoleftpwm,100);
         Serial.println("Left Wheels Backward");
         delay(15);
       }
       if(ps.Analog(PSS_RY)==255)       //Right Wheels Forward
       {
         digitalWrite(locorightBreak,LOW);
         digitalWrite(locorightDir,HIGH);
         analogWrite(locorightpwm,100);
         Serial.println("Right Wheels Forward");
         delay(15);
         
       }
       else if(ps.Analog(PSS_RY)==0)    //Right Wheels Backward
       {
         digitalWrite(locorightBreak,LOW);
         digitalWrite(locorightDir,LOW);
         analogWrite(locorightpwm,100);
         Serial.println("Right Wheels Backward");
         delay(15);
       }
       if(ps.Analog(PSS_LX)==255)       //Left Wheels Forward
       {
         digitalWrite(locoleftBreak,LOW);
         digitalWrite(locoleftDir,HIGH);
         analogWrite(locoleftpwm,200);
         Serial.println("Left Wheels Forward");
         delay(15);
         
       }
       else if(ps.Analog(PSS_LX)==0)    //Left Wheels Backward
       {
         digitalWrite(locoleftBreak,LOW);
         digitalWrite(locoleftDir,HIGH);
         analogWrite(locoleftpwm,50);
         Serial.println("Left Wheels Backward");
         delay(15);
       }
       if(ps.Analog(PSS_RX)==255)       //Right Wheels Forward
       {
         digitalWrite(locorightBreak,LOW);
         digitalWrite(locorightDir,LOW);
         analogWrite(locorightpwm,50);
         Serial.println("Right Wheels Forward");
         delay(15);
         
       }
       else if(ps.Analog(PSS_RX)==0)    //Right Wheels Backward
       {
         digitalWrite(locorightBreak,LOW);
         digitalWrite(locorightDir,LOW);
         analogWrite(locorightpwm,200);
         Serial.println("Right Wheels Backward");
         delay(15);
       }
       
       if(ps.Button(PSB_CROSS)){
        bldcSpeed=900;
        bldc.writeMicroseconds(bldcSpeed);
        Serial.println(bldcSpeed);
       }

       if(ps.Button(PSB_SELECT)&&ps.Button(PSB_R1)){
        
          mode=1;
        
       }

  }
  else if(mode==1){
      if(ps.Button(PSB_START)){
        mode=0;
      }
      if(ps.ButtonPressed(PSB_L2)){
        armBaseAngle-=1;
        ArmServo[0].write(armBaseAngle);
        delay(15);
      }
      else if(ps.ButtonPressed(PSB_R2)){
        armBaseAngle+=1;
        ArmServo[0].write(armBaseAngle);
        delay(15);
      }

      if(ps.ButtonPressed(PSB_L1)){
        armDualAngle-=1;
        ArmServo[1].write(armDualAngle);
        delay(15);
      }
      else if(ps.ButtonPressed(PSB_R2)){
        armDualAngle+=1;
        ArmServo[1].write(armDualAngle);
        delay(15);
      }

      if(ps.ButtonPressed(PSB_PAD_LEFT)){
        armGripperAngle-=1;
        ArmServo[2].write(armGripperAngle);
        delay(15);
      }
      else if(ps.ButtonPressed(PSB_PAD_RIGHT)){
        armGripperAngle+=1;
        ArmServo[2].write(armGripperAngle);
        delay(15);
      }

      if(ps.ButtonPressed(PSB_TRIANGLE)){
        armGrippingAngle-=1;
        ArmServo[3].write(armGrippingAngle);
        delay(15);
      }
      else if(ps.ButtonPressed(PSB_CROSS)){
        armGrippingAngle+=1;
        ArmServo[3].write(armGrippingAngle);
        delay(15);
      }

      if(ps.ButtonPressed(PSB_SQUARE)){
        digitalWrite(AMBreak,LOW);
        digitalWrite(AMDir,LOW);
        analogWrite(AMpwm,armVal);
        delay(15);
      }
      else if(ps.ButtonPressed(PSB_CIRCLE)){
        digitalWrite(AMBreak,LOW);
        digitalWrite(AMDir,HIGH);
        analogWrite(AMpwm,armVal);
        delay(15);
      }
  }

}
