void display_init() /* tft display initialize */
{
  tft.begin(identifier);
  tft.setRotation(NORMAL_ROTATE);    // normal rotatation = 1
  tft.fillScreen(BLACK);             // background color
  
  #define beep_start_delay 55
  #define beep_start_count 5
  for(int i=0; i<beep_start_count; i++)
  {
    sound(true);  delay(beep_start_delay);  
    sound(false); delay(beep_start_delay);
  }
}
void setText(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t size) /* easy to use text display */
{
   tft.setCursor(x, y);
   tft.setTextColor(text_color, bg_color); 
   tft.setTextSize(size);
}
void count_display_setup() /* counter text layout setup */
{ 
  setText(origin_counter[0]-190, origin_counter[1], COLOR_TIMER, BLACK, 5); tft.print("COUNT:");
}
void info_display_setup()  /* infomation text layout setup */
{
  setText(origin_info[0],    origin_info[1],    BLACK,WHITE, 2); tft.print(" ABILITY : ");
  setText(origin_info[0],    origin_info[1]+20, RED,  BLACK, 2); tft.print("           ");
  setText(origin_info[0]+72, origin_info[1]+60, RED,  BLACK, 2); tft.print("   Hz");
  setText(origin_info[0],    origin_info[1]+60, RED,  BLACK, 2); tft.print("0.00 ");
  setText(origin_info[0],    origin_info[1]+40, BLACK,WHITE, 2); tft.print("  COUNT  : ");
  setText(origin_info[0],    origin_info[1]+85, BLACK, CYAN, 1); tft.print("                                FIRMWARE UPDATE:v2.0/11_12_20   ");
}
void guide_switch_setup() /* box guide switch layout setup */
{
  tft.setRotation(GUIDE_SW_ROTATE); // rotation guide switch text only 
  setText(origin_switch[0],     origin_switch[1]   , BLACK, CYAN, 2); tft.print("<< Decrease  ");
  setText(origin_switch[0]+160, origin_switch[1]   , BLACK, CYAN, 2); tft.print(" Increase >>");
  setText(origin_switch[0],     origin_switch[1]-20, WHITE, RED,  2); tft.print("  - Reset -  ");
  setText(origin_switch[0]+160, origin_switch[1]-20, WHITE, BLUE, 2); tft.print(" Start/Stop ");                     
  tft.setRotation(NORMAL_ROTATE);   // restore rotation setting
}
void info_display_update()  /* refresh information display */
{
  if(current_time > 0)
  {
    float countsec = counter_value/float(current_time)*1000.0f; 
    setText(origin_info[0], origin_info[1]+60, RED, BLACK, 2); tft.print(countsec);
  }
}
void info_display_update(int range) /* refresh information (ability scale) display */
{
       if(range == 0){ setText(origin_info[0],  origin_info[1]+20, RED, BLACK, 2); tft.print("PERFECT    "); }
  else if(range == 1){ setText(origin_info[0],  origin_info[1]+20, RED, BLACK, 2); tft.print("POOR       "); }
//  else if(range == 2){ setText(origin_info[0],  origin_info[1]+20, RED, BLACK, 2); tft.print("GOOD       "); }
//  else if(range >= 3){ setText(origin_info[0],  origin_info[1]+20, RED, BLACK, 2); tft.print("FAIR       "); }
}
void count_display_update() /* refresh counter value display */
{
  if(counter_value < 10) 
  {
    setText(origin_counter[0],    origin_counter[1], COLOR_COUNTER,  BLACK, 16); tft.print("0");
    setText(origin_counter[0]+96, origin_counter[1], COLOR_COUNTER,  BLACK, 16); tft.print(counter_value);
  }
  else
  {
    setText(origin_counter[0],  origin_counter[1], COLOR_COUNTER,  BLACK, 16); tft.print(counter_value);
  }
}
void guide_switch_update(uint8_t port) /* refresh press button display */
{
  tft.setRotation(4); // guide switch text only 
  
       if(port == sw_port_2){ setText(origin_switch[0],     origin_switch[1],    BLACK, WHITE, 2); tft.print("<< Decrease  "); }
  else if(port == sw_port_1){ setText(origin_switch[0]+160, origin_switch[1],    BLACK, WHITE, 2); tft.print(" Increase >>");  }
  else if(port == sw_port_4){ setText(origin_switch[0],     origin_switch[1]-20, BLACK, WHITE, 2); tft.print("  - Reset -  "); }
  else if(port == sw_port_3){ setText(origin_switch[0]+160, origin_switch[1]-20, BLACK, WHITE, 2); tft.print(" Start/Stop ");  } 
  
  tft.setRotation(1); // restore setting
}  
