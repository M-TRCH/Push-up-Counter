/*------------------------------------------ library -------------------------------------------*/     
#include <Wire.h>          // i2c library from arduino official 
#include <VL53L0X.h>       // distance sensor library (github : https://github.com/pololu/vl53l0x-arduino)
#include <MCUFRIEND_kbv.h> // display library (github         : https://github.com/prenticedavid/MCUFRIEND_kbv)
#include <Adafruit_GFX.h>  // display extension library       : https://github.com/adafruit/Adafruit-GFX-Library
/*------------------------------------------- object -------------------------------------------*/     
VL53L0X sensor;    // object for distance sensor
MCUFRIEND_kbv tft; // object for tft display
/*----------------------------------- tft display parameter ------------------------------------*/     
/* tft identifier */
#define identifier 0x9486
/* lcd pin */
#define LCD_CS A3   
#define LCD_CD A2  
#define LCD_WR A1  
#define LCD_RD A0
/* 8 bit color code */ 
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define COLOR_TIMER WHITE
#define COLOR_SENSOR_PULSE WHITE
#define COLOR_COUNTER YELLOW
/*-------------------------------------- timer parameter ---------------------------------------*/ 
uint16_t origin_timer[] = {155, 250}; // timer coordinate point (x, y)
unsigned long start_time   = 0, // start timer    (unit : ms) //        
              current_time = 0, // current timer  (unit : ms) // raw data timer //             
              prev_time    = 0, // previous timer (unit : ms)                   //
              prev_sec     = 0, // previous timer (unit : sec)
              prev_min     = 0, // previous timer (unit : min)
              elapse_stop_time    = 0; // There is to allow the counting of time from stop-start to continuous
/*------------------------------------- sensor parameter ---------------------------------------*/ 
/* input and output pin */
#define sw_port_1 12   // increase button pin 
#define sw_port_2 11   // subtract button pin
#define sw_port_3 13   // start/Stop button pin
#define sw_port_4 1    // reset button pin
#define buzzer_port 10 // buzzer pin
/* distance sensor parameter */
uint16_t minIn = 150, maxIn = 500,   // raw data range of distance sensor
         minOut = 0, maxOut = 6,     // process data range (for easy using)
         origin_pulse[] = {15, 190}; // pulse bar coordinate point (x, y)
/* conditions parameter */
boolean sw_start_ftime = true;       // conditions press start switch to first times
/*-------------------------------------- display setting ---------------------------------------*/ 
/* origin point */
#define NORMAL_ROTATE 1   // default rotation
#define GUIDE_SW_ROTATE 4 // virtual switch rotation on screen 
uint16_t origin_counter[] = {205, 25}; // origin counter text
uint16_t origin_info[]    = {15, 80};  // origin info text
uint16_t origin_switch[]  = {8, 430};  // origin switch text
/* conditions parameter */
int8_t state_seq      = -2, // state of sequence working 
       state_counter  = 0;  // state of counter (using when distance sensor detected)
int8_t counter_value  = 0,  // store push up counter (range 0-99 time)
       ablity_value   = 0;  // process ablity data 
/*-------------------------------------- sound parameter ---------------------------------------*/ 
#define count_sound_active 22 // number of times the sound is activated.
unsigned long sound_timer     = 0, // start timer for make sound
              sound_totalTime = 0; // total time to make sound
uint16_t sound_delay    = 1500,    // delay time to active sound
         sound_interval = 250;     // interval time for rhythm sound
boolean  sound_state    = false,   // state of sound sequence 
         sound_on       = true,    // determine of sound activation
         sound_ftime = true; // first state sound working  
/*------------------------------------- arduino parameter --------------------------------------*/ 
#define monitor_BitperSec 9600 // arduino uno recommended value
// #define DEBUG_MODE          // dedug mode is activate serial monitor 
/*----------------------------------------------------------------------------------------------*/ 

void setup() 
{
  #ifdef DEBUG_MODE
    Serial.begin(monitor_BitperSec);  
    while(!Serial); delay(2000);
    Serial.println("debug mode : on");   
  #endif
  Wire.begin(); // i2c communication start (use with distance sennsor)

  switch_pin_setup();    // set input pullup to switch pin
  display_init();        // tft display initiation
  sensor_init();         // distance sensor initiation
  count_display_setup(); // show counter text on display   
  guide_switch_setup();  // show guide switch text on display  
}
void loop() 
{  
/*--------------------------- read switch without debouncing switch ----------------------------*/   
  if(debounce(sw_port_1) && state_seq == 0) count_increase(); // counter value (+)
  if(debounce(sw_port_2) && state_seq == 0) count_decrease(); // counter value (-)
  if(debounce(sw_port_3) && (state_seq == -1 || state_seq == 0 || state_seq == 1)) 
  { /* start or stop action */
    if(sw_start_ftime) state_seq = 0;
    else               state_seq = !state_seq;
  }
  if(debounce(sw_port_4) && state_seq == 1) state_seq = -2; // reset action   
/*-------------------------------- sequence of system working ----------------------------------*/   
  if(state_seq == -2)  
  { /* reset state or waiting for start */
    clock_setup();         // show clock text on display
    info_display_setup();  // show info text on display
    count_reset();         // counter value set to zero
    sw_start_ftime = true; // set first time for state 0
    state_seq++;           // make the next state -1 (floating state)
  }
  if(state_seq == 0) 
  { /* running state */
    if(sw_start_ftime) 
    {
      elapse_stop_time = 0;   // set elapsed stop time to zero
      clock_start();          // start timer
      sw_start_ftime = false; // ending to the first time of state 0
    }
    clock_update();           // refresh timer text
    sensor_update();          // refresh push up counter text
  }
  if(state_seq == 1) 
  { /* stop state or waiting for reset */
    elapse_stop_time = millis()-current_time-start_time; // start timer stop time
  }
  count_display_update();     // refresh counter value on screen
  distance_display_update();  // refresh distance bar value on screen
/*------------------------------ sequence of beep-beep working -------------------------------*/   
  if(sound_state) 
  { /* an interruption occurred */
    if(sound_ftime) 
    { /* starting to timer of active sound */
      sound_timer = millis();
      sound_ftime = false;
      sound(sound_on);
    }
    if(millis()-sound_timer > sound_interval) 
    { /* interval time of switching on and off the sound */
      sound_ftime = true;
      sound_on = !sound_on;
      sound_totalTime += sound_interval;
    }
    if(sound_totalTime >= sound_delay) 
    { /* time out to active sound */
      sound_totalTime = 0;
      sound_ftime = true;
      sound_on      = true;
      sound_state   = false;
    }
  }
  else 
  { /* general case */
    sound(false);
  }
/*----------------------------------------------------------------------------------------------*/
} /* 415 lines | written by teerachot mueangchamnong | 11/12/20 */
