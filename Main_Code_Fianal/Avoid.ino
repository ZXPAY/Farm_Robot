//**********************************************************
void Ultrasonic_AvoidingR1(){ 
   Ultra_distanceR1();
   AvoidingR1();
   Checklight(0, 0, 0);
}
//**********************************************************
void AvoidingR1(){                                
if(distanceR1 < 10) {setMotorLeft(BaseL2 - 15, true);
                    setMotorRight(BaseR2 + 45, true);                   
                    //Serial.println("Drive far");
                    lcd.setCursor(0,1);lcd.print("Far    ");
                    Checklight(1, 0, 0);}
                                      
else if(distanceR1 > 10 && distanceR1 < 100) {setMotorLeft(BaseL2 -5, true);
                                              setMotorRight(BaseR2 - 6, true);
                                              //Serial.println("Drive close");
                                              lcd.setCursor(0,1);lcd.print("Close  ");
                                              Checklight(0, 1, 0);}
                                        
else{setMotorLeft(BaseL2, true);
     setMotorRight(BaseR2, true);Checklight(0, 0, 1); //Serial.println("Forward");
     lcd.setCursor(0,1);lcd.print("Farward");}
}        
//**********************************************************
void Ultrasonic_AvoidingR2(){ 
   Ultra_distanceR1();
   AvoidingR2();
   Checklight(0, 0, 0);
}
//**********************************************************
void AvoidingR2(){                          
if(distanceR1 < 4.5) {setMotorLeft(BaseL2 - 25, true);
                    setMotorRight(BaseR2 + 35, true);                   
                    //Serial.println("Drive far");
                    lcd.setCursor(0,1);lcd.print("Far    ");
                    Checklight(1, 0, 0);}
                                      
else if(distanceR1 > 4.5 && distanceR1 < 100) {setMotorLeft(BaseL2 + 55, true);
                                              setMotorRight(BaseR2 - 15, true);
                                              //Serial.println("Drive close");
                                              lcd.setCursor(0,1);lcd.print("Close  ");
                                              Checklight(0, 1, 0);}
                                        
else{setMotorLeft(BaseL2, true);
     setMotorRight(BaseR2, true);Checklight(0, 0, 1); //Serial.println("Forward");
     lcd.setCursor(0,1);lcd.print("Farward");}
}                     
           
//**********************************************************
void Ultrasonic_AvoidingL1(){ 
   Ultra_distanceL1();
   AvoidingL1();
   Checklight(0, 0, 0);
}
//**********************************************************
void AvoidingL1(){                                 
if(distanceL1 < 12) {setMotorLeft(BaseL2 + 45, true);
                    setMotorRight(BaseR2 - 10, true);                   
                    //Serial.println("Drive far");
                    lcd.setCursor(0,1);lcd.print("Far    ");
                    Checklight(1, 0, 0);}
else if(distanceL1 > 12) {setMotorLeft(BaseL2 - 10, true);
                          setMotorRight(BaseR2 + 20, true);
                          //Serial.println("Drive close");
                          lcd.setCursor(0,1);lcd.print("Close  ");
                          Checklight(0, 1, 0);}
else{setMotorLeft(BaseL2, true);
     setMotorRight(BaseR2, true);Checklight(0, 0, 1); //Serial.println("Forward");
     lcd.setCursor(0,1);lcd.print("Farward");}
}   
//**********************************************************
void Ultrasonic_AvoidingL2(){ 
   Ultra_distanceL1();
   AvoidingL2();
   Checklight(0, 0, 0);
}
//**********************************************************
void AvoidingL2(){                                 
if(distanceL1 < 12) {setMotorLeft(BaseL2 + 35, true);
                    setMotorRight(BaseR2 - 10, true);                   
                    //Serial.println("Drive far");
                    lcd.setCursor(0,1);lcd.print("Far    ");
                    Checklight(1, 0, 0);}
else if(distanceL1 > 12) {setMotorLeft(BaseL2 - 10, true);
                          setMotorRight(BaseR2 + 45, true);
                          //Serial.println("Drive close");
                          lcd.setCursor(0,1);lcd.print("Close  ");
                          Checklight(0, 1, 0);}
else{setMotorLeft(BaseL2, true);
     setMotorRight(BaseR2, true);Checklight(0, 0, 1); //Serial.println("Forward");
     lcd.setCursor(0,1);lcd.print("Farward");}
}                                       
//*****************************************************************************************************************************************************************//
//*****************************************************************************************************************************************************************//
