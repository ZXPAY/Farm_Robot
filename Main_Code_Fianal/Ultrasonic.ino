//超音波宣告和基本程式碼
/*
HC-SR04 Ping distance sensor                                
VCC to arduino 5v GND to arduino GND                                
Echo to Arduino pin 13 Trig to Arduino pin 12                  
More info at: http://goo.gl/kJ8Gl 
Working Voltage DC  5 V
Working Current     15mA
Working Frequency   40Hz
Max Range           4m
Min Range           2cm
MeasuringAngle      15 degree
Trigger Input Signal 10uS TTL pulse
Echo Output Signal Input TTL lever signal and the range in  proportion  Dimension 45*20*15mm                                   
*/
//超音波宣告**********
//Pin
const int trigPin1 = 34;
const int echoPin1 = 35;
const int trigPin2 = 44;
const int echoPin2 = 46;
const int trigPin3 = 38;
const int echoPin3 = 39;
const int trigPin4 = 40;
const int echoPin4 = 41;
const int trigPin5 = 42;
const int echoPin5 = 43;
void Ultrasonic_setup(){
//Ultrasonic
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
//pinMode(trigPin3, OUTPUT);
//pinMode(echoPin3, INPUT);
pinMode(trigPin4, OUTPUT);
pinMode(echoPin4, INPUT);
pinMode(trigPin5, OUTPUT);
pinMode(echoPin5, INPUT);
}

//超音波程式碼**********************************************************
void Ultra_distanceR1(){
  long duration1;
  //V = 331 + 0.6 * Tmp;                //Let Tmp = 28 --> V = 347.8
  //L = 2000000/V;                  //duration/2/10000/V  = V*duration/20000  = dis  -->  最大duration  = 2000000/V      
  //--> L = 5750.4
  digitalWrite(trigPin1, LOW);
  Delay(0.005);
  digitalWrite(trigPin1, HIGH);
  Delay(0.01);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH, 6000);     //讀取 pin 腳上的脈衝 (高或低電位) 持續時間，如果輸入值是 HIGH，pulseIn() 會等 pin 腳上的電位變成 HIGH 之後開始計時，等到又變回 LOW 才停止
                                                 //之後會回傳脈衝的持續時間，單位是微秒；假如在指定的時間內沒有出現任何脈衝或是脈衝長度太長，則會放棄讀取並回傳 0。
  distanceR1 = duration1  * 0.01739;           //The Pace of Sound = 1 / Speed of Sound = 1 / 0.03435 = 29.1 ųs/cm     PS: Speed of Sound = 331 + 0.6*T (T 為 Celsius溫度)(m/s)
  if(distanceR1  <=  0)  {distanceR1 = 100;}           //距離大於100公分，全部等於100公分
}
//**********************************************************
void Ultra_distanceR2(){
  int duration2;
  digitalWrite(trigPin2, LOW);
  Delay(0.005);
  digitalWrite(trigPin2, HIGH);
  Delay(0.01);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH, 6000);     //讀取 pin 腳上的脈衝 (高或低電位) 持續時間，如果輸入值是 HIGH，pulseIn() 會等 pin 腳上的電位變成 HIGH 之後開始計時，等到又變回 LOW 才停止
                                                //之後會回傳脈衝的持續時間，單位是微秒；假如在指定的時間內沒有出現任何脈衝或是脈衝長度太長，則會放棄讀取並回傳 0。
  distanceR2 = duration2  * 0.01739;           //The Pace of Sound = 1 / Speed of Sound = 1 / 0.03435 = 29.1 ųs/cm     PS: Speed of Sound = 331 + 0.6*T (T 為 Celsius溫度)(m/s)
  if(distanceR2  <=  0)  {distanceR2 = 100;}           //距離大於100公分，全部等於100公分
}
//**********************************************************
void Ultra_distanceR3(){
  int duration3;
  digitalWrite(trigPin3, LOW);
  Delay(0.005);
  digitalWrite(trigPin3, HIGH);
  Delay(0.01);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH, 6000);     //讀取 pin 腳上的脈衝 (高或低電位) 持續時間，如果輸入值是 HIGH，pulseIn() 會等 pin 腳上的電位變成 HIGH 之後開始計時，等到又變回 LOW 才停止
                                                //之後會回傳脈衝的持續時間，單位是微秒；假如在指定的時間內沒有出現任何脈衝或是脈衝長度太長，則會放棄讀取並回傳 0。
  distanceR3 = duration3  * 0.01739;           //The Pace of Sound = 1 / Speed of Sound = 1 / 0.03435 = 29.1 ųs/cm     PS: Speed of Sound = 331 + 0.6*T (T 為 Celsius溫度)(m/s)
  if(distanceR3  <=  0)  {distanceR3 = 100;}           //距離大於100公分，全部等於100公分
}
//**********************************************************
void Ultra_distanceF(){
  int duration4;
  digitalWrite(trigPin4, LOW);
  Delay(0.005);
  digitalWrite(trigPin4, HIGH);
  Delay(0.01);
  digitalWrite(trigPin4, LOW);
  duration4 = pulseIn(echoPin4, HIGH, 6000);     //讀取 pin 腳上的脈衝 (高或低電位) 持續時間，如果輸入值是 HIGH，pulseIn() 會等 pin 腳上的電位變成 HIGH 之後開始計時，等到又變回 LOW 才停止
                                                //之後會回傳脈衝的持續時間，單位是微秒；假如在指定的時間內沒有出現任何脈衝或是脈衝長度太長，則會放棄讀取並回傳 0。
  distanceF = duration4  * 0.01739;           //The Pace of Sound = 1 / Speed of Sound = 1 / 0.03435 = 29.1 ųs/cm     PS: Speed of Sound = 331 + 0.6*T (T 為 Celsius溫度)(m/s)
  if(distanceF  <=  0)  {distanceF = 100;}           //距離大於100公分，全部等於100公分
}
//**********************************************************
void Ultra_distanceL1(){
  int duration5;
  digitalWrite(trigPin5, LOW);
  Delay(0.005);
  digitalWrite(trigPin5, HIGH);
  Delay(0.01);
  digitalWrite(trigPin5, LOW);
  duration5 = pulseIn(echoPin5, HIGH, 6000);     //讀取 pin 腳上的脈衝 (高或低電位) 持續時間，如果輸入值是 HIGH，pulseIn() 會等 pin 腳上的電位變成 HIGH 之後開始計時，等到又變回 LOW 才停止
                                                //之後會回傳脈衝的持續時間，單位是微秒；假如在指定的時間內沒有出現任何脈衝或是脈衝長度太長，則會放棄讀取並回傳 0。
  distanceL1 = duration5  * 0.01739;           //The Pace of Sound = 1 / Speed of Sound = 1 / 0.03435 = 29.1 ųs/cm     PS: Speed of Sound = 331 + 0.6*T (T 為 Celsius溫度)(m/s)
  if(distanceL1  <=  2)  {distanceL1 = 100;}           //距離大於100公分，全部等於100公分
}
//**********************************************************
void Ultrasonic_RESET(){                                           //右方超音波校正角度
  Ultra1_min = 100;
  Ultra3_min = 100;
  dev_angle = 0;
}
//**********************************************************
void Ultrasonic_Detect(){                                      
  Ultra_distanceR1();                      
  Ultra_distanceR3();
  if((distanceR1 - distanceR3 > 2.1) && (distanceR1 - distanceR3 < 12)){
    Ultra1_min = distanceR1;
    Ultra3_min = distanceR3;
   } 

  else if((distanceR1 - distanceR3 < -2.1) && (distanceR1 - distanceR3 > -12)){
    Ultra1_min = distanceR1;
    Ultra3_min = distanceR3;
   } 
}
//**********************************************************
void Ultrasonic_Cal(){                                           //右 方超音波校正角度 ， 兩超音波距離約24公分
  if(Ultra3_min > Ultra1_min && Ultra3_min < 50 && Ultra1_min < 50) {
                                                                     dev_angle = asin((Ultra3_min - Ultra1_min)/24) * 180 / 3.14;    
                                                                     if(dev_angle > 0 && dev_angle < 40) {dev_angle = dev_angle;}    
                                                                     else{dev_angle = 0;}
                                                                     }
  else if(Ultra3_min < Ultra1_min && Ultra3_min < 50 && Ultra1_min < 50) {
                                                                           dev_angle = asin((Ultra1_min - Ultra3_min)/24) * 180 / 3.14;
                                                                           if(dev_angle > 0 && dev_angle < 40) {dev_angle = -dev_angle;}    
                                                                           else{dev_angle = 0;}
                                                                          }
}
//**********************************************************
void Ultra_Staight(){
     float Recdis[100], c;
     static int i = 0;
     Ultra_distanceL1();
     if(distanceL1 < 50) {Recdis[i] = distanceL1; i++;}
     if(i = 99) {c = Recdis[99] - Recdis[0];
                 if(c > 10)  {Correct(-5, 5, true, true);}
                 else if(c < -10)  {Correct(5, -5, true, true);}}
}
//**********************************************************
void Ultra_Switch(){
static int n = 0;
if(n == 0){ 
            Ultra_distanceR1();
            Ultra_distanceR2();
            Ultra_distanceR3();
            n = 1;
          }
else if(n == 1){
                 Ultra_distanceF();
                 Ultra_distanceL1();
                 n = 0;
               }
}
//**********************************************************
void Ultraprint(){   
 
  lcd.setCursor(0,0);lcd.print(distanceR1,1);lcd.print(" ");
  lcd.setCursor(6,0);lcd.print(distanceR2,1);lcd.print(" ");
  lcd.setCursor(12,0);lcd.print(distanceR3,1);lcd.print(" ");
  lcd.setCursor(0,1);lcd.print(distanceF,1);lcd.print(" ");
  lcd.setCursor(6,1);lcd.print(distanceL1,1);lcd.print(" ");
        
Serial.print(",disR1=");
Serial.print(distanceR1);
Serial.print(",disR2=");
Serial.print(distanceR2);
Serial.print(",disR3=");
Serial.print(distanceR3);
Serial.print(",disF=");
Serial.print(distanceF);
Serial.print(",disL1=");
Serial.println(distanceL1);
}
//*****************************************************************************************************************************************************************//
