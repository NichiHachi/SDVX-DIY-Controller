#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include <ESP32Encoder.h>

#include "../include/pinout.h"
#include "../include/keys.h"


USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

ESP32Encoder encoder_1;
ESP32Encoder encoder_2;

const int NUM_BTN = 7;

const int btn_key_list[NUM_BTN] = {
  BTN_SQUARE_1_KEY,
  BTN_SQUARE_2_KEY,
  BTN_SQUARE_3_KEY,
  BTN_SQUARE_4_KEY,
  BTN_RECT_1_KEY, 
  BTN_RECT_2_KEY, 
  BTN_SELECT_KEY
};

const char btn_pin_list[NUM_BTN] = {
  BTN_SQUARE_1_PIN,
  BTN_SQUARE_2_PIN,
  BTN_SQUARE_3_PIN,
  BTN_SQUARE_4_PIN,
  BTN_RECT_1_PIN, 
  BTN_RECT_2_PIN, 
  BTN_SELECT_PIN
};

bool last_state_btn[NUM_BTN];

// Debounce btn
int last_state_time_btn[NUM_BTN];
int time_now;
const int debounce_threshold = 5; //5ms

int last_encoder_1_value = 0;
int last_encoder_2_value = 0;
int encoder_1_value;
int encoder_2_value;


void setup() {
  USB.begin();
  Keyboard.begin();
  Mouse.begin();

  for (int i=0; i<NUM_BTN; i++){
    pinMode(btn_pin_list[i], INPUT_PULLUP);
    last_state_btn[i] = HIGH;
    last_state_time_btn[i] = 0;
  }

  encoder_1.attachFullQuad(ENCORDER_1_A_PIN, ENCORDER_1_B_PIN);
  encoder_2.attachFullQuad(ENCORDER_2_A_PIN, ENCORDER_2_B_PIN);

  encoder_1.setCount(0);
  encoder_2.setCount(0);
}

void loop() {
  time_now = millis();
  for (int i=0; i<NUM_BTN; i++){
    bool state = digitalRead(btn_pin_list[i]);
    if (time_now - last_state_time_btn[i] > debounce_threshold && state != last_state_btn[i]){
      if (state == LOW){
        Keyboard.press(btn_key_list[i]);
      } else {
        Keyboard.release(btn_key_list[i]);
      }
      last_state_btn[i] = state;
      last_state_time_btn[i] = time_now;
    }
  }

  encoder_1_value = encoder_1.getCount();
  encoder_2_value = encoder_2.getCount();
  
  if (encoder_1_value != last_encoder_1_value || encoder_2_value != last_encoder_2_value){
    Mouse.move(encoder_1_value - last_encoder_1_value, 
              encoder_2_value - last_encoder_2_value);

    last_encoder_1_value = encoder_1_value;
    last_encoder_2_value = encoder_2_value;
  }
}


