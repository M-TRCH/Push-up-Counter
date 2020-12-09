#include <Wire.h>
#include <VL53L0X.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>   
//----------------------- library -----------------------//                
VL53L0X sensor;
MCUFRIEND_kbv tft;
//------------------------ class ------------------------//                
#define LCD_CS A3   
#define LCD_CD A2  
#define LCD_WR A1  
#define LCD_RD A0 
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define COLOR_t WHITE
#define COLOR_p WHITE
#define COLOR_c YELLOW
//----------------------- display -----------------------//                
uint16_t origin_t[] = {155, 250};
unsigned long start_time = 0;
unsigned long current_time, prev_time, prev_sec, prev_min = 0;
//------------------------ clock ------------------------//                
#define sw_port_1 12   // Increase
#define sw_port_2 11   // Subtract
#define sw_port_3 13   // Start/Stop
#define sw_port_4 1    // Reset 
#define buzzer_port 10 // buzzer
uint16_t minRange = 150, maxRange = 500;
uint16_t minOut = 0, maxOut = 6;
uint16_t origin_p[] = {15, 190};
boolean rst_sw = false, state3_first = true;
//------------------------ sensor -----------------------//                
#define identifier 0x9486
#define baudrate 9600
uint16_t origin_c[] = {205, 25}; // count
uint16_t origin_i[] = {15, 80};  // info
uint16_t origin_s[] = {8, 430};  // switch
uint8_t state = 3, sensor_state = 0;
int8_t count = 0, ablity = 0;
//------------------------ system -----------------------//         
#define count_activeSound 22
unsigned long sound_timer     = 0;
unsigned long sound_totalTime = 0;
uint16_t sound_delay      = 1500;
uint16_t sound_interval   = 250;
boolean  sound_state      = false;
boolean  sound_on         = true;
boolean  sound_oneTime    = true;
//------------------------ sound ------------------------//           


void setup() 
{
  // Serial.begin(baudrate);  
  Wire.begin();
  
  pinMode(sw_port_1, INPUT_PULLUP);
  pinMode(sw_port_2, INPUT_PULLUP);
  pinMode(sw_port_3, INPUT_PULLUP);
  pinMode(sw_port_4, INPUT_PULLUP);
  //------- system setup -------//                
   
  display_init(); 
  guide_switch_setup(); // guide display
  //  clock_setup();   
  //------- Add-on setup -------//                
}
void loop() 
{  
  if(debounce(sw_port_1) && state == 1) count_add();
  if(debounce(sw_port_2) && state == 1) count_subtract();
  if(debounce(sw_port_3) && rst_sw == false)
  {
    state++;
    if(state > 3) state = 0; 
  }
  if(debounce(sw_port_4) && rst_sw == true) state++;
  //------- read switch -------//                
  

  if(state == 0) 
  {
    clock_start();
    state++;
  }
  else if(state == 1) update();
  else if(state == 2)
  {
    rst_sw = true;    
    state3_first = true;
  }
  else if(state == 3 && state3_first == true)
  {
    clock_setup();
    info_setup();
    count_reset();
    rst_sw = false;  
    state3_first = false;
  } 
  display();
  //------- state output -------//


  if(sound_state)
  {
    if(sound_oneTime)
    {
      sound_timer = millis();
      sound_oneTime = false;
      sound(sound_on);
    }
    if(millis()-sound_timer > sound_interval)
    {
      sound_oneTime = true;
      sound_on = !sound_on;
      sound_totalTime += sound_interval;
    }
    if(sound_totalTime >= sound_delay)
    {
      sound_totalTime = 0;
      sound_oneTime = true;
      sound_on      = true;
      sound_state   = false;
    }
  }
  else
  {
    sound(false);
  }
  //------- sound state -------//

}
