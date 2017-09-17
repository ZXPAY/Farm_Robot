//無刷馬達宣告************
/*
 * 減速比: 1/49
 * 定格扭力: 18  kg*cm
 * 定格轉速: 120 rpm
 * 功率:約 22 W
 * 直徑: 45.9 mm
 */
//Pin
const int motorLeft=4;
const int motorRight=5;
const int Ina1=30;
const int Inb1=31;
const int Ina2=32;
const int Inb2=33;

void Motor_setup(){
//motor
pinMode(Ina1,OUTPUT);
pinMode(Inb1,OUTPUT);
pinMode(Ina2,OUTPUT);
pinMode(Inb2,OUTPUT);
pinMode(motorLeft,OUTPUT);
pinMode(motorRight,OUTPUT);
}
//馬達程式碼***********************************************************
void setMotorLeft(int speedL,boolean reverse){                    //true:正轉
  LeftSpeed = speedL;
analogWrite(motorLeft,LeftSpeed);                                    //false:反轉
digitalWrite(Ina1 , reverse);
digitalWrite(Inb1 , !reverse);
}
//***********************************************************
void setMotorRight(int speedR,boolean reverse){                    //true:正轉
  RightSpeed  = speedR;
analogWrite(motorRight,RightSpeed);                                    //false:反轉
digitalWrite(Ina2 , !reverse);
digitalWrite(Inb2 , reverse);
}
//車子前進**********************************************************
void motorStart(int speedL,int speedR,boolean a,boolean b){
int C;                  
//Serial.println("Start------------------------------------");
while(LeftSpeed <= speedL && RightSpeed <= speedR)
{ 
LeftSpeed+=5;
RightSpeed+=5;
if(LeftSpeed >= speedL)  {LeftSpeed = speedL;}
if(RightSpeed >= speedR)  {RightSpeed = speedR;}
setMotorLeft(LeftSpeed,a);
setMotorRight(RightSpeed,b);
thetaprint();  //speedprint();
 if(LeftSpeed == speedL && RightSpeed == speedR)
 {break;}
}
}
//***********************************************************
void motorBrake(int speed01  , int speed02  , boolean a , boolean b){
  int C;
//  Serial.println("Brake------------------------------------");
while(LeftSpeed >= speed01 && RightSpeed >= speed02)
{ 
  RGB_lightL(1,0,0);
  RGB_lightR(1,0,0);
LeftSpeed-=5;
RightSpeed-=5;
if(LeftSpeed <= speed01)  {LeftSpeed = speed01;}
if(RightSpeed <= speed02)  {RightSpeed = speed02;}
setMotorLeft(LeftSpeed,a);
setMotorRight(RightSpeed,b);
thetaprint(); //speedprint();
 if(LeftSpeed==speed01 && RightSpeed ==speed02)
 {break;}
}
Delay(200);
  RGB_lightL(0,0,0);
  RGB_lightR(0,0,0);
}

//**********************************************************
void speedprint(){   
     
 lcd.setCursor(0, 1); // 將游標移動第0欄、第1列。
 lcd.print("L=");
 lcd.print(LeftSpeed);
 lcd.print("   ");
 lcd.setCursor(8, 1); // 將游標移動第0欄、第1列。
 lcd.print("R=");
 lcd.print(RightSpeed);
 lcd.print("   ");
 
/*
Serial.print("LeftSpeed=");
Serial.print(LeftSpeed);
Serial.print(",");
Serial.print("RightSpeed=");
Serial.println(RightSpeed);
*/
}
//*****************************************************************************************************************************************************************//
