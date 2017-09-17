/*陀螺儀 GY-521
◎ 使用晶片：MPU-6050
◎ 供電電源：3～5V（內部低壓差穩壓）
◎ 通信方式：標準IIC通信協定
◎ 晶片內置16bit AD轉換器 / 16位元資料輸出
◎ 陀螺儀範圍：±250 / 500 / 1000 / 2000°/s
◎ 加速度範圍：±2 / ±4 / ±8 / ±16g 
◎ 引腳間距2.54mm
//接繳:SCL A5      SDA A4
// MPU6050 
// http://www.giuseppecaccavale.it/
// Giuseppe Caccavale
 */
//******************************************************************************************************************************************************            
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added seamless Fastwire support
//                 - added note about gyro calibration
//      2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//      2012-06-20 - improved FIFO overflow handling and simplified read process
//      2012-06-19 - completely rearranged DMP initialization code and simplification
//      2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//      2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//      2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                 - add 3D math helper file to DMP6 example sketch
//                 - add Euler output and Yaw/Pitch/Roll output formats
//      2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//      2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//      2012-05-30 - basic DMP initialization working

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector




// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void GY521_setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(76);
    mpu.setYGyroOffset(8);    //-3720  -1140 779 76  8 -11
    mpu.setZGyroOffset(-11);
    mpu.setXAccelOffset(-3720);
    mpu.setYAccelOffset(-1140);
    mpu.setZAccelOffset(779); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(3, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output

}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
void getGY521(){   
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }
    mpu.resetFIFO();
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); 
            thetaR = -(ypr[0] * 180/M_PI);          //float的值
           // roll = ypr[1] * 180/M_PI;
           // pitch = ypr[2] * 180/M_PI;
        // blink LED to indicate activity

        blinkState = !blinkState;
        digitalWrite(DMP6_LED, blinkState);
    }
}
//**********************************************************
void Correct(int a, int b,  boolean L,  boolean R){                //a,b參數為改變的量
 int Max,Min,A;
 //static int n = 0;
 Max  = 160;   Min  = 32;
LeftSpeed += a;
RightSpeed += b;
 if(LeftSpeed  >= Max)  {LeftSpeed = Max;}   else if(LeftSpeed <= Min) {LeftSpeed  = Min;}
 if(RightSpeed  >= Max)  {RightSpeed = Max;}   else if(RightSpeed <= Min) {RightSpeed  = Min;}

 /*
 if(LeftSpeed == Max && RightSpeed == Min) {n++;}
 else if(LeftSpeed == Min && RightSpeed == Max) {n++;}
 if(n == 20)      {lcd.clear();
                  lcd.print("Error....."); 
                  lcd.setCursor(0,1);
                  lcd.print("Enter key to break..."); 
                  A = Enterkeypad();}*/
                  thetaprint();// speedprint();
 setMotorLeft(LeftSpeed, L);
 setMotorRight(RightSpeed, R);
}
//***********************************************************
void SimpleCorrect(int SpeedL, int SpeedR, int a, int b){
  theta = Devtheta(CaseGY521);
  if(theta  <= -1) {Correct(-a, b, true, true); Checklight(0, 1, 1);}
  else if(theta  >= 1)  {Correct(a, -b, true,true); Checklight(1, 0, 1);}
  
/*       //Have some problem-----> value will drift
  else if(theta > 5) { Checklight(1, 1, 1);
                        motorBrake(0, 0, true, true);
                        motor_spinAround(BaseL3, BaseR3, 0, theta); 
                        motorStart(SpeedL, SpeedR, true, true);
                       }
  else if(theta < -5){ Checklight(1, 1, 1);
                        motorBrake(0, 0, true, true);                     
                        motor_spinAround(BaseL3, BaseR3, 0, theta); 
                        motorStart(SpeedL, SpeedR, true, true);
                       }
*/  
   
  else if(theta  < 1 && theta > -1){
                                      Checklight(0, 0, 1);
                                      setMotorLeft(SpeedL, true);
                                      setMotorRight(SpeedR, true);
                                     }
                                    
  thetaprint();   //speedprint();

  
}
//***********************************************************
void motor_spinAround(int speedL,int speedR,float alpha, float ang){    //原地旋轉
  unsigned long past;
  past = millis();
  theta = Devtheta(CaseGY521);
  if(theta  <= alpha  && alpha >= 0){                        //左轉 
     if(alpha  - theta  >  20)  {motorStart(speedL , speedR , false , true);}
     else if(alpha  - theta  <  20)  {motorStart(BaseL3 , BaseR3 , false , true);}

     while(theta  < alpha){          //向左旋轉
       theta = Devtheta(CaseGY521);
       if(alpha  - theta  <  20)      {setMotorLeft(BaseL3 , false);
                                       setMotorRight(BaseR3 , true);}         //快到時煞車
 
       else{setMotorLeft(LeftSpeed , false);
            setMotorRight(RightSpeed , true);}
              thetaprint();  //speedprint();
        Blink_RGB_lightL(1, 1, 1);
      }
        RESET(ang); 
        motorBrake(0, 0, false, true); Delay(200);
        Pulse_RESET();
        RGB_lightL(0,0,0);
    }



  else if(theta  >= alpha  && alpha <= 0){            //右轉
      if(theta  - alpha  >  20) {motorStart(speedL , speedR , true , false);}
      else if(theta  - alpha  <  20) {motorStart(BaseL3 , BaseR3 , true , false);}
    while(theta  > alpha){            //向右旋轉
      theta = Devtheta(CaseGY521);
      if(theta  - alpha  <  20)      {setMotorLeft(BaseL3 , true);
                                      setMotorRight(BaseR3 , false);}         //快到時煞車
      else{setMotorLeft(LeftSpeed , true);
           setMotorRight(RightSpeed , false);}
      thetaprint();  //speedprint();
      Blink_RGB_lightR(1, 1, 1);
    }
  
  motorBrake(0, 0, true, false);    Delay(200);
  Pulse_RESET();
  RGB_lightR(0,0,0);
  RESET(ang); 
  thetaprint();  //speedprint();
  }
}
//**********************************************************
void motorTurn(int speedL,int speedR,int alpha){
  theta = Devtheta(CaseGY521);
      LeftSpeed = speedL;
      RightSpeed  = speedR;
  if(theta  <= alpha  && alpha >= 0){
    while(theta  <= alpha){      //向左旋轉
      theta = Devtheta(CaseGY521);
           setMotorLeft(LeftSpeed , true);
           setMotorRight(RightSpeed , true);
      thetaprint();
      Blink_RGB_lightL(1,0,0);
    }
    RGB_lightL(0,0,0);
  }

  else if(theta  >= alpha  && alpha <= 0){
    while(theta  >= alpha){           //向右旋轉
      theta = Devtheta(CaseGY521);
           setMotorLeft(LeftSpeed , true);
           setMotorRight(RightSpeed , true);
      thetaprint();
      Blink_RGB_lightR(0,1,0);
    }
    RGB_lightR(0,0,0);
  }
}
//***********************************************************
int RESET(int Dev){
  getGY521();
if(Dev == 999) {thetai = thetaR;}             //準位歸0
else{thetai += Dev;}
if(thetai > 0 && thetai < 180){
  return 1;
}
else if(thetai > -180 && thetai < 0){
  return 4;
}  
}
//**********************************************************
int Initial_GY521(){                    //尋找初始值
  lcd.clear();
while(thetai == 0){
  lcd.setCursor(0, 0);
  lcd.print("Initialize....");
  lcd.clear();
  getGY521();
  thetai = thetaR;
if(thetai > 0 && thetai < 180){
  return 1;
}
else if(thetai > -180 && thetai < 0){
  return 4; }

 }
}
//**********************************************************
float Devtheta(int state){                 
float dev, lastdev;                           
float a;                             
  switch(state){
    case 1:                      // 0~180
          //Serial.print("Case 1 ");  Serial.print("  ");
          getGY521();
          a = thetai - 180;
          if(thetaR > -180 && thetaR < a) {dev = (thetaR + 360) - thetai;}
          else{dev = thetaR - thetai;}
          
          //if(dev > 175 && flag_GY521 != 2) {flag_GY521 = 1;}                      //左轉
          //else if(dev < -175 && flag_GY521 != 1)  {flag_GY521 = 2;}                     //右轉

          if(flag_GY521 == 1){
            if(dev < -90 && dev > -180) {dev = dev + 360;} 
          }
          if(flag_GY521 == 2){
            if(dev > 90 && dev < 180) {dev = dev - 360;}
          }
          //thetaprint();
          //Serial.println(flag_GY521);
          //if((dev - lastdev) > 20 || (dev - lastdev) < -20){
             // lastdev = dev;
              //theta = Devtheta(CaseGY521);}
          return dev;
    break;

    case 4:                     //-90~-360
          //Serial.print("Case 4 "); Serial.print("  ");
          getGY521();
           a = thetai + 180;
          if(thetaR < 180 && thetaR > a) {dev = (thetaR - 360) - thetai;}
          else{dev = thetaR - thetai;} 

          //if(dev > 170 && flag_GY521 != 2) {flag_GY521 = 1;}                      //左轉
          //else if(dev < -170 && flag_GY521 != 1)  {flag_GY521 = 2;}                     //右轉

          if(flag_GY521 == 1){
            if(dev < -90 && dev > -180) {dev = dev + 360;} 
          }
          if(flag_GY521 == 2){
            if(dev > 90 && dev < 180) {dev = dev - 360;}
          }
          //thetaprint();
          //Serial.println(flag_GY521);
        //  if((dev - lastdev) > 20 || (dev - lastdev) < -20){
          //    lastdev = dev;
          //    theta = Devtheta(CaseGY521);}
          return dev;         
    break;

    default:
           //Serial.println("Error");
           lcd.setCursor(0, 0); lcd.print("Error");
    break;
  }
  
}

//**********************************************************
void thetaprint(){         
 lcd.setCursor(0, 0); // 將游標移動第0欄、第1列。        
 lcd.print("Z=");   
 lcd.print(theta);
 lcd.print(" ");
 //Serial.println(theta);
}
//*****************************************************************************************************************************************************************//
