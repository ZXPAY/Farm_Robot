// do not use this sensor
//沒有使用此電子羅盤，其90度不是真的90度
/*
  HMC5883L Triple Axis Digital Compass. Compass Example.
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-magnetometr-hmc5883l.html
  GIT: https://github.com/jarzebski/Arduino-HMC5883L
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
  名稱：HMC5883L模組(三軸磁場模組)
型號：GY-271
使用芯片：HMC5883L
供電電源：3-5v
通信方式：IIC通信協議
測量範圍：±1.3-8 高斯
*/
/*
#include <HMC5883L.h>
HMC5883L compass;

 void HMC5883L_setup(){
  // Initialize Initialize HMC5883L
  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
 }
float Compass(){
   Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  //Magnetic Field Calculators : http://www.ngdc.noaa.gov/geomag-web/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (-3.88 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 
  // Output
  Serial.print(" Heading = ");
  Serial.print(heading);
  Serial.print(" Degress = ");
  Serial.print(headingDegrees);
  Serial.println();
  
 return headingDegrees;
}
//**********************************************************
int Initial_Compass(){
  float ai, a;
  int C;
  ai =  180;  
  a = Compass();
  if(a  >=  0 &&  a < 90) {C  = 1;}
  else if(a  >=  90 &&  a < 180) {C  = 2;}
  else if(a  >=  180 &&  a < 270) {C  = 3;}
  else if(a  >=  270 &&  a < 360) {C  = 4;}

  switch(C){
    case 1:
          CW  = 105;
          CCW = 85;
    break;
    case 2:
          CW  = 90;
          CCW = 105;
    break;
    case 3:
          CW  = 57.5;
          CCW = 90;
    break;
    case 4:
          CW  = 85;
          CCW = 80;
    break;
  }
  if(a >=  0 && a <= 180)  {Cib = ai  - a;
                            return 1;}    
  else if(a > 180 && a <= 360)  {Cib  = a - ai;   
                            return 2;} 
}
//**********************************************************
float Devtheta2(int state){            //校正角度
float dev,  T, formertheta2;             
  switch(state){
    case 1:
          formertheta2  = theta2;
          theta2  = Compass();
          if(theta2  - formertheta2 > 300)     { theta2 = theta2  - 360;}  
          T  = theta2 + Cib;
          dev = T  -180;
          return  dev;
    break;

    case 2:
         formertheta2  = theta2;
         theta2  =  Compass();
         if(theta2  - formertheta2 < -300)     { theta2 = theta2  + 360;}   
         T  =  theta2 - Cib;                                            
         dev  =  T - 180;
         return dev;
    break;
  }
}
//***********************************************************
void Correct2(int a, int b,  boolean L,  boolean R){                //a,b參數為改變的量
 int Max,Min;
 devtheta2 = Devtheta2(Devstate);
 if(devtheta2 > 1 ||  devtheta2 < -1)    { Max  = 120;   Min  = 50;
                                           LeftSpeed += 3*a;
                                           RightSpeed += 3*b;}
 else{ Max  = 100;   Min  = 55;
       LeftSpeed += a;
       RightSpeed += b;}

 if(LeftSpeed  > Max)  {LeftSpeed = Max;}   else if(LeftSpeed < Min) {LeftSpeed  = Min;}
 if(RightSpeed  > Max)  {RightSpeed = Max;}   else if(RightSpeed < Min) {RightSpeed  = Min;}
 setMotorLeft(LeftSpeed, L);
 setMotorRight(RightSpeed, R);
 theta2print();
}
//**********************************************************
void motorCorrect(int C,  boolean a, boolean  b){
float rec;
devtheta2 = Devtheta2(Devstate);
switch(C){                   
  case 1:                 //順時針誤差-->Z字修正           if(devtheta2 > 1)
    rec = devtheta2;
    Record_Theta1(rec);
    while(devtheta2 > -rec){
      devtheta2 = Devtheta2(Devstate);
      Correct(-1, 1,  true, true);
      theta2print();
    }
    while(devtheta2 > 0){
      Record_Theta1(devtheta2);
       setMotorLeft(80, true);
       setMotorRight(80, true);
      devtheta2 = Devtheta2(Devstate);
      Correct(-1, 1,  true, true);
      theta2print();
    }
  break;

  case  2:                 //逆時針誤差-->Z字修正          if(devtheta2 < -1)
    rec = devtheta2;
    while(devtheta2 < -rec){
      devtheta2 = Devtheta2(Devstate);
      Correct(1, -1,  true, true);
      theta2print();
    }
    while(devtheta2 > 0){
      setMotorLeft(80, true);
      setMotorRight(80, true);
      devtheta2 = Devtheta2(Devstate);
      Correct(-1, 1,  true, true);
      theta2print();
    }
    break;
  }
  
}
//**********************************************************
void motorTurn2_Detect(float alpha,  boolean a,  boolean b){
devtheta2 = Devtheta2(Devstate);
if(devtheta2 > 0){
  while(devtheta2 > alpha){              
      devtheta2 = Devtheta2(Devstate);
      Correct(-1, 1,  a, b);
      // Ultrasonic_Judge();
       theta2print();
  }
}

else if(devtheta2  < 0){
    while(devtheta2 < alpha){            
      devtheta2 = Devtheta2(Devstate);
      Correct(1, -1, a, b);
     //  Ultrasonic_Judge();
       theta2print();
    }
}

}
//***********************************************************
void motor_spinAround2(int speed1,  int speed2,  float alpha){    //原地旋轉
devtheta2 = Devtheta2(Devstate);
if(alpha  >= 0){
  while(devtheta2 < alpha){              
      devtheta2 = Devtheta2(Devstate);
      if(alpha  - devtheta2  <  5)      {motorBrake(50, 50, true,  false);}         //快到時煞車
      else{
      setMotorLeft(speed1, true);
      setMotorRight(speed2, false);}
                        theta2print(); 
}
}

else  if(alpha  < 0){
    while(devtheta2 > alpha){            
      devtheta2 = Devtheta2(Devstate);
      if(devtheta2  - alpha  <  5)      {motorBrake(50, 50, false,  true);}         //快到時煞車
      else{
      setMotorLeft(speed1 , false);
      setMotorRight(speed2 , true);}
                        theta2print(); 
}
}
}
//***********************************************************
void theta2print(){
 int level;
 level  = (abs(level_L) + abs(level_R)) * 0.5;
 level  = level/9.5;
 lcd.setCursor(0, 0); // 將游標移動第0欄、第1列。        
 lcd.print("Z=");
 lcd.print(devtheta2,2);
 lcd.print(" ");
 lcd.setCursor(8, 0);
 lcd.print("dis=");
 lcd.print(level,0);
 lcd.setCursor(0,1); lcd.print("L=");  lcd.print(LeftSpeed,0);
 lcd.setCursor(8,1); lcd.print("R=");  lcd.print(RightSpeed,0);
Serial.print("dis=");
Serial.print(level);
Serial.print("devtheta2=");
Serial.println(devtheta2);

}
*/
//*****************************************************************************************************************************************************************//
