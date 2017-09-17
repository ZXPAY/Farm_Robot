//雙向霍爾編碼器 PPR = 248 pulses/rev
 /* attachinterrupt 中斷子腳位判斷
Board                             int.0  |  int.1  |  int.2  |  int.3  |  int.4  |  int.5
Uno, Ethernet                       2        3        
Mega2560                            2        3         21         20        19       18
32u4 based (e.g Leonardo, Micro)    3        2          0          1         7  
*/
//Encoder宣告*************
//Pin            棕:VCC   綠:GND    藍:Pin A    紫:Pin B
const byte pinA_L = 2;      // encoder pin A to Arduino pin 2 which is also interrupt pin 0 which we will use
const byte pinB_L = 3;      // encoder pin B to Arduino pin 2 which is also interrupt pin 1 but we won't use it
const byte pinA_R = 19;      // encoder pin A to Arduino pin 19 which is also interrupt pin 4 which we will use
const byte pinB_R = 18;      // encoder pin B to Arduino pin 18 which is also interrupt pin 5 but we won't use it

void Encoder_setup(){
//Encoder
pinMode(pinA_L,INPUT);    // reads Pin A of the encoder
pinMode(pinB_L,INPUT);    // reads Pin B of the encoder
digitalWrite(pinA_L,HIGH);                        // turn on pullup resistor
digitalWrite(pinB_L,HIGH);
attachInterrupt(0,knobTurned_L,RISING);    // calls our 'knobTurned()' function when pinA_L goes from LOW to HIGH

pinMode(pinA_R,INPUT);    // reads Pin A of the encoder
pinMode(pinB_R,INPUT);    // reads Pin B of the encoder
digitalWrite(pinA_R,HIGH); 
digitalWrite(pinB_R,HIGH);
attachInterrupt(4,knobTurned_R,RISING);    // calls our 'knobTurned()' function when pinA_L goes from LOW to HIGH
}
//Encoder程式碼**********************************************************
void knobTurned_L(){     //計算pulses數目
  /* AH HA! the knob was turned */
  state = 0;    // reset this value each time
  int stateLA  = digitalRead(pinA_L);
  int stateLB  = digitalRead(pinB_L);
  state = state + stateLA;   // add the state of Pin A
 
  state <<= 1;  // shift the bit over one spot
  state = state + stateLB;   // add the state of Pin B
  
  /* now we have a two bit binary number that holds the state of both pins
     00 - something is wrong we must have got here with a key bounce
     01 - sames as above - first bit should never be 0
     10 - knob was turned backwards
     11 - knob was turned forwards
     */   
  /* We can pull a value out of our truth table and add it to the current level_A */
  level_L =  level_L  + bump[state];
  /* Let's see what happened */
  //Serial.print(bits[state] + "    ");  // show us the two bits
  //Serial.print(bump[state],DEC);       // show us the direction of the turn
 // Serial.print("    ");
}
//**********************************************************
void knobTurned_R(){
  /* AH HA! the knob was turned */
  state = 0;    // reset this value each time
  int stateRA  = digitalRead(pinA_R);
  int stateRB  = digitalRead(pinB_R);
  state = state + stateRA;   // add the state of Pin A
 
  state <<= 1;  // shift the bit over one spot
  state = state + stateRB;   // add the state of Pin B
  
  /* now we have a two bit binary number that holds the state of both pins
     00 - something is wrong we must have got here with a key bounce
     01 - sames as above - first bit should never be 0
     10 - knob was turned backwards
     11 - knob was turned forwards
     */   
  /* We can pull a value out of our truth table and add it to the current level_A */
  level_R = level_R + bump[state];
  /* Let's see what happened */
  //Serial.print(bits[state] + "    ");  // show us the two bits
  //Serial.print(bump[state],DEC);       // show us the direction of the turn
 // Serial.print("    ");
}
//**********************************************************
double encoder_speedL(int a){
  float durationA_RIS, durationA_FALL, durationB_RIS,  durationB_FALL, durationA,  durationB, RPMA, RPMB, RPM, Velocity;
  durationA_RIS = pulseIn(pinA_L, HIGH, 24192);         //  60/(duration*2*1e-6*248) = RPM  -->let RPM = 5(最小值可讀取的值)  -->所以最大duration  = 24192
  durationA_FALL = pulseIn(pinA_L, LOW, 24192);
  //durationB_RIS = pulseIn(pinB_L, HIGH, 24192);
  //durationB_FALL = pulseIn(pinB_L, LOW, 24192);
  durationA = durationA_RIS + durationA_FALL;
  //durationB = durationB_RIS + durationB_FALL;
  durationA = durationA * 0.000001;                      //F=N*R/60    //F:frequency頻率(HZ)  
  //durationB = durationB * 0.000001;                                    //N:speed速度(RPM)
  if(durationA  ==  0){
       RPMA  = 0;
    }
    else { RPMA  = 60  / (durationA  * 248); }                      //R:resolution精準度(pulses/rev)
   
    // if(durationB  ==  0){
      // RPMB  = 0;
    //}
    //else { RPMB  = 60  / (durationB  * 248); }                      //R:resolution精準度(pulses/rev)
  //RPM = (RPMA + RPMB) / 2;              //平均
  Velocity  = RPMA * 0.41888;          //換算成車子行進速度 cm/s           Velocity  = RPM * 8 * pi / 60  = RPM * 0.41888
 switch(a){
   case 0:
    return RPMA;
    break;
   case 1:
   return Velocity;
   default:
    break;
  }
}
//**********************************************************
double encoder_speedR(int a){
  float durationA_RIS, durationA_FALL, durationB_RIS,  durationB_FALL, durationA,  durationB, RPMA, RPMB, RPM, Velocity;
  durationA_RIS = pulseIn(pinA_L, HIGH, 24192);         //  60/(duration*2*1e-6*248) = RPM  -->let RPM = 5(最小值可讀取的值)  -->所以最大duration  = 24192
  durationA_FALL = pulseIn(pinA_L, LOW, 24192);
  //durationB_RIS = pulseIn(pinB_L, HIGH, 24192);
  //durationB_FALL = pulseIn(pinB_L, LOW, 24192);
  durationA = durationA_RIS + durationA_FALL;
  //durationB = durationB_RIS + durationB_FALL;
  durationA = durationA * 0.000001;               //  F  = N * R / 60  // F:frequency頻率(HZ)  
  //durationB = durationB * 0.000001;                                    // N:speed速度(RPM)
    if(durationA  ==  0){                                              //R:resolution精準度(pulses/rev)
       RPMA  = 0;
    }
    else { RPMA  = 60  / (durationA  * 248);}                     
   
    // if(durationB  ==  0){
      // RPMB  = 0;
    //}
    //else { RPMB  = 60  / (durationB  * 248); }                      //R:resolution精準度(pulses/rev)
  //RPM = (RPMA + RPMB) / 2;              //平均
  Velocity  = RPMA * 0.41888;          //換算成車子行進速度 cm/s           Velocity  = RPM * 8 * pi / 60  = RPM * 0.41888
 switch(a){
   case 0:
    return RPMA;
    break;
   case 1:
   return Velocity;
   default:
    break;
  }
}
//**********************************************************
int Encoder_feedback(int a,  int b){
  int diff;
  Encoder_speedL = encoder_speedL(1);          //PS: encoder_speed(0);     //rotary speed   RPM
  Encoder_speedR = encoder_speedR(1);         // PS: encoder_speed(1);     //Speed   cm/s
  if(Encoder_speedL > Encoder_speedR)   {diff  =  Encoder_speedL  - Encoder_speedR;              //左輪速度大於右輪
                                         Com  =  b/diff;
                                         lcd.clear(); lcd.print("Com="); lcd.print(Com); Delay(1000);
                                         Serial.print("Com=");
                                         Serial.println(Com);
                                         return 1;}
  else if(Encoder_speedR > Encoder_speedL)   {diff  =  Encoder_speedR  - Encoder_speedL;          //右輪速度大於左輪
                                              Com  =  a/diff;
                                              lcd.clear(); lcd.print("Com="); lcd.print(Com); Delay(1000);
                                              Serial.print("Com=");
                                              Serial.println(Com);
                                              return 2;}
                                              
}
//**********************************************************
void Pulse_RESET(){
  level_L = 0;
  level_R = 0;
}
//**********************************************************

int Getdis(){
  float dis;
  dis = (-level_L  + level_R)/2  * 0.11;
  return dis;
}
//**********************************************************
void Rundis(float D, int speedL, int speedR){
  float disi = 0, disf  = 0;
  disf = (-level_L  + level_R)/2 * 0.11;
  disi = disf;
  lcd.clear();
if(D > 0){
    motorStart(speedL, speedR, true, true);
  while(disf - disi < D){                                               //逆轉
  disf = (-level_L  + level_R)/2 * 0.11;
 // Serial.print("disi="); Serial.print(disi);
 // Serial.print(" disf="); Serial.println(disf);
  lcd.setCursor(0,0);lcd.print("disi=");lcd.print(disi);lcd.print(" ");
  lcd.setCursor(0,1);lcd.print("disf=");lcd.print(disf);lcd.print(" ");
    }
}

else if(D < 0){
    motorStart(speedL, speedR, false, false);
  while(disf - disi > D){
  disf = (-level_L  + level_R)/2 * 0.11;
 // Serial.print("disi="); Serial.print(disi);
//   Serial.print(" disf="); Serial.println(disf);
 lcd.setCursor(0,0);lcd.print("disi=");lcd.print(disi);lcd.print(" ");
 lcd.setCursor(0,1);lcd.print("disf=");lcd.print(disf);lcd.print(" ");
  }
}
lcd.clear();
}

//**********************************************************
void Encoderprint(){      
 lcd.setCursor(0, 1); // 將游標移動第0欄、第1列。
 lcd.print("PL=");
 lcd.print(-level_L);
 lcd.setCursor(8, 1); 
 lcd.print("PR=");
 lcd.print(level_R);
//Serial.print(level_L);
//Serial.print(" ");
//Serial.print(level_R);
//Serial.println(" ");
}
//**********************************************************
void Encoder_Speedprint(float speedL,float speedR){
  /*
 lcd.setCursor(0, 1); // 將游標移動第0欄、第1列。
 lcd.print("VL=");
 lcd.print(speedL);
 lcd.setCursor(8, 1); 
 lcd.print("VR=");
 lcd.print(speedR);
*/
Serial.print("EncRPML=");
Serial.print(speedL);
Serial.print(",");
Serial.print("EncRPMR=");
Serial.println(speedR);
}
//Encoder走直線程式碼***********************************************************
void Encoder_line(double setpoint,  boolean L, boolean R){
int Input = 0;
 Input  = -level_L - level_R;
   if(Input >= 0){
      LeftSpeed--;           if(LeftSpeed < 35)    {LeftSpeed = 35;}
      RightSpeed++;          if(RightSpeed  > 100)    {RightSpeed = 100;}
      }
    else if(Input < 0){
      LeftSpeed++;               if(LeftSpeed > 100)    {LeftSpeed = 100;}
      RightSpeed--;        if(RightSpeed  < 35)    {RightSpeed = 35;}
      }
      setMotorLeft(LeftSpeed,  L);
      setMotorRight(RightSpeed,  R);     
       Serial.print("IN=");        Serial.print(Input);
 Serial.print(" LeftSpeed=");        Serial.print(LeftSpeed);
Serial.print(" RightSpeed=");        Serial.println(RightSpeed);
   

}
//*****************************************************************************************************************************************************************//

