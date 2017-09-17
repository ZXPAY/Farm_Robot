//*****************************************************************************************************************************************************************//
 /*LCD規格
* 產品規格：
*型號：LCM I2C 1602 V2-B
*尺寸：80*36*19mm
*液晶屏：1602字符型（HD44780） 
*顏色：藍底白字 
*電壓：5V
*端口：I2C
*地址：0x20-0x27（默認0x27）
*平台：Arduino
 */ 
//matrix keypad library下載: http://playground.arduino.cc/Code/Keypad
//按鍵接腳為:colume1:Pin2   colume2:Pin3   colume3Pin:4   colume4:Pin5   colume5:Pin6   colume6:Pin7   colume7:Pin8   colume8:Pin9
//Main code

//******************************************************************************************************************************************************
#include <Wire.h>  // Arduino IDE 內建
#include <LiquidCrystal_I2C.h>   //液晶螢幕宣告
LiquidCrystal_I2C lcd(0x27,16,2);  // 設定 LCD I2C 位址
#include <Keypad.h>     //按鍵keypad程式庫

//全域變數宣告.......

//臨時按鍵flag      非中斷子
 int flag_failure = 0;

//電子羅盤修正
  float theta2;
  int Devstate, CW, CCW;                       //initial angle
  float Cib;                           //校正角度
  float devtheta2;                      //差值(角度變化)    devtheta = theta2  - theta2i     負:逆時針     正:順時針\

//Watering Check
  int check = 0;
  int flag = 0;
  int flag_Water = 0;
  float dis_WI = 0;
  float dis_WF = 0;

//Catching Check
  int flag2 = 0;

//GY521陀螺儀宣告
  int flag_GY521 = 0;   
  float roll = 0, pitch = 0, theta = 0, thetaR = 0;
  float thetai = 0;
  int CaseGY521 = 0;
  float Tmp = 28;
  float error2,  gyerror;
  float dev;
  float Max, Min;
  const int DMP6_LED = 52;
  
//Keypad宣告
  const byte ROWS=4;    //定義4行
  const byte COLS=4;    //定義4列
  char keys[ROWS][COLS]={
    {'1','2','3','A'},    
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'},
  };
  byte rowPins[ROWS] = {26,27,28,29};
  byte colPins[COLS] = {22,23,24,25};
 Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//Ultrasonic宣告***********
  float distanceR1,  distanceR2,  distanceR3,  distanceF,  distanceL1;
        //右邊第一顆    右邊第二顆    右邊第三顆      前 面       左 邊
  float Ultra1_min = 150, Ultra3_min = 150;                       
  float dev_angle;
  unsigned long now = 0;

//Encoder宣告***********
  volatile byte state = 0;           // will store two bits for pins A & B on the encoder which we will get from the pins above
  int level_L = 0;            // a value bumped up or down by the encoder
  int level_R = 0;            // a value bumped up or down by the encoder
  int D;
  float situationA;           //走了多少圈
  float situationB;           //走了多少圈
  float Encoder_speedL , Encoder_speedR;
  String bits[] = {"00","01","10","11"};
  int bump[] = {0,0,-1,1};
  int Com;
  float dis_I, dis_F, Enc_dev;

//馬達宣告***********
  int LeftSpeed,  RightSpeed;
  const int BaseL1 = 47, BaseR1 = 35;    // 1檔 低速檔      //基速   
  const int BaseL2 = 65, BaseR2 = 53;    // 2檔                  
  const int BaseL3 = 82, BaseR3 = 58;    // 3檔                  
  const int BaseL4 = 102, BaseR4 = 78;    // 4檔                    
  const int BaseL5 = 150, BaseR5 = 89;    // 5檔 高速檔              

//伺服馬達宣告***********
  int posiL = 100, posiR = 40, posiUp_Down = 5;    //Servo Motor Initial Position
  float Ser_dev;   // case 4 --> Catch potting

//PID宣告**************
  float Setpoint, Input, Output;       //Define Variables we'll be connecting to
  float Setpoint2, Input2, Output2;       //Define Variables we'll be connecting to
  float Setpoint3, Input3, Output3;       //Define Variables we'll be connecting to
  float Kp=0.45, Ki=1.85, Kd=0;              //Specify the links and initial tuning parameters
  float Kp2=2.0, Ki2=2.4, Kd2=0;           //Specify the links and initial tuning parameters   
  float Kp3=4.7, Ki3=1.5, Kd3=0;           //Specify the links and initial tuning parameters        
  float Setpoint4, Input4, Output4;       //Define Variables we'll be connecting to
  float Setpoint5, Input5, Output5;       //Define Variables we'll be connecting to
  float Setpoint6, Input6, Output6;       //Define Variables we'll be connecting to
  float Kp4=23.3, Ki4=0, Kd4=0;           //Specify the links and initial tuning parameters
  float Kp5=40, Ki5=0, Kd5=0;           //Specify the links and initial tuning parameters
  float Kp6=15, Ki6=0, Kd6=0;           //Specify the links and initial tuning parameters
  float Setpoint_Ultra1, Input_Ultra1, Output_Ultra1;       //Define Variables we'll be connecting to
  float Setpoint_Ultra2, Input_Ultra2, Output_Ultra2;       //Define Variables we'll be connecting to
  float Kp_Ultra1=5, Ki_Ultra1=0, Kd_Ultra1=0;           //Specify the links and initial tuning parameters
  float Kp_Ultra2=5, Ki_Ultra2=0, Kd_Ultra2=0;           //Specify the links and initial tuning parameters
  
//紀錄*********** 
  float Record1[100];
  float Record2[100];
  float Record3[100];
  int Num1 = 0;
  int Num2 = 0;
  int Num3 = 0;
  float a;

//*****************************************************************************************************************************************************
void setup() {
//Initialiing.......
//Communiction
  Serial.begin(115200);                   //Computer
  
  Encoder_setup();          //Encoder
  Motor_setup();            //Motor
  Ultrasonic_setup();       //Ultrasonic
  //HMC5883L_setup();        //Compass
  GY521_setup();              //Gyroscope
  Servo_setup();              //Servo motor
  Watering_setup();           //Relay 
  LED_Setup();                //LED
  
//LCD
  lcd.begin();      // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光
  lcd.backlight();    // 輸出初始化文字

/*Kalman Cilibration ---> is not complete
error2 = CalibrateZ();
Serial.print("error2=");Serial.print(error2,DEC);
Serial.print(",gyerror=");Serial.println(gyerror,DEC);
*/
CaseGY521 = Initial_GY521();        //DMP6 MPU6050--GY521  Initializing......
//Serial.print("thetai = ");
//Serial.println(thetai);
Checklight_blink();                  //Check Led blink

}
//*****************************************************************************************************************************************************
//主程式碼
void loop() {
//區域變數宣告...............
  int A = 0, E = 0, L = 0, R = 0, Up = 0;                      // E : Encoder Feedback
  float B = 0, C = 0, D = 0;                   //Enterkeypad 
  float disI = 0, disF = 0;            //Encoder run distance     
  float RollI = 0;                     
  float speedRL = 0, speedRR = 0;
  int Time = 0;                //Case 3

/*
Ultra_distanceR1();
Ultra_distanceR2();
Ultra_distanceR3();
Ultra_distanceF();
Ultra_distanceL1();
*/
 
//getGY521();
theta = Devtheta(CaseGY521);
//Serial.println(theta);
/*  
speedRL = encoder_speedL(1);
speedRR = encoder_speedR(1);  
lcd.setCursor(0,1); lcd.print(speedRL); lcd.print("   ");
lcd.setCursor(8,1); lcd.print(speedRR); lcd.print("   ");
Serial.print(speedRL);   Serial.print("  ");
Serial.println(speedRR);
*/
  /*
  s = theta - a - gyerror*n;
  n++;
  Serial.print(" thetai=");
  Serial.print(theta);
  Serial.print(" NEW=");
  Serial.println(s);
  */
  /*
  static int n  = 100;
  theta = gy521.getAngle(Yaw);
  theta = gy521.getAngle(Yaw) - error - n*gyerror;
   Serial.print(n);
   Serial.print(",");
   n++;
  Serial.println(theta);
  */
//float a = Compass();
//Serial.print(a);
//Serial.print(" , ");
//devtheta2 = Devtheta2(Devstate);
//Serial.println(devtheta2);
//theta2print();
// Record_Theta(devtheta2);
//Serial.println(devtheta2);
//Speedprint(LeftSpeed, RightSpeed);
//Serial.print(" ");
//Ultraprint();
//Encoder_speedL  = encoder_speedL(0);        //計算現在多少RPM
//Encoder_speedR  = encoder_speedR(0);       //計算現在多少RPM
//Encoder_Speedprint(Encoder_speedL, Encoder_speedR);
//Checklight(1, 1, 1);                 //Led亮燈，   Checkligh(紅 , 黃 , 綠);

char key;
key = keypad.getKey();
switch (key){
  //************
  case '1':    //前進+修正        //ASCII 字元碼char to DEX 為49
  lcd.clear();
  lcd.print("Enter Case...");            //Three Numbers
  A = Enterkeypad();
 switch(A){
           case 10:              //測試基速
                  lcd.clear();
                  lcd.print("Enter LeftSpeed..."); 
                  lcd.setCursor(0,1);
                  L = Enterkeypad();   //Serial.print("L= ");  Serial.println(L);   
                  lcd.clear(); lcd.print("Enter RightSpeed..."); 
                  lcd.setCursor(0,1);
                  R = Enterkeypad();   //Serial.print("R= ");  Serial.println(R);   lcd.clear();
                  motorStart(L, R, true, true); Checklight_blink();  
           break;
                       
           case 11:              //測試走距離函式
                  lcd.clear();
                  lcd.print("Enter Distance...(cm)"); 
                  lcd.setCursor(0,1);
                   A = Enterkeypad();  lcd.clear();
                  Rundis(A, BaseL2, BaseR2);      // Rundis(int D, int speedL, int speedR, boolean a, boolean b);
                  if(A > 0) {motorBrake(0, 0, true, true);}
                  else if(A < 0) {motorBrake(0, 0, false, false);}
                  Checklight_blink();   
           break;
           
           case 12:   //測試轉彎
                  lcd.clear();
                  lcd.print("SpinAround..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   //Serial.print("A= ");  Serial.println(A);
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                   motor_spinAround(BaseL4, BaseR4, -90, -90);             //先右轉90度  
                   motor_spinAround(BaseL4, BaseR4, 90, 90);            //再左轉90度
                   Checklight_blink();   
           break;
           /*
            case 19:          //encoder feedback
                  lcd.clear();
                  lcd.print("Encoder Feedback..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();  
                  if(A == 0) {lcd.clear(); break;}  else{lcd.clear();}
                      Checklight(0, 0, 1);
                     motorStart(BaseL2, BaseR2, true, true);  
                     Delay(100);
                      E = Encoder_feedback(BaseL2, BaseR2);
                    switch(E){
                            case  1:
                                    Checklight(1, 0, 0);
                                    LeftSpeed = LeftSpeed - Com;          if(LeftSpeed <  0)  {LeftSpeed  = 0;}
                                    RightSpeed  = RightSpeed;
                                    motorStart(LeftSpeed,RightSpeed, true, true);
                                     Serial.print("Com=");
                                    Serial.println(Com);                                   
                            break;

                            case  2:
                                    Checklight(0, 1, 0);
                                    LeftSpeed = LeftSpeed;
                                    RightSpeed  = RightSpeed  - Com;              if(RightSpeed <  0)  {RightSpeed  = 0;}
                                    motorStart(LeftSpeed,RightSpeed, true, true);
                                   
                            break;
                      }
                    thetaprint();   speedprint(); Checklight_blink();   
           break;
          */

           case 101:          //澆水幾秒停
                  lcd.clear();
                  lcd.print("Watering seconds..."); 
                  lcd.setCursor(0,1);
                  A = Enterkeypad();     lcd.clear();
                  Watering(A); Checklight_blink();   
           break;
           
           case 20:        //測試第一個彎
                  lcd.clear(); Checklight(1, 1, 1); //燈全亮  
                  Ultra_distanceF();          //前面的超音波
                  motorStart(BaseL2, BaseR2, true, true);  
               while(1 == 1){
               theta = Devtheta(CaseGY521);
                Ultra_distanceR1();
                PID_Calculation2(0);                 
                if(distanceR1 < 20) {break;}
               }
              Ultra_distanceF();
               disI = Getdis();
               disF = Getdis();
              while(1 == 1){              //無窮迴圈     第一個窄彎
                   theta = Devtheta(CaseGY521);   
                   Ultra_distanceL1();    
                   disF = Getdis();       lcd.setCursor(8,1);lcd.print(disF - disI);lcd.print("  ");            
                   if(disF - disI < 50) {Ultrasonic_AvoidingR1();}
                   else if((disF - disI >= 50) && (disF - disI < 130)) {Ultrasonic_AvoidingR2();}
                        
                   if(distanceL1 < 35 && (disF - disI) > 130) {Checklight(1, 1, 1);                      
                                                                break;}
               }
                   motorBrake(0, 0,  true, true);     Checklight_blink();   
           break;

           case 21:
                lcd.clear(); Checklight(1, 1, 1); //燈全亮
                motorStart(BaseL2, BaseR2, true, true);  
                disI = Getdis();
                disF = Getdis();
              while(1 == 1){             //無窮迴圈     第二個窄彎   
                Ultra_distanceF();
                theta = Devtheta(CaseGY521);
                disF = Getdis();      lcd.setCursor(8,1);lcd.print(disF - disI);lcd.print("  ");
                 Ultrasonic_AvoidingL2();
                 if(disF - disI > 150 && distanceL1 > 80) {Checklight(1, 1, 1);Rundis(10, BaseL2, BaseR2);
                                                            motor_spinAround(BaseL4, BaseR4, 90, 90);
                                                            break;}
               }
                    motorStart(BaseL2, BaseR2, true, true);  
                while(distanceF > 40){Checklight(1, 1, 1);               //偵測小於30就轉彎 
                    Ultra_distanceF();
                    PID_Calculation2(0);   
                }
                  Delay(100);
                  motor_spinAround(BaseL4, BaseR4, 90, 90);
                  Ultra_distanceF();       
                  motorStart(BaseL3, BaseR3, true, true);            
               while(distanceF > 30){Checklight(1, 1, 1);               //偵測小於30就停止   
                    Ultra_distanceF();
                    PID_Calculation2(0);
                }
                   motorBrake(0, 0,  true, true);  Delay(200);     Checklight_blink();     
           break;

           case 22:
                  lcd.clear();
                  lcd.print("Ultra_Declination..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad(); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                 Ultra_distanceF();
                 Ultrasonic_RESET();
                 while(distanceF > 20){
                  Ultrasonic_Detect();
                  Ultra_distanceF();
                  lcd.setCursor(0,0); lcd.print("R1="); lcd.print(distanceR1);
                  lcd.setCursor(8,0); lcd.print("R3="); lcd.print(distanceR3);
                 }
                  Ultrasonic_Cal();
                 lcd.clear(); lcd.setCursor(0,0); lcd.print("Min1="); lcd.print(Ultra1_min);
                              lcd.setCursor(8,0); lcd.print("Min3="); lcd.print(Ultra3_min);
                              lcd.setCursor(0,1); lcd.print("dev="); lcd.print(dev_angle);
           break;

           case 30:                  //第三關   走sine 波
                lcd.clear(); Checklight(1, 1, 1); //燈全亮
                motorStart(BaseL2, BaseR2, true, true);  
                Ultra_distanceF();
                disI = Getdis();
                disF = Getdis();
               while(distanceF > 30 && (disF - disI) < 300){
                Ultra_distanceF();
                disF = Getdis();      lcd.setCursor(8,1);lcd.print(disF - disI);lcd.print("  ");
                SimpleCorrect(BaseL2, BaseR2, 2, 2);    
               }
                motorBrake(0, 0, true, true);  Checklight_blink();    
           break;

           case 31:                
                lcd.clear(); Checklight(1, 1, 1); //燈全亮
                motorStart(BaseL2, BaseR2, true, true);  
                Ultra_distanceF();
                disI = Getdis();
                disF = Getdis();
               while(distanceF > 30 && (disF - disI) > 300){
                Ultra_distanceF();
                disF = Getdis();      lcd.setCursor(8,1);lcd.print(disF - disI);lcd.print("  ");
                SimpleCorrect(BaseL2, BaseR2, 2, 2);    
               }
                  motor_spinAround(BaseL4, BaseR4, -90, -90);
                  Ultra_distanceF();       
                  motorStart(BaseL2, BaseR2, true, true);            
               while(distanceF > 30){Checklight(1, 1, 1);               //偵測小於30就停止   
                   Ultra_distanceF();
                   SimpleCorrect(BaseL2, BaseR2, 2, 2);    
                }
                   motorBrake(0, 0,  true, true);  Delay(200);     Checklight_blink();    
           break;
           
           case 32:                  //第三關   走sine 波 Use Roll
                lcd.clear(); Checklight(1, 1, 1); //燈全亮
                motorStart(BaseL2, BaseR2, true, true);  
                Ultra_distanceF();
                theta = Devtheta(CaseGY521); Delay(500);
                RollI = roll;                Serial.print("RollI="); Serial.println(RollI);
                disI = Getdis();
                disF = Getdis();
               while(distanceF > 30 && (disF - disI) < 400){
                Ultra_distanceF();
                disF = Getdis();      lcd.setCursor(8,1);lcd.print(disF - disI);lcd.print("  ");
                theta = Devtheta(CaseGY521); Delay(20);
                if(roll - RollI > 10) {Correct(2, -2, true, true);}
                else if(roll - RollI < -10) {Correct(-2, 2, true, true);}
                else{SimpleCorrect(BaseL2, BaseR2, 2, 2);}  
                lcd.setCursor(0,0); lcd.print("Z="); lcd.print(theta); lcd.print(" ");  Serial.print(" theta="); Serial.print(theta);
                lcd.setCursor(8,0); lcd.print("RI="); lcd.print(RollI); lcd.print(" "); Serial.print("roll="); Serial.print(roll);
                lcd.setCursor(0,1); lcd.print("RF="); lcd.print(roll); lcd.print(" ");
                lcd.setCursor(8,1); lcd.print("L="); lcd.print(LeftSpeed); lcd.print(" ");  Serial.print(" LeftSpeed="); Serial.print(LeftSpeed);
                Serial.print(" RightSpeed="); Serial.println(RightSpeed);
               }
               motorBrake(0, 0, true, true);
               motor_spinAround(BaseL4, BaseR4, -90, -90); 
               motorStart(BaseL2, BaseR2, true, true);  
               Ultra_distanceF();
              while(distanceF > 30){
                Ultra_distanceF();
                SimpleCorrect(BaseL2, BaseR2, 2, 2);
              }
               motorBrake(0, 0, true, true);  Checklight_blink();  
           break;

           case 33:

           break;

           case 40:                  //伺服馬達測試 L
                  lcd.clear();
                  lcd.print("Enter positionL"); 
                  lcd.setCursor(0,1);
                  L = Enterkeypad(); //  Serial.print("L= ");  Serial.println(L); 
                  lcd.clear();
                  setServoL(L);
                  
           break;

           case 41:                  //伺服馬達測試  R
                  lcd.clear();
                  lcd.print("Enter positionR"); 
                  lcd.setCursor(0,1);
                  R = Enterkeypad();  // Serial.print("R= ");  Serial.println(R); 
                  lcd.clear();
                  setServoR(R);
                  
           break;

           case 42:                  //伺服馬達測試 Up Down
                  lcd.clear();
                  lcd.print("Enter positionUp"); 
                  lcd.setCursor(0,1);
                  Up = Enterkeypad();   //Serial.print("Up= ");  Serial.println(Up); 
                  lcd.clear();
                  setServoUD(Up); 
           break;

           case 43:                  //伺服馬達測試夾取動作
                  lcd.clear();
                  lcd.print("Enter positionI"); 
                  lcd.setCursor(0,1); lcd.print("Catch LR");
                  L = Enterkeypad();   //Serial.print("Up= ");  Serial.println(Up); 
                  lcd.clear();
                  lcd.print("Enter positionF"); 
                  lcd.setCursor(0,1); lcd.print("Catch LR");
                  R = Enterkeypad();   //Serial.print("Up= ");  Serial.println(Up);
                  setServoLR(L ,R);  
           break;

           case 44:                  //伺服馬達測試上升動作
                  lcd.clear();
                  lcd.print("Enter positionI"); 
                  lcd.setCursor(0,1);lcd.print("Catch Up Down");
                  L = Enterkeypad();   Serial.print("Up= ");  Serial.println(Up); 
                  lcd.clear();
                  lcd.print("Enter positionF"); 
                  lcd.setCursor(0,1);lcd.print("Catch Up Down");
                  R = Enterkeypad(); //  Serial.print("Up= ");  Serial.println(Up);
                  setServoUp_Down(L ,R);  
           break;

           case 45:    //C
                 Catch_Up();
                 Delay(1000);
                 Catch_Down();
           break;
//************************************************************************************************************************
//************************************************************************************************************************
           case 51:
                  flag_failure = false;
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("Start Case....."); lcd.setCursor(0,1);
                  A = Enterkeypad();
                  switch(A){
                    case 1:
                           Case_61();
                           Case_62();
                           Case_63();
                           Case_64();
                    break;
                    case 2:
                           Case_62();
                           Case_63();
                           Case_64();
                    break;
                    case 3:
                           Case_63();
                           Case_64();
                    break;
                    case 4:
                           Case_64();
                    break;
                  }

           break;
           
//************************************************************
           case 61:                //第一關  澆水
                Case_61();
           break;
//************************************************************
           case 62:                  //第二關   閃避障礙物
                 Case_62();   
           break;
//************************************************************
           case 63:
                 Case_63();          //第三關  Sin 波  
           break;
//************************************************************
           case 64:
                 Case_64();           //第四關   夾取盆栽
           break;
//************************************************************
           case 992:
                case_992();          //直走一段距離超音波停止 高速檔
           break;
//************************************************************
           case 993:
                case_993();           //直走一段距離超音波停止 低速檔
           break;
//************************************************************
           case 994:
                case_994();           //直走多少距離後後右轉
           break;
//************************************************************
           case 995:
                case_995();           //直走多少距離後後左轉
           break;
//************************************************************
           case 996:
                case_996();            //直走後右轉
           break;
//************************************************************
           case 997:
                case_997();           //直走後左轉
           break;
//************************************************************
           case 998:
                case_998();           //直走後走一段距離右轉
           break;
//************************************************************
           case 999:
                case_999();           //直走後走一段距離左轉
           break;
//************************************************************************************************************************
//************************************************************************************************************************
           case 84:
                  lcd.clear();
                  lcd.print("Enter Kp..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp6 = B;  Ki6 = C; Kd6 = D;
                  Serial.print(Kp);Serial.print(" , ");Serial.print(Ki);Serial.print(" , ");Serial.println(Kd);Delay(1000);
                  motorStart(BaseL2, BaseR2, true, true);
             Ultra_distanceF();
           while(distanceF > 40){
             Ultra_distanceF();
             PID_GY521_Calculation3(0);
                 }
                 motorBrake(0,0,true,true);
           break;
            case 85:
                  lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL2, BaseR2, true, true);  
                 while(distanceF > 35){
                   Ultra_distanceF();     //前面的超音波
                   PID_GY521_Calculation3(0);
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
           break;

//************************************************************************************************************************
           case 86:
                  lcd.clear();
                  lcd.print("Enter Kp..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp4 = B;  Ki4 = C; Kd4 = D;
                  Serial.print(Kp);Serial.print(" , ");Serial.print(Ki);Serial.print(" , ");Serial.println(Kd);Delay(1000);
                  motorStart(BaseL3, BaseR3, true, true);
             Ultra_distanceF();
           while(distanceF > 40){
             Ultra_distanceF();
             PID_GY521_Calculation(0);
                 }
                 motorBrake(0,0,true,true);
           break;

            case 87:
                  lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL3, BaseR3, true, true);  
                 while(distanceF > 35){
                   Ultra_distanceF();     //前面的超音波
                   PID_GY521_Calculation(0);
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
           break;

           case 88:
                  lcd.clear();
                  lcd.print("Enter Kp..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp5 = B;  Ki5 = C; Kd5 = D;
                  Serial.print(Kp);Serial.print(" , ");Serial.print(Ki);Serial.print(" , ");Serial.println(Kd);Delay(1000);
                  motorStart(BaseL5, BaseR5, true, true);
           Ultra_distanceF();
           while(distanceF > 55){
             Ultra_distanceF();
             PID_GY521_Calculation2(0);
                 }
                 motorBrake(0,0,true,true);
           break;            

            case 89:
                  lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL5, BaseR5, true, true);  
                 while(distanceF > 40){
                   Ultra_distanceF();     //前面的超音波
                   PID_GY521_Calculation2(0);
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
           break;

           case 90:    
                  lcd.clear();
                  lcd.print("Enter Kp..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp = B;  Ki = C; Kd = D;
                  Serial.print(Kp);Serial.print(" , ");Serial.print(Ki);Serial.print(" , ");Serial.println(Kd);Delay(1000);
                  motorStart(BaseL2, BaseR2, true, true);
           Ultra_distanceF();
           while(distanceF > 40){
             Ultra_distanceF();
             PID_Calculation(0);
             Encoderprint();
                 }
                 motorBrake(0,0,true,true);
           break;
                    
           case 91:      //PID 2檔 直線
                   lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL2, BaseR2, true, true);  
                 while(distanceF > 30){
                   Ultra_distanceF();     //前面的超音波
                   PID_Calculation(0);
                   Encoderprint();
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
           break;

           case 92:    
                  lcd.clear();
                  lcd.print("Enter Kp2..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki2..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd2..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp2 = B;  Ki2 = C; Kd2 = D;
                  Serial.print(Kp2);Serial.print(" , ");Serial.print(Ki2);Serial.print(" , ");Serial.println(Kd2);Delay(1000);
                  motorStart(BaseL3, BaseR3, true, true);
           Ultra_distanceF();
           while(distanceF > 40){
             Ultra_distanceF();
             PID_Calculation2(0);
             Encoderprint();
                 }
                 motorBrake(0,0,true,true);
           break;        

           case 93:       //PID 3檔 直線
                   lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL3, BaseR3, true, true);  
                 while(distanceF > 30){
                   Ultra_distanceF();     //前面的超音波
                   PID_Calculation2(0);
                   Encoderprint();
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
           break;

            case 94:
                  lcd.clear();
                  lcd.print("Enter Kp3..."); 
                  B = Enterkeypad();   Serial.print("B= ");  Serial.println(B); lcd.clear();
                  lcd.print("Enter Ki3..."); 
                  C = Enterkeypad();   Serial.print("C= ");  Serial.println(C); lcd.clear();
                  lcd.print("Enter Kd3..."); 
                  D = Enterkeypad();   Serial.print("D= ");  Serial.println(D); lcd.clear();
                  Kp3 = B;  Ki3 = C; Kd3 = D;
                  Serial.print(Kp3);Serial.print(" , ");Serial.print(Ki3);Serial.print(" , ");Serial.println(Kd3);Delay(1000);
                  motorStart(BaseL5, BaseR5, true, true);
           Ultra_distanceF();
           while(distanceF > 40){
             Ultra_distanceF();
             PID_Calculation3(0);
             Encoderprint();
                 }
                 motorBrake(0,0,true,true);
            break;

            case 95:
                  lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL5, BaseR5, true, true);  
                 while(distanceF > 35){
                   Ultra_distanceF();     //前面的超音波
                   PID_Calculation3(0);
                   Encoderprint();
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
            break;

            case 96:       //測試檔位
                  lcd.clear();
                  lcd.print("Motor File 1~5...."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A);
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                 switch(A){
                    case 1:
                           lcd.setCursor(1,0); lcd.print(BaseL1);  lcd.setCursor(8,0); lcd.print(BaseR1); Delay(1000); lcd.clear(); 
                           motorStart(BaseL1, BaseR1, true, true); speedprint(); Checklight_blink();  
                    break;
                    case 2:
                           lcd.setCursor(1,0); lcd.print(BaseL2);  lcd.setCursor(8,0); lcd.print(BaseR2); Delay(1000); lcd.clear(); 
                           motorStart(BaseL2, BaseR2, true, true); speedprint(); Checklight_blink();  
                    break;
                    case 3:
                           lcd.setCursor(1,0); lcd.print(BaseL3);  lcd.setCursor(8,0); lcd.print(BaseR3); Delay(1000); lcd.clear(); 
                           motorStart(BaseL3, BaseR3, true, true); speedprint(); Checklight_blink();  
                    break;
                    case 4:
                           lcd.setCursor(1,0); lcd.print(BaseL4);  lcd.setCursor(8,0); lcd.print(BaseR4); Delay(1000); lcd.clear(); 
                           motorStart(BaseL4, BaseR4, true, true); speedprint(); Checklight_blink();  
                    break;
                    case 5:
                           lcd.setCursor(1,0); lcd.print(BaseL5);  lcd.setCursor(8,0); lcd.print(BaseR5); Delay(1000); lcd.clear(); 
                           motorStart(BaseL5, BaseR5, true, true); speedprint(); Checklight_blink();  
                    break;
                    default:
                            lcd.print("Error....."); Checklight_blink();  
                    break;
                 }
            break;

            case 97:
                  lcd.clear();
                  lcd.print("Enter distance...."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A);
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  disI = Getdis();
                  disF = Getdis();
                  motorStart(BaseL3, BaseR3, true, true);
                    while((disF - disI) < A){
                      disF = Getdis();
                      PID_GY521_Calculation(0);
                    }
                   motorBrake(0, 0, true, true);  Checklight_blink();  
            break;

            case 888:
                  lcd.clear();
                  lcd.print("PID Straight..."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter 0 to break..."); 
                  A = Enterkeypad();   Serial.print("A= ");  Serial.println(A); 
                  if(A == 0) {lcd.clear(); break;}   else{lcd.clear();}
                  Ultra_distanceF();     //前面的超音波
                  motorStart(BaseL3, BaseR3, true, true);  
                 while(distanceF > 35){
                   Ultra_distanceF();     //前面的超音波
                   PID_GY521_Calculation(-90);
                 }
                    motorBrake(0, 0, true, true);      Checklight_blink();
            break;


            
            default:
                   lcd.clear();
                   lcd.print("Invalid Key.....");
                   lcd.setCursor(0,1);
                   lcd.print("Try again later.....");
                   Delay(1000);
                   lcd.clear();
            break;
  break;}
//************
case '2':         //ASCII 字元碼char to DEX 為50

break;
//************
case '3':            //ASCII 字元碼char to DEX 為51       //測試按鍵
lcd.clear();  lcd.print("Enter key.....");
D = Enterkeypad(); 
lcd.clear(); lcd.print("Enter key= "); lcd.print(D);
Delay(1000); lcd.clear();
     break;
//************
case '4':       //ASCII 字元碼char to DEX 為52            //測試Delay
 lcd.clear();
 lcd.print("Enter Delay secs..."); 
 lcd.setCursor(0,1);
 A = Enterkeypad();  lcd.clear(); A = 1000 * A;
 Delay(A);
     break;
//************
case '5':                       //ASCII 字元碼char to DEX 為53
Wait_to_water();
     break;
//************
case '6':                         //ASCII 字元碼char to DEX 為54

     break;
//************ 
case '7':                         //ASCII 字元碼char to DEX 為55
CaseGY521 = RESET(-90);                                       //測試轉彎後，準位調整   
     break;
   
//************
case '8':                       //ASCII 字元碼char to DEX 為56
CaseGY521 = RESET(90);                                       //測試轉彎後，準位調整      
     break;
//************
case '9':                        //ASCII 字元碼char to DEX 為57

     break;
//************
case '0':                     //ASCII 字元碼char to DEX 為48
  lcd.clear();              
  CaseGY521 = RESET(999);    //準位歸0
  Pulse_RESET();                  //脈衝數值重新計算
  flag = 0;
  flag2 = 0;
  flag_GY521 = 0;
  flag_failure = false;
  check = 0;
  thetaprint(); 
     break;
//************
case 'A':                //ASCII 字元碼char to DEX 為65
  lcd.clear();
  Checklight_blink();
  RGB_lightL(1,1,1);
  RGB_lightR(1,1,1);
  Delay(1000);
  RGB_lightL(0,0,0);
  RGB_lightR(0,0,0);
     break;
//************
case 'B':                   //ASCII 字元碼char to DEX 為66
  lcd.clear();
  thetaprint();                 //Yaw值print out
  lcd.setCursor(0, 1); // 將游標移動第0欄、第1列。        
  lcd.print("Dis=");
  D = (-level_L  + level_R)/2 * 0.11;           //讀取目前走了多少  
  lcd.print(D); lcd.print(" cm");
     break;
//************
case 'C':                             //ASCII 字元碼char to DEX 為67
  Ultra_distanceR1();     //超音波數值print out
  Ultra_distanceR2();
  Ultra_distanceR3();
  Ultra_distanceF();
  Ultra_distanceL1();
  lcd.clear();
  Ultraprint();
     break;
//************
case 'D':                          //ASCII 字元碼char to DEX 為68
  lcd.clear();
  lcd.setCursor(0,1);lcd.print("PL=");lcd.print(-level_L);           //脈衝數print out
  lcd.setCursor(8,1);lcd.print("PR=");lcd.print(level_R);
     break;
//************
case '*':                          //ASCII 字元碼char to DEX 為42
  lcd.clear(); lcd.print("theta2......."); Delay(1000);           //記錄讀值print out
  Record_Print(Num1);
 // Serial.println("LeftSpeed-----------------------");
  lcd.clear(); lcd.print("LeftSpeed......."); Delay(1000);
  Record_Print2(Num2);
 // Serial.println("RightSpeed-----------------------");
  lcd.clear(); lcd.print("RightSpeed......."); Delay(1000);
  Record_Print3(Num3);
  Delay(1000); 
     break;
//************
case '#':                            //ASCII 字元碼char to DEX 為35
  motorBrake(0 , 0 , true , true);         //煞車
     break;
//************
}
}

//**********************************************************
float Enterkeypad(){                                      //利用while迴圈和按鍵，輸入一碼或二碼或三碼，以得到input
  int i=0, c = 0, K[3], neg = 1;
  float n = 0, s = 1;
  K[0]  = 0;  K[1]  = 0;  K[2]  = 0;  K[3]  = 0;
  lcd.setCursor(0,1);
  while(i<100){
  K[i]  = keypad.getKey();
  if(K[i] ==  49) {K[i] = 1; lcd.print(K[i]);i++;}  
  else if(K[i] ==  50) {K[i] = 2; lcd.print(K[i]); i++;}      
  else if(K[i] ==  51) {K[i] = 3; lcd.print(K[i]); i++;}      
  else if(K[i] ==  52) {K[i] = 4; lcd.print(K[i]); i++;}      
  else if(K[i] ==  53) {K[i] = 5; lcd.print(K[i]); i++;}      
  else if(K[i] ==  54) {K[i] = 6; lcd.print(K[i]); i++;}      
  else if(K[i] ==  55) {K[i] = 7; lcd.print(K[i]); i++;}      
  else if(K[i] ==  56) {K[i] = 8; lcd.print(K[i]); i++;}      
  else if(K[i] ==  57) {K[i] = 9; lcd.print(K[i]); i++;}      
  else if(K[i] ==  48) {K[i] = 0; lcd.print(K[i]); i++;}
  else if(K[i] ==  35) {lcd.clear(); lcd.print("Restart"); Delay(1000); lcd.clear(); i=0; n = 0, s = 1; c = 0; neg = 1; lcd.setCursor(0,1);}      //  # : Restart
  else if(K[i] ==  66) {lcd.clear(); lcd.setCursor(0,1); for(int j=0;j<i-1;j++) {lcd.print(K[j]);}i--; if(i<0) {i=0;}}                 //  B : Delete
  else if(K[i] ==  67) {lcd.clear(); for(int j=0;j<i;j++) {K[j] = 0;} break;}                             //  C : Break; 直接跳離迴圈
  else if(K[i] ==  68) {neg = -1; lcd.print("-");}                            //  D : 負號
  else if(K[i] ==  42) {if(i == 1) {c = 1;} else if(i == 2) {c = 2;} lcd.print(".");}                            //  * : .(小數點)
  else if(K[i] ==  65 && i == 0) {lcd.clear(); lcd.print("error...");  lcd.setCursor(0,1); lcd.print("Restart"); Delay(1000); lcd.clear(); i=0; lcd.setCursor(0,1);}    
  else if(K[i] ==  65 && i == 1) {K[2] = K[0]; K[1] = 0; K[0] = 0; break;}             //輸入一碼
  else if(K[i] ==  65 && i == 2) {K[2] = K[1]; K[1] = K[0]; K[0] = 0; if(c == 1) {s = 0.1;} break;}          //輸入二碼
  else if(K[i] ==  65 && i == 3) {lcd.clear(); if(c == 1) {s = 0.01;} else if(c == 2) {s =0.1;} break;}        // A  : Enter            //輸入三碼
  else{K[i]  = 0;}          
  //Serial.println(i);     
  } 
  n = neg * (100*K[0]  + 10*K[1] + 1*K[2]) * s;
  //Serial.println(n);
  return n;
}
//輪詢式delay**********************************************************
void Delay(unsigned long t){
  unsigned long past;
  now = millis();
  past = now;
  while(now - past < t && flag_failure == false){
    now = millis();
    emergency();              //緊急按鍵
   // theta = Devtheta(CaseGY521);
   //Serial.print("past="); Serial.print(past); Serial.print("  now="); Serial.println(now);
  }
}

//紀錄**********************************************************
void Record_Theta1(float alpha){    
  static int n  =  0;        //使用static宣告只會初始化一次        
  Record1[n]  = alpha;
  Num1 = n;
  n++;
}
//**********************************************************
void Record_Theta2(float alpha){    
  static int n2  =  0;        //使用static宣告只會初始化一次        
  Record2[n2]  = alpha;
  Num2 = n2;
  n2++;
}
//**********************************************************
void Record_Theta3(float alpha){    
  static int n3  =  0;        //使用static宣告只會初始化一次        
  Record3[n3]  = alpha;
  Num3 = n3;
  n3++;
}
//**********************************************************
void Record_Print(int N){   
  Serial.println("Start Record Print......");   
  lcd.clear(); lcd.print("Start Print...."); Delay(1000);
  float sum=0;  
  float avg;       
  lcd.clear(); 
 for(int i = 0; i < N; i++){
  lcd.setCursor(0,0);   
  Serial.println(Record1[i]);
  sum = sum + Record1[i];
  Delay(100);
  lcd.print(Record1[i],2);
}
  avg = sum/N;
  lcd.clear();
  lcd.setCursor(0,1); // 將游標移動第0欄、第1列。
  lcd.print("avg = ");
  lcd.print(avg,2);
  Serial.print("avg=  ");
  Serial.print(avg);
  Delay(1000);
}
//**********************************************************
void Record_Print2(int N){   
Serial.println("Start Record Print......");   
lcd.clear(); lcd.print("Start Print...."); Delay(1000);
float sum=0;  
float avg;      
lcd.clear();    
for(int i = 0; i < N; i++){
   lcd.setCursor(0,0); 
  Serial.println(Record2[i]);
  sum = sum + Record2[i];
  Delay(100);
  lcd.print(Record2[i],2);
}
  avg = sum/N;
  lcd.clear();
  lcd.setCursor(0,1); // 將游標移動第0欄、第1列。
  lcd.print("avg = ");
  lcd.print(avg,2);
  Serial.print("avg=  ");
  Serial.print(avg);
  Delay(1000);
}
//**********************************************************
void Record_Print3(int N){   
Serial.println("Start Record Print......");   
lcd.clear(); lcd.print("Start Print...."); Delay(1000);
float sum=0;  
float avg; 
lcd.clear();      
for(int i = 0; i < N; i++){
  lcd.setCursor(0,0); 
  Serial.println(Record3[i]);
  sum = sum + Record3[i];
  Delay(100);
  lcd.print(Record3[i],2);
}
  avg = sum/N;
  lcd.clear();
  lcd.setCursor(0,1); // 將游標移動第0欄、第1列。
  lcd.print("avg = ");
  lcd.print(avg,2);
  Serial.print("avg=  ");
  Serial.print(avg);
  Delay(1000);
}

//*****************************************************************************************************************************************************************//
