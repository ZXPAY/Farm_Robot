//Use Relay to be button, turn on relay to fill up the potting.
//澆水程式碼
/*R385抽水馬達
尺寸：90毫米*40毫米*35毫米
出水口直徑：內徑6毫米，外徑9毫米
工作電壓：DC6-12V  （推薦9V1A 12V1A）    //12V單次使用時間最好控制再3小時內，6V更長些～～
工作電流：0.5-0.7A
流量：1.5-2L/Min（左右）
最大吸程：2米
揚程：最大為3米
壽命：最大可達2500H
水溫：可達80
電壓6V時：功率為6W/H
 */
const int RelayPin  = 53;         

void Watering_setup() {
  // put your setup code here, to run once:
pinMode(RelayPin, OUTPUT);
}
//**********************************************************
void Ultrasonic_Watering(){ 
  float dev;
   Ultra_distanceR1();
   Ultra_distanceR2();
   //Ultraprint();
  lcd.setCursor(8,0);lcd.print(distanceR1);lcd.print(" ");
  lcd.setCursor(8,1);lcd.print(distanceR2);lcd.print(" ");
  //lcd.setCursor(12,0);lcd.print(distanceR3,1);lcd.print(" ");
  if(distanceR1 < 50) {check =0;}
  if (distanceR1 > 50 && distanceR2  <= 29 && flag != 3 && check == 0)     //第二個超音波偵測到，澆水     0 1 0                     
   {//Serial.println("GET"); 
    lcd.clear();  lcd.setCursor(0, 1);   lcd.print("GET     ");        
          motorBrake(0, 0,  true, true);                //停車，澆水
       Wait_to_water();    //停車三秒
    if(flag == 0 && flag_failure == false) {
                        Watering(8);
                        Delay(100);
                        Rundis(15, BaseL3-5, BaseR3);                //走15公分，避免再次澆水
                        check = 1;
                        flag_Water = 0;
                        flag++;}
     else if(flag == 1 && flag_failure == false){
              Watering(6.5);
              Delay(100);
              Rundis(15, BaseL3-5, BaseR3);                //走15公分，避免再次澆水
              check = 1;
              flag_Water = 0;
              flag++;}   
     else if(flag == 2 && flag_failure == false){
              Watering(7);
              Delay(100);
              Rundis(15, BaseL3-5, BaseR3);                //走15公分，避免再次澆水
              check = 1;
              flag++;}    
  }
  
 else if(distanceR1  < 40)             //第一個超音波偵測到，煞車     1 0 0
   {//Serial.println("NEAR"); 
    Checklight(1, 0, 0);
  PID_GY521_Calculation(0);
  lcd.setCursor(0, 1);  lcd.print("NEAR     ");
  lcd.setCursor(8,0);lcd.print(distanceR1);lcd.print(" ");
  lcd.setCursor(8,1);lcd.print(distanceR2);lcd.print(" ");
  }
     else if(distanceR1 > 50 && distanceR2 > 29 && distanceR2 < 45 && check == 0) { 
                                                              motorBrake(0, 0, true, true);  
                                                              motor_spinAround(BaseL4, BaseR4, -85, -90);   Checklight(1, 0, 0);     //距離太遠，澆不到水
                                                              dev = distanceR2 - 29;
                                                              Rundis(dev, BaseL2, BaseR2);      // Rundis(int D, int speedL, int speedR);
                                                              motorBrake(0, 0, true, true);
                                                              motor_spinAround(BaseL4, BaseR4, 85, 90);  
                                                              Rundis(-10, BaseL2, BaseR2);      // Rundis(int D, int speedL, int speedR);
                                                              motorBrake(0, 0, false, false); 
                                                              motorStart(BaseL2, BaseR2, true, true);
                                                             }


    else if(distanceR1 > 50 && distanceR2 < 12 && check == 0){ motorBrake(0, 0, true, true);
                                                              Rundis(-15, BaseL2, BaseR2); motorBrake(0, 0, false, false);          //距離太近，會噴出去
                                                              dev = distanceR2 + 7;
                                                              motor_spinAround(BaseL4, BaseR4, 85, 90);
                                                              Rundis(dev, BaseL2, BaseR2); motorBrake(0, 0, true, true);
                                                              motor_spinAround(BaseL4, BaseR4, -85, -90);
                                                              motorStart(BaseL2, BaseR2, true, true);
                                                             }
                                                                                                                                                                                    
    else{PID_GY521_Calculation(0); lcd.setCursor(0, 1); lcd.print("Nothing");}
   // Serial.println("Nothing");
   //Serial.print("check="); Serial.print(check); Serial.print("flag="); Serial.println(flag);
}
//**********************************************************
void Wait_to_water(){                //等待澆水，3秒後確認澆水不會澆出去，則繼續執行。 反之，則按下緊急按鈕 # 鍵
  RGB_lightL(LOW, LOW, HIGH);
  RGB_lightR(LOW, LOW, HIGH);
  Delay(3000);
  RGB_lightL(LOW, LOW, LOW);
  RGB_lightR(LOW, LOW, LOW);
}
//**********************************************************
void Watering(float t){             //澆水時間函數
  t = t*1000;
  digitalWrite(RelayPin,  HIGH);     //開關開
  Delay(t); 
  digitalWrite(RelayPin,  LOW);     //開關開
}
//*****************************************************************************************************************************************************************//
