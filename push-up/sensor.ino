void count_add()
{
  count++;   
  if(count == count_activeSound) sound_state = true;
  if(count >  99) count_reset();
}
void count_subtract()
{
  count--;
  if(count < 0) count_reset();
}
void count_reset()
{
  count = 0;
}
void sensor_init()
{
  setText(origin_p[0] , origin_p[1], COLOR_p, MAGENTA, 5); tft.print("PULSE");
 
  sensor.setTimeout(500);
  if(!sensor.init()) Serial.println("Failed to detect and initialize sensor!");
  sensor.startContinuous(); 
}
int distance_raw()
{
  return sensor.readRangeContinuousMillimeters();
}
int distance()
{
  int bar = constrain(distance_raw(), minRange, maxRange);
  bar = map(bar, minRange, maxRange, minOut, maxOut);
  // Serial.println(bar);
  return bar;
}
void distance_display()
{
  int bar = distance();
       if(bar == maxOut  ){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print(" "); setText(origin_p[0]+200 , origin_p[1], WHITE, BLACK, 5); tft.print("      ");}
  else if(bar == maxOut-1){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("  "); setText(origin_p[0]+230 , origin_p[1], WHITE, BLACK, 5); tft.print("     ");}
  else if(bar == maxOut-2){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("   "); setText(origin_p[0]+260 , origin_p[1], WHITE, BLACK, 5); tft.print("    ");}
  else if(bar == maxOut-3){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("    "); setText(origin_p[0]+290 , origin_p[1], WHITE, BLACK, 5); tft.print("   ");}
  else if(bar == maxOut-4){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("     "); setText(origin_p[0]+320 , origin_p[1], WHITE, BLACK, 5); tft.print("  ");}
  else if(bar == maxOut-5){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("      "); setText(origin_p[0]+350 , origin_p[1], WHITE, BLACK, 5); tft.print(" ");}
  else if(bar == maxOut-6){ setText(origin_p[0]+170 , origin_p[1], WHITE, GREEN, 5); tft.print("       "); } 
}
boolean debounce(uint8_t port)
{
  boolean status = LOW;
  uint8_t interval = 50;
  if(port == sw_port_3) interval = 5; 
  
  if(digitalRead(port) == LOW)
  {
    delayMicroseconds(interval);
    if(digitalRead(port) == LOW)
    {
      while(digitalRead(port) == LOW)
      {
        if(state == 1)
        {
          update(); display(); delay(0);  
        }
        guide_switch_update(port);
      }
      guide_switch_setup();
      status = HIGH;
    }
  }
  return status;
}
void sensor_update()
{
   int dis = distance();
   int max_tolerance = 4;
   int min_tolerance = 2;
   
   if(sensor_state == 0)
   {
      if(dis < minOut+min_tolerance)
      {
        sensor_state++;
        ablity = dis; 
      }
   }
   if(sensor_state == 1)
   {
      if(dis > maxOut-max_tolerance)
      {
        info_update(ablity);
        if(!sound_state) sound(true);
        count_add();
        sensor_state--;
      }
   }
}
void sound(bool status)
{
  if(status)  analogWrite(buzzer_port,   5); // sound on
  else        analogWrite(buzzer_port, 255); // sound off
}   
