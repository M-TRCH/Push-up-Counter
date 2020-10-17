void display_init()
{
  tft.begin(identifier);
  tft.setRotation(1);
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
  setText(origin_c[0]-200, origin_c[1], COLOR_t, BLACK, 5); tft.print("COUNT:");
}
void count_display()
{
  if(count < 10) 
  {
    setText(origin_c[0],  origin_c[1], COLOR_c,  BLACK, 20); tft.print("0");
    setText(origin_c[0]+120,  origin_c[1], COLOR_c,  BLACK, 20); tft.print(count);
  }
  else
  {
    setText(origin_c[0],  origin_c[1], COLOR_c,  BLACK, 20); tft.print(count);
  }
}
void info_setup()
{
    setText(30,   80, BLACK, WHITE, 2); tft.print(" ABILITY : ");
    setText(30,  100, RED, BLACK, 2); tft.print("___________");
    setText(102, 140, RED, BLACK, 2); tft.print("count/sec");
    setText(30,  140, RED, BLACK, 2); tft.print("0.00");
    setText(30,  120, BLACK, WHITE, 2); tft.print("  COUNT  : ");
    setText(30,  165, BLACK,  CYAN, 1); tft.print("FIRMWARE UPDATE:v1.1/14_10_20");
}
void info_update()
{
    float countsec = count/float(current_time)*1000.0f;
    setText(30,  140, RED, BLACK, 2); tft.print(countsec);
}
void info_update(int range)
{
         if(range <  1){ setText(30,  100, RED, BLACK, 2); tft.print("PERFECT    "); }
    else if(range <  2){ setText(30,  100, RED, BLACK, 2); tft.print("GOOD       "); }
    else if(range <  3){ setText(30,  100, RED, BLACK, 2); tft.print("FAIR       "); }
}
