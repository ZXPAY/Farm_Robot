//MG996R 伺服馬達宣告和基本程式碼
#include <Servo.h>
//Pin
const int ServoLPin = 12;
const int ServoRPin  = 7;
const int ServoUp_DownPin  = 10;

//Servo宣告*************
Servo ServoL;
Servo ServoR;
Servo  ServoUp_Down;
//***********************************************************
void Servo_setup(){
 ServoL.attach(ServoLPin);          // attaches the servo on pins to the servo object
 ServoR.attach(ServoRPin); 
 ServoUp_Down.attach(ServoUp_DownPin);  
 ServoL.write(posiL);                   // Initialize Servo Position
 ServoR.write(posiR);
 ServoUp_Down.write(posiUp_Down);
}
//***********************************************************
void setServoL(int p){             //設定左邊伺服馬達位置
  ServoL.write(p);
}
void setServoR(int p){             //設定右邊伺服馬達位置
  ServoR.write(p);
}
void setServoUD(int p){             //設定上下移動的伺服馬達位置
  ServoUp_Down.write(p);
}
//***********************************************************
void setServoLR(int d1, int d2){       //左右夾取動作          //d1: 現在位置   d2: 後來位置
if(d1 > d2){                                                  //30-->63
  while(d1 > d2){                                             //63-->30     
    ServoL.write(d1);
    ServoR.write(140-d1);               //相差150度
    Delay(20);
    d1--;
  }
}

else if(d2 > d1){
  while(d2 > d1){
    ServoL.write(d1);
    ServoR.write(140-d1);
    Delay(20);
    d1++;
  }
}
}
//***********************************************************
void setServoUp_Down(int d1, int d2){ //上升下降動作    //d1: 現在位置   d2: 後來位置     
if(d1 > d2){                                           //20-->95     
  while(d1 > d2){                                      //95-->10
    ServoUp_Down.write(d1);
    Delay(20);
    d1--;
  }
}

else if(d2 > d1){
  while(d2 > d1){
    ServoUp_Down.write(d1);
    Delay(20);
    d1++;
  }
}

}
//*****************************************************************************************************************************************************************//
