//主要Case撰寫處
//******************************************************************************************************************************************************  
void Case_61(){
  float disI = 0, disF = 0;  
  int DisPlant;
                  lcd.clear();         Checklight(1, 1, 1); //燈全亮
                  lcd.setCursor(0,0); lcd.print("Enter flag value...."); lcd.setCursor(0,1);
                  flag = Enterkeypad(); lcd.clear();
                  if(flag == 0) {DisPlant = 180;}
                  else if(flag == 1) {DisPlant = 320;}
                  else if(flag == 2) {DisPlant = 450;}
                  else {DisPlant = 650;}
                  Pulse_RESET();
                  check = 1;        //澆水一定要第一顆打到，才會澆水
               disI = Getdis();
               disF = Getdis();
               motorStart(BaseL3, BaseR3, true, true);
               while((disF - disI) < DisPlant && flag_failure == false){   
                disF = Getdis();
                PID_GY521_Calculation(0);
                emergency();              //緊急按鍵
               }
               while(flag != 3 && flag_failure == false){                                                              
                    Ultrasonic_Watering();          
                    emergency();              //緊急按鍵          
                 }                                 
                  lcd.clear();
                  disI = Getdis();
                  disF = Getdis(); 
                  Ultra_distanceF(); 
               while((distanceF > 40 || (disF - disI) < 85) && flag_failure == false){                
                disF = Getdis();
                Blink_RGB_lightR(0,1,0);
                PID_GY521_Calculation(3);
                Ultra_distanceF();    
                emergency();              //緊急按鍵          
               }
                  motorBrake(0, 0, true, true);
                 if(flag_failure == false){
                  motor_spinAround(BaseL4, BaseR4, -85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
                  flag_GY521 = 2;
                  motorStart(BaseL3, BaseR3, true, true);
                 }
                  disI = Getdis();
                  disF = Getdis(); 
                  Ultra_distanceF();
               while((distanceF > 37 || (disF - disI) < 150) && flag_failure == false){
                disF = Getdis();
                Blink_RGB_lightR(0,1,0);
                PID_GY521_Calculation(-90);
                Ultra_distanceF();
                emergency();              //緊急按鍵
               }
                  motorBrake(0, 0, true, true);
                 if(flag_failure == false){
                    motor_spinAround(BaseL4, BaseR4, -180, -180);
                    flag_GY521 = 0;
                    motorStart(BaseL3, BaseR3, true, true);
                  }
                  
}
//****************************************************************************************************************************************************** 
//**********************************************************
void Case_62(){
  float disI = 0, disF = 0;  
                  lcd.clear(); Checklight(1, 1, 1); //燈全亮  
                  Pulse_RESET();
                  disI = Getdis();
                  disF = Getdis();
                  Ultra_distanceR1();
                  if(flag_failure == false) {motorStart(BaseL5, BaseR5, true, true);}
               while(((disF - disI) < 265 || distanceR1 < 20) && flag_failure == false){
                  disF = Getdis();
                  Ultra_distanceR1();
                  PID_GY521_Calculation2(0);
                  emergency();              //緊急按鍵
               } 
                motorBrake(BaseL3, BaseR3, true, true);
               while(distanceR1 > 30 && flag_failure == false){
                  Ultra_distanceR1();
                  PID_GY521_Calculation(0);
                  emergency();              //緊急按鍵
               } 
               disI = Getdis();
               disF = Getdis();
              while((distanceL1 < 23 || (disF - disI) < 160) && flag_failure == false){              //第一個窄彎
                   Ultra_distanceL1();    
                   theta = Devtheta(CaseGY521);
                   disF = Getdis();               
                   if((disF - disI) < 60) {Ultrasonic_AvoidingR1();}
                   else if(((disF - disI) >= 60) && ((disF - disI) < 165)) {Ultrasonic_AvoidingR2();}
                   else if((disF - disI) > 165) {break;}
                   emergency();              //緊急按鍵
               }
                Checklight(1, 1, 1);
                disI = Getdis();
                disF = Getdis();
              while(flag_failure == false){             //無窮迴圈     第二個窄彎   
                theta = Devtheta(CaseGY521);
                disF = Getdis(); 
                 if((disF - disI) < 60) {Ultrasonic_AvoidingL1();}
                 else {Ultrasonic_AvoidingL2();}
                 if((disF - disI) > 100) {Blink_RGB_lightL(0,1,0);}       //左轉方向燈
                                  if((disF - disI) > 170 && distanceL1 > 60) {Rundis(20, BaseL2, BaseR2);
                                                                              motor_spinAround(BaseL3, BaseR3, 80, 0);
                                                                              flag_GY521 = 1;
                                                                              break;}
                    emergency();              //緊急按鍵
               }
                   if(flag_failure == false) {motorStart(BaseL3, BaseR3, true, true);}  
                    Ultra_distanceF();
                    disI = Getdis();
                    disF = Getdis();
                while((distanceF > 40 || (disF - disI) < 120) && flag_failure == false ){Checklight(1, 1, 1);               //偵測小於30就轉彎 
                    disF = Getdis();
                    Blink_RGB_lightL(0,1,0);
                    Ultra_distanceF();
                    PID_GY521_Calculation(90); 
                    emergency();              //緊急按鍵  
                }
                  motorBrake(0, 0, true, true);
                  if(flag_failure == false){
                    motor_spinAround(BaseL4, BaseR4, 180, 180);
                    flag_GY521 = 0;      
                    motorStart(BaseL3, BaseR3, true, true);
                  }
}
//****************************************************************************************************************************************************** 
//**********************************************************
void Case_63(){
  float disI = 0, disF = 0;    
                lcd.clear(); Checklight(1, 1, 1); //燈全亮
                if(flag_failure == false) {motorStart(BaseL3, BaseR3, true, true);}  
                disI = Getdis();
                disF = Getdis();
               while((disF - disI) < 350 && flag_failure == false){
                disF = Getdis();    
                PID_GY521_Calculation(0);
                emergency();              //緊急按鍵
               }
                disI = Getdis();
                disF = Getdis();
               while((disF - disI) < 290 && flag_failure == false){
                PID_GY521_Calculation(0);
                emergency();              //緊急按鍵
                disF = Getdis();
                  if((disF - disI) > 100){         //右轉方向燈
                    Blink_RGB_lightR(0,1,0);
                  }       
               }
                Ultra_distanceF();
                disI = Getdis();
                disF = Getdis();
               while(distanceF > 40 && flag_failure == false){
                Ultra_distanceF();
                PID_GY521_Calculation(0);
                emergency();              //緊急按鍵
                disF = Getdis();
                Blink_RGB_lightR(0,1,0);                   
               }
                 motorBrake(0, 0, true, true);
              if(flag_failure == false){
                 motor_spinAround(BaseL4, BaseR4, -85, 0);
                 flag_GY521 = 2;
                 motorStart(BaseL3, BaseR3, true, true);
              } 
                 disI = Getdis();
                 disF = Getdis();
              Ultra_distanceF();
              while((distanceF > 37 || (disF - disI) < 100) && flag_failure == false){
                Blink_RGB_lightR(0,1,0);
                disF = Getdis();
                Ultra_distanceF();
                PID_GY521_Calculation(-90);
                emergency();              //緊急按鍵
              }
                motorBrake(0, 0, true, true);
                if(flag_failure == false){
                  motor_spinAround(BaseL5, BaseR5, -170, -180);
                  flag_GY521 = 0;
                  motorStart(BaseL3, BaseR3, true, true);
                }
}
//**********************************************************
void Case_64(){
  float disI = 0, disF = 0;  
  int A;
                  lcd.clear();         Checklight(1, 1, 1); //燈全亮  
                  Pulse_RESET();
                  flag_GY521 = 0;
                  if(flag_failure == false) {motorStart(BaseL3, BaseR3, true, true);}        //起步
                  disI = Getdis();
                  disF = Getdis();
               while((disF - disI) < 160 && flag_failure == false){
                disF = Getdis();
                PID_GY521_Calculation(0);
                emergency();              //緊急按鍵
               }
                Pulse_RESET();
                 motorBrake(BaseL3, BaseR3, true, true);
                  disI = Getdis();
                  disF = Getdis();
               while(flag2 != 2 && flag_failure == false){
                  PID_GY521_Calculation(0);
                  emergency();              //緊急按鍵
                  disF = Getdis();
                  Ultrasonic_CatchUp(); 
                 }                      
                 flag2 = 0;               //RESET flag
                 disI = Getdis();
                 disF = Getdis();
                if(flag_failure == false) {motorStart(BaseL3, BaseR3, true, true);}
               while(flag2 != 2 && flag_failure == false){
                disF = Getdis();
                emergency();              //緊急按鍵
                  if((disF - disI) > 160){       
                    PID_GY521_Calculation(0);
                    Ultrasonic_CatchDown(); 
                  }          
                  else{
                    PID_GY521_Calculation(0);
                  }
               }
                  flag2 = 0;
                  lcd.clear();  
                  if(flag_failure == false) {motorStart(BaseL5, BaseR5, true, true);}
                  Ultra_distanceF();          //前面的超音波     
                 disI = Getdis();
                 disF = Getdis();        
                while((distanceF > 45 || (disF - disI) < 220) && flag_failure == false){Checklight(1, 1, 1);               //偵測小於45就停止   
                   PID_GY521_Calculation2(0); 
                   disF = Getdis();
                   Ultra_distanceF();  
                   emergency();              //緊急按鍵         
                 }
                  motorBrake(0, 0,  true, true);  Delay(200);
                  flag_failure = false;    
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("Mission Complete"); lcd.setCursor(0,1); lcd.print("!!!!!");  
                  
}
//****************************************************************************************************************************************************** 
//****************************************************************************************************************************************************** 
//****************************************************************************************************************************************************** 
void case_992(){    //直走多少距離超音波停止 高速檔
  int disI, disF; 
                  disI = Getdis();
                  disF = Getdis();
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL5, BaseR5, true, true);  
                 while((distanceF > 45 || (disF - disI) < 560) && flag_failure == false){
                   Ultra_distanceF();     //前面的超音波
                   disF = Getdis();
                   PID_GY521_Calculation2(0);
                   emergency();              //緊急按鍵     
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
}
//****************************************************************************************************************************************************** 
void case_993(){    //直走多少距離超音波停止 低速檔
  int disI, disF; 
                  disI = Getdis();
                  disF = Getdis();
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL5, BaseR5, true, true);  
                 while((distanceF > 45 || (disF - disI) < 560) && flag_failure == false){
                   Ultra_distanceF();     //前面的超音波
                   disF = Getdis();
                   PID_GY521_Calculation(0);
                   emergency();              //緊急按鍵     
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
}
//****************************************************************************************************************************************************** 
void case_994(){    //直走多少距離後後右轉
  int disI, disF, Dis_you_want;
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Enter dis...R");
  Dis_you_want = Enterkeypad();
  disI = Getdis();
  disF = Getdis();
  Ultra_distanceF();
  while((disF - disI) < Dis_you_want){   
   Blink_RGB_lightR(0,1,0);
   disF = Getdis();
   PID_GY521_Calculation(0);
   }
   motorBrake(0,0,true,true);
   motor_spinAround(BaseL4, BaseR4, -85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
   motorStart(BaseL3,BaseR3, true,true);
}
//*********************************************************************************
void case_995(){    //直走多少距離後左轉
  int disI, disF, Dis_you_want;
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Enter dis...L");
  Ultra_distanceF();
  while((disF - disI) < Dis_you_want){   
   Blink_RGB_lightL(0,1,0);
   disF = Getdis();
   PID_GY521_Calculation(0);
   }
   motorBrake(0,0,true,true);
   motor_spinAround(BaseL4, BaseR4, 85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
   motorStart(BaseL3,BaseR3, true,true);
}
//*********************************************************************************
void case_996(){    //直走後右轉
  Ultra_distanceF();
  while(distanceF > 35){   
   Blink_RGB_lightR(0,1,0);
   PID_GY521_Calculation(0);
   Ultra_distanceF();
   }
   motorBrake(0,0,true,true);
   motor_spinAround(BaseL4, BaseR4, -85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
   motorStart(BaseL3,BaseR3, true,true);
}
//*********************************************************************************
void case_997(){      //直走後左轉
   Ultra_distanceF();
   while(distanceF > 35){   
    PID_GY521_Calculation(0);
    Blink_RGB_lightL(0,1,0);
    Ultra_distanceF();
    }
    motorBrake(0,0,true,true);
    motor_spinAround(BaseL4, BaseR4, 85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
    motorStart(BaseL3,BaseR3, true,true);
}
//*********************************************************************************
void case_998(){      //直走後走一段距離右轉
  float disI, disF;
   disI = Getdis();
   disF = Getdis();
   Ultra_distanceF();
  while((distanceF > 35 || (disF - disI) < 155) ){
   Blink_RGB_lightR(0,1,0);
   disF = Getdis();
   Ultra_distanceF();
   PID_GY521_Calculation(0);
  }
    motorBrake(0,0,true,true);
    motor_spinAround(BaseL4, BaseR4, -85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
    motorStart(BaseL3,BaseR3, true,true);
}
//*********************************************************************************
void case_999(){      //直走後走一段距離右轉
  float disI, disF;
   disI = Getdis();
   disF = Getdis();
   Ultra_distanceF();
  while((distanceF > 35 || (disF - disI) < 155) ){
   Blink_RGB_lightR(0,1,0);
   disF = Getdis();
   Ultra_distanceF();
   PID_GY521_Calculation(0);
  }
    motorBrake(0,0,true,true);
    motor_spinAround(BaseL4, BaseR4, 85, 0);                  //void motor_spinAround(int speedL,int speedR,float alpha)
    motorStart(BaseL3,BaseR3, true,true);
}
//****************************************************************************************************************************************************** 
//****************************************************************************************************************************************************** 
void emergency(){                 //緊急按鈕
  char key;
  int F;
  key = keypad.getKey();
  if(key == '#') {
                  motorBrake(0, 0, true, true);
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("1-->break"); lcd.setCursor(0,15);
                               lcd.setCursor(0,1); lcd.print("0-->continue.");
                  F = Enterkeypad();
                   switch(F){
                    case 1:
                     motorBrake(0, 0, true, true);
                     flag_failure = true; 
                     lcd.clear();  
                    break;                 
                    case 0:
                      lcd.clear();
                    break;
                    default:
                       lcd.clear(); lcd.setCursor(0,0); lcd.print("Error......."); Delay(1000);
                       lcd.clear();
                    break;
                    }

  }
}
//****************************************************************************************************************************************************** 
//****************************************************************************************************************************************************** 
