void sensor_init() /* distance sensor initialize */
{
  setText(origin_pulse[0] , origin_pulse[1], COLOR_SENSOR_PULSE, MAGENTA, 5); tft.print("PULSE");
  sensor.setTimeout(500);
  if(!sensor.init()) 
  {
    #ifdef DEBUG_MODE
      Serial.println("ditance sensor : failed to detect");
    #endif
  }
  sensor.startContinuous(); 
}
int distance_raw() /* read raw value of distance sensor */
{
  return sensor.readRangeContinuousMillimeters();
}
int distance() /* process value of distance sensor */
{
  int bar = constrain(distance_raw(), minIn, maxIn);
  bar = map(bar, minIn, maxIn, minOut, maxOut);
  // Serial.println(bar);
  return bar;
}
void sensor_update() /* refresh push up value on screen */
{
   int dis = distance();
   int max_tolerance = 4;
   int min_tolerance = 2;
   
   if(state_counter == 0)
   {
      if(dis < minOut+min_tolerance)
      {
        state_counter++;
        ablity_value = dis; 
      }
   }
   if(state_counter == 1)
   {
      if(dis > maxOut-max_tolerance)
      {
        info_display_update(ablity_value);
        if(!sound_state) sound(true);
        count_increase();
        state_counter--;
      }
   }
}
void distance_display_update() /* refresh distance bar on screen */
{
  int bar = distance();
       if(bar == maxOut  ){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print(" "); setText(origin_pulse[0]+200 , origin_pulse[1], WHITE, BLACK, 5); tft.print("      ");}
  else if(bar == maxOut-1){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("  "); setText(origin_pulse[0]+230 , origin_pulse[1], WHITE, BLACK, 5); tft.print("     ");}
  else if(bar == maxOut-2){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("   "); setText(origin_pulse[0]+260 , origin_pulse[1], WHITE, BLACK, 5); tft.print("    ");}
  else if(bar == maxOut-3){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("    "); setText(origin_pulse[0]+290 , origin_pulse[1], WHITE, BLACK, 5); tft.print("   ");}
  else if(bar == maxOut-4){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("     "); setText(origin_pulse[0]+320 , origin_pulse[1], WHITE, BLACK, 5); tft.print("  ");}
  else if(bar == maxOut-5){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("      "); setText(origin_pulse[0]+350 , origin_pulse[1], WHITE, BLACK, 5); tft.print(" ");}
  else if(bar == maxOut-6){ setText(origin_pulse[0]+170 , origin_pulse[1], WHITE, GREEN, 5); tft.print("       "); } 
}
void switch_pin_setup() /* define pin mode on toggle switch */
{
  pinMode(sw_port_1, INPUT_PULLUP);
  pinMode(sw_port_2, INPUT_PULLUP);
  pinMode(sw_port_3, INPUT_PULLUP);
  pinMode(sw_port_4, INPUT_PULLUP);
}
void count_increase() /* mannual increase counter value */
{
  counter_value++;   
  if(counter_value == count_sound_active) sound_state = true;
  if(counter_value >  99) count_reset();
}
void count_subtract() /* mannual subtract counter value */
{
  counter_value--;
  if(counter_value < 0) count_reset();
}
void count_reset() /* reset counter value to zero */
{
  counter_value = 0;
}
boolean debounce(uint8_t port) /* debouncing switch process */
{
  boolean status = LOW;
  #define interval_time 10
   
  if(digitalRead(port) == LOW)
  {
    unsigned long start_timer = micros();
    while(micros()-start_timer < interval_time);
   
    while(digitalRead(port) == LOW)
    {
      if(state_seq == 0)
      {
       clock_update();
       sensor_update();
      }
      count_display_update();
      distance_display_update();
      guide_switch_update(port);
    }
    guide_switch_setup();
    status = HIGH;
  }
  return status;
}
void sound(bool status) /* control to sound */
{
  if(status)  analogWrite(buzzer_port,   5); // sound on
  else        analogWrite(buzzer_port, 255); // sound off
}   
