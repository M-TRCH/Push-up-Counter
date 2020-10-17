void clock_setup()
{   
   setText(origin_t[0], origin_t[1], COLOR_t, BLACK, 5); tft.print("00:00:000");
   setText(origin_t[0]-150, origin_t[1], COLOR_t, BLUE, 5); tft.print("TIME");
   setText(origin_t[0]+10 , origin_t[1]+45, RED, BLACK, 2); tft.print("MIN");
   setText(origin_t[0]+100, origin_t[1]+45, RED, BLACK, 2); tft.print("SEC");
   setText(origin_t[0]+175, origin_t[1]+45, RED, BLACK, 2); tft.print("MILLISEC");
}
void clock_start()
{   
   start_time = millis();

   prev_time = start_time;
   prev_sec = start_time/1000;
   prev_min = start_time/60000;
}
void clock_update()
{
   current_time = millis()-start_time;

   
   unsigned long dif_time = current_time-prev_time;
   unsigned long dif_sec  = current_time/1000 -prev_sec;
   unsigned long dif_min  = current_time/60000-prev_min;
  
   if(dif_time >= 1000)
   {
      info_update();
      
      prev_time = current_time; 
      setText(origin_t[0]+180, origin_t[1], COLOR_t, BLACK, 5); tft.print("000");   
      
      if(dif_sec >= 60)
      {
        prev_sec = current_time/1000; 
        setText(origin_t[0]+90, origin_t[1], COLOR_t, BLACK, 5); tft.print("00");

        if(dif_min >= 60)
        {
           prev_min = current_time/60000; 
           setText(origin_t[0], origin_t[1], COLOR_t, BLACK, 5); tft.print("00");
        }
        else if(dif_min < 10) 
        {
         setText(origin_t[0]   , origin_t[1], COLOR_t, BLACK, 5); tft.print("0");
         setText(origin_t[0]+30, origin_t[1], COLOR_t, BLACK, 5); tft.print(dif_min);
        }
        else 
        {
          setText(origin_t[0], origin_t[1], COLOR_t, BLACK, 5); tft.print(dif_min);    
        }
      }
      else if(dif_sec < 10) 
      {
        setText(origin_t[0]+ 90, origin_t[1], COLOR_t, BLACK, 5); tft.print("0");
        setText(origin_t[0]+120, origin_t[1], COLOR_t, BLACK, 5); tft.print(dif_sec);
      }
      else 
      {
        setText(origin_t[0]+90, origin_t[1], COLOR_t, BLACK, 5); tft.print(dif_sec);    
      }
   }
   else
   {
      setText(origin_t[0]+180, origin_t[1], COLOR_t, BLACK, 5); tft.print(dif_time);
   }
}
