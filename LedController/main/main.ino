#include <SPI.h>
#include "config.h"
#include "Button.h"
#include "Adafruit_TLC59711.h"

btn               BTN;
modes             Mode          = NONE;
brightness        Brightness;
last_update       LastUpdate;
update_delay      UpdateDelay;
Adafruit_TLC59711 LedController = Adafruit_TLC59711(NUM_TLC59711, clock, data);

Button  BtnMode1(BTN.BTN_1, BTN.DEBOUNCE);
Button  BtnMode2(BTN.BTN_2, BTN.DEBOUNCE);
Button  BtnMode3(BTN.BTN_3, BTN.DEBOUNCE);


void setup() {
  #ifdef DEBUGGING
    Serial.begin(BAUDRATE);
    Serial.println("[Main] Setup started");
  #endif

  BtnMode1.begin();
  BtnMode2.begin();
  BtnMode3.begin();
  LedController.begin();
  LedController.write();

  #ifdef DEBUGGING
    Serial.println("[Main] Setup complete");
  #endif
}

void loop() {
  readButtons();
  checkModes();
}

void checkModes(){
  if(Mode == MODE1){
    if(doIncreaseMode1){
      if(millis() - LastUpdate.Mode1 >= UpdateDelay.Mode1){
        if(Brightness.Mode1 != MAX_BRIGHTNESS){
          Brightness.Mode1 = MAX_BRIGHTNESS;
          updatePWM(1);
        }
        else{
          doIncreaseMode1 = false;
          return;
        }
        LastUpdate.Mode1 = millis();
      }
    }
    if(millis() - BtnMode1.lastReleased() >= 10000){
      if(Brightness.Mode1 != MAX_BRIGHTNESS){
        Brightness.Mode1 = MAX_BRIGHTNESS;
        updatePWM(1);
      }
    }
  }

  else if(Mode == MODE2 && (millis() - LastUpdate.Mode2 >= UpdateDelay.Mode2)){
    bool doUpdate = false;
    if(doIncreaseMode2){
      if(Brightness.Mode2 < MAX_BRIGHTNESS)
        Brightness.Mode2 += INCREMENT_VALUE;
      else
        Mode = NONE;
    }
    else{
      if(Brightness.Mode2 > MIN_BRIGHTNESS)
        Brightness.Mode2 -= DECREMENT_VALUE;
      else
        Mode = NONE;
    }
    if(Mode == MODE2){
      updatePWM(2);
    }
    LastUpdate.Mode2=  millis();
  }

  else if(Mode == MODE3 && BtnMode3.lastReleased() >= 10000) {
    if(millis() - LastUpdate.Mode3 >= UpdateDelay.Mode3){
      if(Brightness.Mode3 != MIN_BRIGHTNESS)
        Brightness.Mode3 -= DECREMENT_VALUE;
      if(Brightness.Mode3 < MIN_BRIGHTNESS)
        Brightness.Mode3 = MIN_BRIGHTNESS;
      if(Brightness.Mode3 == MIN_BRIGHTNESS)
        Mode = NONE;
      updatePWM(3);
      LastUpdate.Mode3 = millis();
    }
  }
}

void readButtons() {
  BtnMode1.read();
  BtnMode2.read();
  BtnMode3.read();

  if(BtnMode1.wasReleased()){
    if(Mode != MODE1 || Brightness.Mode1 == MIN_BRIGHTNESS){
      if(Mode != MODE1){
        Brightness.Mode2 = Brightness.Mode3 = MIN_BRIGHTNESS;
        updatePWM(2);
        updatePWM(3);
      }
      doIncreaseMode1 = true;
      Mode == MODE1;
      return;
    }
    else if(Brightness.Mode1 > MIN_BRIGHTNESS){
      Brightness.Mode1 -= DECREMENT_VALUE;
      doIncreaseMode1 = false;
    }
    updatePWM(1);
    Mode = MODE1;
    #ifdef DEBUGGING
      Serial.println("[Mode 1] Button pressed");
    #endif
  }

  else if(BtnMode2.wasReleased()){
    if(Mode != MODE2){
      Brightness.Mode1 = Brightness.Mode3 = MIN_BRIGHTNESS;
      updatePWM(1);
      updatePWM(3);
    }
    Mode = MODE2;
    if(Brightness.Mode2 != MIN_BRIGHTNESS)
      doIncreaseMode2 = true;
    else
      doIncreaseMode2 = false;   
    #ifdef DEBUGGING
      Serial.println("[Mode 2] Button pressed");
    #endif   
  }
  else if(BtnMode3.wasReleased()){
    if(Mode != MODE3){
      Brightness.Mode1 = Brightness.Mode2 = MIN_BRIGHTNESS;
      updatePWM(1);
      updatePWM(2);
    }
    Mode = MODE3;
    Brightness.Mode3 = random(MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    updatePWM(3);
    #ifdef DEBUGGING
      Serial.println("[Mode 3] Button pressed");
    #endif
  }
}

void updatePWM(uint8_t m){
  uint16_t c_brightness;
  switch(m){
    case 1:
      c_brightness = Brightness.Mode1;
      for(uint8_t i=0; i<SIZE_MODE1; i++)
        LedController.setPWM(i, c_brightness);
      break;
    case 2:
      c_brightness = Brightness.Mode2;
      for(uint8_t i=0; i<SIZE_MODE2; i++)
        LedController.setPWM(i, c_brightness);
      break;
    case 3:
      c_brightness = Brightness.Mode3;
      for(uint8_t i=0; i<SIZE_MODE3; i++)
        LedController.setPWM(i, c_brightness);
      break;
  }
  #ifdef DEBUGGING
    Serial.print("[Mode " + String(m) + "] Brightness: ");
    Serial.println(c_brightness);
  #endif
  LedController.write();
}
