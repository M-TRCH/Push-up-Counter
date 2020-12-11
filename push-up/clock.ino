void clock_setup() /* clock text layout setup */
{   
  setText(origin_timer[0]    , origin_timer[1]   , COLOR_TIMER, BLACK, 5); tft.print("00:00:00"); 
  setText(origin_timer[0]-140, origin_timer[1]   , COLOR_TIMER, BLUE , 5); tft.print("TIME");
  setText(origin_timer[0]+10 , origin_timer[1]+45, RED        , BLACK, 2); tft.print("min");
  setText(origin_timer[0]+100, origin_timer[1]+45, RED        , BLACK, 2); tft.print("sec");
  setText(origin_timer[0]+185, origin_timer[1]+45, RED        , BLACK, 2); tft.print("msec");
}
void clock_start() /* start timer */
{   
   start_time = millis();         // start counter on raw data
   prev_time  = start_time;       // start counter on msec unit
   prev_sec   = start_time/1000;  // start counter on sec unit
   prev_min   = start_time/60000; // start counter on min unit
}
void clock_update() /* refresh time display */
{
   current_time = millis()-start_time-elapse_stop_time;       // calculate total elapsed time
   unsigned long dif_time = current_time-prev_time;      // difference time in msec unit 
   unsigned long dif_sec  = current_time/1000 -prev_sec; // difference time in sec unit
   unsigned long dif_min  = current_time/60000-prev_min; // difference time in min unit
  
   if(dif_time >= 1000)
   {
      info_display_update();            // data update is push up frequency (unit : Hz)
      prev_time = current_time; // update previous times (unit : ms)  
      setText(origin_timer[0]+180, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print("00"); // reset the screen on msec text position   
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if(dif_sec >= 60) // when more than 60 seconds will need to update the screen
      {
        prev_sec = current_time/1000; // update previous times (unit : sec)
        setText(origin_timer[0]+90, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print("00");

        /////////////////////////////////////////////////////////////////////////////////////////////
        if(dif_min >= 60)
        {
           prev_min = current_time/60000; // update previous times (unit : min) 
           setText(origin_timer[0], origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print("00");
        }
        else if(dif_min < 10) 
        {
         setText(origin_timer[0]   , origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print("0");
         setText(origin_timer[0]+30, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print(dif_min);
        }
        else 
        {
          setText(origin_timer[0], origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print(dif_min);    
        }
        /////////////////////////////////////////////////////////////////////////////////////////////
      }
      else if(dif_sec < 10) 
      {
        setText(origin_timer[0]+ 90, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print("0");
        setText(origin_timer[0]+120, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print(dif_sec);
      }
      else 
      {
        setText(origin_timer[0]+90, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print(dif_sec);    
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   }
   else
   {
     setText(origin_timer[0]+180, origin_timer[1], COLOR_TIMER, BLACK, 5); tft.print(int(dif_time/10)); // msec text position update
   }
}
