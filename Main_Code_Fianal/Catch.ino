//夾取盆栽程式碼
//**********************************************************
void Ultrasonic_CatchUp(){                                //Down 10-->100    Up 100-->10
   Ultra_distanceR1();
   Catching_Up();
   Checklight(0, 0, 0);
}
//**********************************************************
void Catching_Up(){                                     //11~20 cm 可夾取
  float disI, disF;
  lcd.setCursor(0, 1); lcd.print("Nothing...");
  if(distanceR1 < 70 && distanceR1 > 7 && flag2 == 0) {         Checklight(1, 1, 1);     //發現盆栽
   lcd.setCursor(0, 1);  lcd.print("NEAR     ");
   RGB_lightL(HIGH, HIGH, HIGH);
   disI = Getdis();
   disF = Getdis();
     while(disF - disI < 24){
       disF = Getdis();
       PID_GY521_Calculation(0);
     }
    RGB_lightL(LOW, LOW, LOW);
    motorBrake(0, 0, true, true);
    motor_spinAround(BaseL4, BaseR4, -85, -90);  
    motorStart(BaseL2, BaseR2, true, true);
    Ultra_distanceF(); 
      while(distanceF > 16){
        Ultra_distanceF();
      }
     motorBrake(0, 0, true, true);
     lcd.setCursor(0, 1);  lcd.print("Catch Up  ");
     Catch_Up();         lcd.clear();
     Rundis(-12, BaseL2, BaseR2);
     motorBrake(0, 0, false, false);
     motor_spinAround(BaseL4, BaseR4, 85, 90); 
     flag2 = 2;
  }

  else if(distanceR1  < 7 && flag2 == 0){
     motorBrake(0, 0, true, true);
     Rundis(-10, BaseL2, BaseR2);
     motorBrake(0, 0, false, false);
     motor_spinAround(BaseL4, BaseR4, 85, 90);   Checklight(1, 0, 0);     //距離太近，會撞到
     Rundis(15, BaseL2, BaseR2);
     motorBrake(0, 0, true, true);
     motor_spinAround(BaseL4, BaseR4, -85, -90);  
     Rundis(-12, BaseL2, BaseR2);
     motorBrake(0 , 0, false, false);
     motorStart(BaseL2, BaseR2, true, true);  
  }

}
//**********************************************************
void Catch_Up(){
  setServoLR(100,50);          Delay(100);       //夾取
  setServoUp_Down(5, 85);   Delay(100);      //下降
  setServoLR(50,85);          Delay(100);       //夾取
  setServoUp_Down(85, 20);                     //上升
}
//**********************************************************
void Ultrasonic_CatchDown(){
   Ultra_distanceR1();
   Catching_Down();
   Checklight(0, 0, 0);
}
//**********************************************************
void Catching_Down(){
  float disI, disF;
  lcd.setCursor(0, 1); lcd.print("Nothing...");
  RGB_lightL(HIGH, HIGH, HIGH);
  if(distanceR1 < 70 && distanceR1 > 7 && flag2 == 0) {         Checklight(1, 0, 1);     //發現盆栽
   disI = Getdis();
   disF = Getdis();
     while(disF - disI < 24){
       disF = Getdis();
       PID_GY521_Calculation(0);
     }
    RGB_lightL(LOW, LOW, LOW);
    motorBrake(0, 0, true, true);
    motor_spinAround(BaseL4, BaseR4, -85, -90);  
    motorStart(BaseL2, BaseR2, true, true);
    Ultra_distanceF(); 
      while(distanceF > 16.5){
        Ultra_distanceF();
      }
    motorBrake(0, 0, true, true);
    lcd.setCursor(0, 1);  lcd.print("Catch Down  ");
    Catch_Down();        lcd.clear();
    Rundis(-5, BaseL2, BaseR2);
    motorBrake(0, 0, false, false);
    motor_spinAround(BaseL4, BaseR4, 85, 90); 
    flag2 = 2;
  }

  else if(distanceR1  <  7 && flag2 == 0){
     motorBrake(0, 0, true, true);
     Rundis(-10, BaseL2, BaseR2);
     motorBrake(0, 0, false, false);
     motor_spinAround(BaseL4, BaseR4, 85, 90);   Checklight(1, 0, 0);     //距離太近，會撞到
     Rundis(15, BaseL2, BaseR2);
     motorBrake(0, 0, true, true);
     motor_spinAround(BaseL4, BaseR4, -85, -90);  
     Rundis(-10, BaseL2, BaseR2);
     motorBrake(0 , 0, false, false);
  }

}
//**********************************************************
void Catch_Down(){
  setServoUp_Down(20, 85);   Delay(100);         //下降
  setServoLR(85,50);          Delay(100);         //放開
  setServoUp_Down(85, 5);                       //上升
  setServoLR(50,100);          Delay(100);
}
//*****************************************************************************************************************************************************************//


