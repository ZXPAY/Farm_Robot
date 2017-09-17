/*參考資料
 * PID控制理論: http://www.tabbymeow.com/pid%E6%8E%A7%E5%88%B6%E7%90%86%E8%AB%965-%E8%AA%BF%E6%95%B4pid%E5%8F%83%E6%95%B8/
 * Ziegler–Nichols method : https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method#cite_note-microstar-2
 * How to calculate kp ki kd : http://electronics.stackexchange.com/questions/127524/how-to-calculate-kp-kd-and-ki
 * http://forum.arduino.cc/index.php?topic=43396.0 : 
 * If the system must remain online, one tuning method is to first set Ki and Kd values to zero. 
 * Increase the Kp until the output of the loop oscillates, then the Kp should be set to approximately half of that value for a "quarter amplitude decay" type response. 
 * Then increase Ki until any offset is correct in sufficient time for the process. However, too much Ki will cause instability. 
 * Finally, increase Kd, if required, until the loop is acceptably quick to reach its reference after a load disturbance. 
 * However, too much Kd will cause excessive response and overshoot. A fast PID loop tuning usually overshoots slightly to reach the setpoint more quickly; 
 * however, some systems cannot accept overshoot, 
 * in which case an "over-damped" closed-loop system is required, which will require a Kp setting significantly less than half that of the Kp setting causing oscillation.
 * PIDTuningClassical : http://web.archive.org/web/20080616062648/http://controls.engin.umich.edu:80/wiki/index.php/PIDTuningClassical#Ziegler-Nichols_Method
 * Optimum Settings for Automatic Controllers : http://staff.guilan.ac.ir/staff/users/chaibakhsh/fckeditor_repo/file/documents/Optimum%20Settings%20for%20Automatic%20Controllers%20(Ziegler%20and%20Nichols,%201942).pdf
 */
//PID程式碼***********************************************************
//PID 應用於Encoder上，因為環境參數時時在改變，難以抓其值
void PID_Calculation(int setpoint){                    //Encoder Line    2檔PID
 static unsigned long lastTime;                        // setpoint : point you want 
 float dInput, ITerm, Error;
 static float lastInput;
 int adjust;
 
if(millis()  - lastTime  >  20)       //run per 50ms 
{
  lastTime = millis();  
  Input = -level_L  - level_R * 1.0756;  //正值:右輪加速     //負值:左輪加速 1.0261
     Error = Input - setpoint;
     ITerm  += (Ki * Error);
     dInput = (Input - lastInput);
  Output = (Kp * Error) + ITerm - (Kd * dInput);     //Compute PID Output
  adjust = Output;
  if(adjust > 30) {adjust = 30;}
  else if(adjust < -30) {adjust = -30;}
      lastInput = Input;     
      
if(Input > 0){          //左輪比較快
  setMotorLeft(BaseL2 - adjust, true);
  setMotorRight(BaseR2 + adjust, true); 
  Checklight(0, 1, 0);
}

else if(Input < 0){         //右輪比較快
  setMotorLeft(BaseL2 - adjust, true);
  setMotorRight(BaseR2 + adjust, true); 
  Checklight(1, 0, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error);
      Serial.print(" ,OUT=");        Serial.print(Output);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
 */     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
}
}
//PID程式碼***********************************************************
void PID_Calculation2(int setpoint2){                    //Encoder Line    4檔PID
 static unsigned long lastTime2;                      // setpoint : point you want 
 float dInput2, ITerm2, Error2;
 static float lastInput2;
 int adjust2;

if(millis()  - lastTime2  >  20)       //run per 50ms 
{
  lastTime2 = millis();  
  Input2 = -level_L  - level_R * 1.07;  //正值:右輪加速     //負值:左輪加速 1.035
     Error2 = Input2 - setpoint2;
     ITerm2  += (Ki2 * Error2);
     dInput2 = (Input2 - lastInput2);
  Output2 = (Kp2 * Error2) + ITerm2 - (Kd2 * dInput2);     //Compute PID Output
  adjust2 = Output2;
  if(adjust2 > 40) {adjust2 = 40;}
  else if(adjust2 < -40) {adjust2 = -40;}
      lastInput2 = Input2;     
      
if(Input2 > 0){          //左輪比較快
  setMotorLeft(BaseL4 - adjust2, true);
  setMotorRight(BaseR4 + adjust2, true); 
  Checklight(0, 1, 0);
}

else if(Input2 < 0){         //右輪比較快
  setMotorLeft(BaseL4 - adjust2, true);
  setMotorRight(BaseR4 + adjust2, true); 
  Checklight(1, 0, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error2);
      Serial.print(" ,OUT=");        Serial.print(Output2);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
 */     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
}
}
//PID3程式碼***********************************************************
void PID_Calculation3(int setpoint2){                    //Encoder Line    5檔PID
 static unsigned long lastTime3;                      // setpoint : point you want 
 float dInput3, ITerm3, Error3;
 static float lastInput3;
 int adjust3;
 
if(millis()  - lastTime3  >  20)       //run per 50ms 
{
  lastTime3 = millis();  
  Input3 = -level_L  - level_R * 1.05;  //正值:右輪加速     //負值:左輪加速
     Error3 = Input3 - setpoint2;
     ITerm3  += (Ki3 * Error3);
     dInput3 = (Input3 - lastInput3);
  Output3 = (Kp3 * Error3) + ITerm3 - (Kd3 * dInput3);     //Compute PID Output
  adjust3 = Output3;
  if(adjust3 > 40) {adjust3 = 40;}
  else if(adjust3 < -40) {adjust3 = -40;}
      lastInput3 = Input3;     
      
if(Input3 > 0){          //左輪比較快
  setMotorLeft(BaseL5 - adjust3, true);
  setMotorRight(BaseR5 + adjust3, true); 
  Checklight(0, 1, 0);
}

else if(Input3 < 0){         //右輪比較快
  setMotorLeft(BaseL5 - adjust3, true);
  setMotorRight(BaseR5 + adjust3, true); 
  Checklight(1, 0, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error3);
      Serial.print(" ,OUT=");        Serial.print(Output3);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
 */     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
}
}
//*****************************************************************************************************************************************************************//
//PID程_GY521式碼***********************************************************
void PID_GY521_Calculation(int setpoint){                    //GY521 Line   
 static unsigned long lastTime;                        // setpoint : point you want 
 float dInput, ITerm, Error;
 static float lastInput;
 int adjust;
 int ANS_PL, ANS_ML;
 int ANS_PR, ANS_MR;
 theta = Devtheta(CaseGY521);
if(millis()  - lastTime  >  20)       //run per 20ms 
{
  lastTime = millis();  
  Input4 = theta;  //正值:右輪加速   
     Error = Input4 - setpoint;
     ITerm  += (Ki4 * Error);
     dInput = (Input4 - lastInput);
  Output4 = (Kp4 * Error) + ITerm - (Kd4 * dInput);     //Compute PID Output
  adjust = Output4;
  if(adjust > 50) {adjust = 50;}
  else if(adjust < -50) {adjust = -50;}
      lastInput = Input4;     
     
if(Error > 0){         //左偏
  ANS_PL = BaseL3 + abs(adjust); if(ANS_PL > 150) {ANS_PL = 150;}
  ANS_MR = BaseR3 - abs(adjust); if(ANS_MR < 40) {ANS_MR = 40;}
  setMotorLeft(ANS_PL, true);
  setMotorRight(ANS_MR, true); 
  Checklight(1, 0, 0);
}

else if(Error < 0){    //右偏
  ANS_ML = BaseL3 - abs(adjust); if(ANS_ML < 40) {ANS_ML = 40;}
  ANS_PR = BaseR3 + abs(adjust); if(ANS_PR > 150) {ANS_PR = 150;}
  setMotorLeft(ANS_ML, true);        //adjust < 0
  setMotorRight(ANS_PR, true); 
  Checklight(0, 1, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error);
      Serial.print(" ,OUT=");        Serial.print(Output);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
*/     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
thetaprint();
}
}
//PID程_GY521式碼***********************************************************
void PID_GY521_Calculation2(int setpoint){                 //GY521 Line   
 static unsigned long lastTime;                        // setpoint : point you want 
 float dInput, ITerm, Error;
 static float lastInput;
 int adjust;
 int ANS_PL, ANS_ML;
 int ANS_PR, ANS_MR;
 theta = Devtheta(CaseGY521);
if(millis()  - lastTime  >  10)       //run per 10ms 
{
  lastTime = millis();  
  Input5 = theta;  //正值:右輪加速   
     Error = Input5 - setpoint;
     ITerm  += (Ki5 * Error);
     dInput = (Input5 - lastInput);
  Output5 = (Kp5 * Error) + ITerm - (Kd5 * dInput);     //Compute PID Output
  adjust = Output5;
  if(adjust > 70) {adjust = 70;}
  else if(adjust < -70) {adjust = -70;}
      lastInput = Input4;     

      
if(Error > 0){         //左偏
  ANS_PL = BaseL5 + abs(adjust); if(ANS_PL > 200) {ANS_PL = 200;}
  ANS_MR = BaseR5 - abs(adjust); if(ANS_MR < 60) {ANS_MR = 60;}
  setMotorLeft(ANS_PL, true);
  setMotorRight(ANS_MR, true); 
  Checklight(1, 0, 0);
}

else if(Error < 0){    //右偏
  ANS_ML = BaseL5 - abs(adjust); if(ANS_ML < 60) {ANS_ML = 60;}
  ANS_PR = BaseR5 + abs(adjust); if(ANS_PR > 200) {ANS_PR = 200;}
  setMotorLeft(ANS_ML, true);        //adjust < 0
  setMotorRight(ANS_PR, true); 
  Checklight(0, 1, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error);
      Serial.print(" ,OUT=");        Serial.print(Output);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
*/     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
thetaprint();
}
}
//PID程_GY521式碼***********************************************************
void PID_GY521_Calculation3(int setpoint){                    //GY521 Line   
 static unsigned long lastTime;                        // setpoint : point you want 
 float dInput, ITerm, Error, lastInput;
 int adjust;
 int ANS_P, ANS_M;
 theta = Devtheta(CaseGY521);
if(millis()  - lastTime  >  20)       //run per 20ms 
{
  lastTime = millis();  
  Input6 = theta;  //正值:右輪加速   
     Error = Input6 - setpoint;
     ITerm  += (Ki6 * Error);
     dInput = (Input6 - lastInput);
  Output6 = (Kp6 * Error) + ITerm - (Kd6 * dInput);     //Compute PID Output
  adjust = Output6;
  if(adjust > 50) {adjust = 50;}
  else if(adjust < -50) {adjust = -50;}
      lastInput = Input6;    
       
ANS_P = BaseL2 + abs(adjust); if(ANS_P > 150) {ANS_P = 150;}
ANS_M = BaseR2 - abs(adjust); if(ANS_M < 40) {ANS_P = 40;}

if(Error > 0){         //左偏
  setMotorLeft(ANS_P, true);
  setMotorRight(ANS_M, true); 
  Checklight(1, 0, 0);
}

else if(Error < 0){    //右偏
  setMotorLeft(ANS_M, true);        //adjust < 0
  setMotorRight(ANS_P, true); 
  Checklight(0, 1, 0);
}
/*
      Serial.print(" ,L=");        Serial.print(-level_L);
      Serial.print(" ,R=");        Serial.print(level_R);
      Serial.print(" ,Err=");        Serial.print(Error);
      Serial.print(" ,OUT=");        Serial.print(Output);
      Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
      Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
*/     
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
thetaprint();
}
}
//*****************************************************************************************

