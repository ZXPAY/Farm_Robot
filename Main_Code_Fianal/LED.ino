//Check LED宣告***********
  const int RLed  = 51;
  const int YLed  = 49;
  const int GLed  = 47;
  const int PinL_Red = A0;
  const int PinL_Green = A1;
  const int PinL_Blue = A2;
  const int PinR_Red = A3;
  const int PinR_Green = A4; 
  const int PinR_Blue = A5;

void LED_Setup(){
  pinMode(RLed, OUTPUT);         //Check LED
  pinMode(YLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(DMP6_LED, OUTPUT);
  pinMode(PinL_Red, OUTPUT);
  pinMode(PinL_Green, OUTPUT);
  pinMode(PinL_Blue, OUTPUT);
  pinMode(PinR_Red, OUTPUT);
  pinMode(PinR_Green, OUTPUT);
  pinMode(PinR_Blue, OUTPUT);
}

//Check LED 亮**********************************************************
void Checklight(boolean a, boolean b, boolean c){       //Check LED亮
  digitalWrite(RLed,a); 
  digitalWrite(YLed,b);
  digitalWrite(GLed,c);
}
//Check LED 閃爍**********************************************************
void Checklight_blink(){       //Check LED閃爍
  Checklight(1, 0, 0);           //紅  
  Delay(500);
  Checklight(0, 1, 0);            //黃
  Delay(500);
  Checklight(0, 0, 1);            //綠
  Delay(500);
  Checklight(0, 0, 0);   
}

//RGB Light Left**********************************************************
void RGB_lightL(int R, int G, int B){
  digitalWrite(PinL_Red, R);
  digitalWrite(PinL_Green, G);
  digitalWrite(PinL_Blue, B);
}
//**********************************************************
void Blink_RGB_lightL(boolean R, boolean G, boolean B){       //閃爍方向燈(左)，利用輪詢式方法
  static unsigned long past = millis();
  now = millis();
  if(now - past < 500){
    digitalWrite(PinL_Red, R);
    digitalWrite(PinL_Green, G);
    digitalWrite(PinL_Blue, B);
  }
  else if(now- past > 500 && now - past < 1000){
    digitalWrite(PinL_Red, LOW);
    digitalWrite(PinL_Green, LOW);
    digitalWrite(PinL_Blue, LOW);
  }
  else{past = now;}
}
//RGB Light Right**********************************************************
void RGB_lightR(int R, int G, int B){
  digitalWrite(PinR_Red, R);
  digitalWrite(PinR_Green, G);
  digitalWrite(PinR_Blue, B);
}
//**********************************************************
void Blink_RGB_lightR(boolean R, boolean G, boolean B){       //閃爍方向燈(右)，利用輪詢式方法
  static unsigned long past = millis();
  now = millis();
  if(now- past < 500){
    digitalWrite(PinR_Red, R);
    digitalWrite(PinR_Green, G);
    digitalWrite(PinR_Blue, B);
  }
  else if(now- past > 500 && now - past < 1000){
    digitalWrite(PinR_Red, LOW);
    digitalWrite(PinR_Green, LOW);
    digitalWrite(PinR_Blue, LOW);
  }
  else{past = now;}
}
//*****************************************************************************************************************************************************************//
