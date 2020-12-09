void display_init()
{
  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(WHITE);
  for(int i=0; i<5; i++)
  {
    sound(true);  delay(55);  
    sound(false); delay(55);
  }
  tft.fillScreen(BLACK);  
  
  sensor_init();
  count_setup();
  info_setup();
}
void display()
{ 
  count_display();
  distance_display();
}
void update()
{
  clock_update();
  sensor_update();
}
void setText(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t size)
{
   tft.setCursor(x, y);
   tft.setTextColor(text_color, bg_color); 
   tft.setTextSize(size);
}
void count_setup()
{ 
  setText(origin_c[0]-190, origin_c[1], COLOR_t, BLACK, 5); tft.print("COUNT:");
}
void count_display()
{
  if(count < 10) 
  {
    setText(origin_c[0],    origin_c[1], COLOR_c,  BLACK, 16); tft.print("0");
    setText(origin_c[0]+96, origin_c[1], COLOR_c,  BLACK, 16); tft.print(count);
  }
  else
  {
    setText(origin_c[0],  origin_c[1], COLOR_c,  BLACK, 16); tft.print(count);
  }
}
void info_setup()
{
    setText(origin_i[0],    origin_i[1],    BLACK,WHITE, 2); tft.print(" ABILITY : ");
    setText(origin_i[0],    origin_i[1]+20, RED,  BLACK, 2); tft.print("           ");
    setText(origin_i[0]+72, origin_i[1]+60, RED,  BLACK, 2); tft.print("   Hz");
    setText(origin_i[0],    origin_i[1]+60, RED,  BLACK, 2); tft.print("0.00 ");
    setText(origin_i[0],    origin_i[1]+40, BLACK,WHITE, 2); tft.print("  COUNT  : ");
    setText(origin_i[0],    origin_i[1]+85, BLACK, CYAN, 1); tft.print("                                FIRMWARE UPDATE:v1.2/09_12_20   ");
}
void info_update()
{
    float countsec = count/float(current_time)*1000.0f;
    setText(origin_i[0],  origin_i[1]+60, RED, BLACK, 2); tft.print(countsec);
}
void info_update(int range)
{
       if(range == 0){ setText(origin_i[0],  origin_i[1]+20, RED, BLACK, 2); tft.print("PERFECT    "); }
  else if(range == 1){ setText(origin_i[0],  origin_i[1]+20, RED, BLACK, 2); tft.print("POOR       "); }
//  else if(range == 2){ setText(origin_i[0],  origin_i[1]+20, RED, BLACK, 2); tft.print("GOOD       "); }
//  else if(range >= 3){ setText(origin_i[0],  origin_i[1]+20, RED, BLACK, 2); tft.print("FAIR       "); }
}
void guide_switch_setup()
{
  tft.setRotation(4); // guide switch text only 
  setText(origin_s[0],     origin_s[1]   , BLACK, CYAN, 2); tft.print("<< Subtract  ");
  setText(origin_s[0]+160, origin_s[1]   , BLACK, CYAN, 2); tft.print(" Increase >>");
  setText(origin_s[0],     origin_s[1]-20, WHITE, RED,  2); tft.print("  - Reset -  ");
  setText(origin_s[0]+160, origin_s[1]-20, WHITE, BLUE, 2); tft.print(" Start/Stop ");
                       
  tft.setRotation(1); // restore setting
}
void guide_switch_update(uint8_t port)
{
  tft.setRotation(4); // guide switch text only 
  
       if(port == sw_port_2){ setText(origin_s[0],     origin_s[1],    BLACK, WHITE, 2); tft.print("<< Subtract  "); }
  else if(port == sw_port_1){ setText(origin_s[0]+160, origin_s[1],    BLACK, WHITE, 2); tft.print(" Increase >>");  }
  else if(port == sw_port_4){ setText(origin_s[0],     origin_s[1]-20, BLACK, WHITE, 2); tft.print("  - Reset -  "); }
  else if(port == sw_port_3){ setText(origin_s[0]+160, origin_s[1]-20, BLACK, WHITE, 2); tft.print(" Start/Stop ");  } 
  
  tft.setRotation(1); // restore setting
}  
