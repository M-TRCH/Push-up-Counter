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
uint16_t origin_t[] = {180, 250};
unsigned long start_time = 0;
unsigned long current_time, prev_time, prev_sec, prev_min = 0;
//------------------------ clock ------------------------//                
#define sw_port_1 12
#define sw_port_2 11
#define sw_port_3 13
#define sw_port_4 1
#define buzzer_port 10
uint16_t minRange = 150, maxRange = 500;
uint16_t minOut = 0, maxOut = 7;
uint16_t origin_p[] = {30, 190};
boolean rst_sw = false, state3_first = true;
//------------------------ sensor -----------------------//                
#define identifier 0x9486
#define baudrate 9600
uint16_t origin_c[] = {230, 25};
uint8_t state = 3, sensor_state = 0;
int8_t count = 0, ablity = 0;
//------------------------ system -----------------------//                



void setup() 
{
  //Serial.begin(baudrate);  
  Wire.begin();
  
  pinMode(sw_port_1, INPUT_PULLUP);
  pinMode(sw_port_2, INPUT_PULLUP);
  pinMode(sw_port_3, INPUT_PULLUP);
  pinMode(sw_port_4, INPUT_PULLUP);
  //------- system setup -------//                
   
  display_init(); 
  clock_setup();   
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

  
  sound(false);
}
/* 330 lines */
