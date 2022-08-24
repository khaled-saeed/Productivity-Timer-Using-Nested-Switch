#include "Func.h"



LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void buzzing()
{
    for(int i = 0 ; i < 50 ; i++)
    {
        digitalWrite(PINS::BUZZER , !digitalRead(PINS::BUZZER));
        delay(1);
    }
    
}
void LCD_init()
{
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
}

ButtonState_t  previousState{RELEASED} ; 
ButtonState_t ButtonDebouncing(const int pin)
{
    if(digitalRead(pin) == PRESSED && previousState == PRESSED)
    {
        return PRESSED; 
    }
    else if(digitalRead(pin) == PRESSED && previousState == RELEASED)
    {
        previousState = PRESSED ; 
    }
    else if(digitalRead(pin) == RELEASED && previousState == PRESSED)
    {
        previousState = RELEASED ; 
    }
    else
    {
        return RELEASED; 
    }
}
void dispTime( uint32_t s)
{
    int mm = s/(int)60;
    int ss = s - mm*60 ;  
    int i = 10 ; 
    lcd.setCursor(i,1);
    while(ss>0 || i>8)
    {
        lcd.print(ss%10); 
        ss/=10; 
        i--; 
        lcd.setCursor(i,1); 
    }
    lcd.print(":");
    while(i>5)
    {
        i--;
        lcd.setCursor(i,1);
        lcd.print(mm%10); 
        mm/=10; 
    }
}
void handleEvent(superEvent_t event , ProTimer_t *proTimer)
{
    switch (proTimer->currentState)
    {
    case IDLE:  lcd.setCursor(4,0);
                lcd.print("SET TIME");
                dispTime(0); 
                proTimer->currentTime = 0 ; 
                proTimer->elapsedTime = 0 ; 
                static int buzzerCount = 0 ; 
                switch (event.sig)
                {
                case INCR:  if( proTimer->currentTime +60 <= 59940)
                            {
                                proTimer->currentState = TIME_SET; 
                                proTimer->currentTime += 60;
                                lcd.clear();
                                buzzerCount = 0 ; 
                            }
                            
                    break;

                case START_PAUSE:   proTimer->currentState = STAT ; 
                                    lcd.clear(); 
                                    
                    break;

                case TIME_TICK: if (buzzerCount <20 && event.ss%5 == 0)
                                    {
                                        buzzing(); 
                                        buzzerCount++ ;
                                    }
                                        
                    break;    
                default:
                    break;
                }
                
        break;
    case TIME_SET:
                lcd.setCursor(4,0);
                lcd.print("SET TIME");
                dispTime(proTimer->currentTime); 
                switch (event.sig)
                {
                case INCR:  if( proTimer->currentTime +60 <= 59940)
                            {
                                proTimer->currentTime += 60 ; 
                            }
                    break;
                case DECR:  if( proTimer->currentTime-60 > 0)
                            {
                                proTimer->currentTime -= 60 ; 
                            }
                            else
                            {
                                proTimer->currentTime = 0 ; 
                                proTimer->currentState = IDLE ; 
                                lcd.clear(); 
                            }
                    break;
                case START_PAUSE:   proTimer->currentState = COUNT_DOWN;   
                                    lcd.clear(); 
                    break;
                case ABRT:  proTimer->currentState = IDLE ; 
                            proTimer->currentTime = 0 ; 
                            lcd.clear(); 
                    break;
                
                default:
                    break;
                }
        break;
    case PAUSE:
                lcd.setCursor(4,0);
                lcd.print("Paused");
                dispTime(proTimer->currentTime); 
                switch (event.sig)
                {
                case INCR: if( proTimer->currentTime +60 <= 59940)
                            {
                                proTimer->currentState = TIME_SET; 
                                proTimer->currentTime += 60;
                                
                            }
                    break;
                case DECR:  if( proTimer->currentTime-60 > 0)
                            {
                                proTimer->currentTime -= 60 ; 
                                proTimer->currentState = TIME_SET ; 

                            }
                            else
                            {
                                proTimer->currentTime = 0 ; 
                                proTimer->currentState = IDLE ; 
                                lcd.clear() ; 
                            }
                            
                    break;
                case START_PAUSE:   proTimer->currentState = COUNT_DOWN ; 
                                    lcd.clear() ; 
                    break;
                case ABRT:  proTimer->currentState = IDLE ; 
                            proTimer->currentTime = 0  ; 
                            lcd.clear() ; 
                    break;
                  
                default:
                    break;
                }
        break;
    case COUNT_DOWN:
                
                lcd.setCursor(6,0) ;
                lcd.print("Time"); 
                dispTime(proTimer->currentTime); 
         
                switch (event.sig)
                {
                case START_PAUSE:
                                proTimer->proTime += proTimer->elapsedTime;
                                proTimer->elapsedTime = 0 ; 
                                proTimer->currentState = PAUSE;
                                 proTimer->proTime += proTimer->elapsedTime;
                                 proTimer->elapsedTime = 0 ; 
                                lcd.clear() ;  
                    break;
                case ABRT:  
                                proTimer->proTime += proTimer->elapsedTime;
                                proTimer->elapsedTime = 0 ; 
                                proTimer->currentState = IDLE ; 
                    break;
                case TIME_TICK: if(event.ss == 10) 
                                {
                                    proTimer->currentTime -= 1 ; 
                                    proTimer->elapsedTime += 1 ; 
                                    if(proTimer->currentTime == 0 )
                                    {
                                        proTimer->currentState = IDLE ; 
                                        proTimer->proTime += proTimer->elapsedTime;
                                        proTimer->elapsedTime = 0 ; 
                                    }
                                }
                    break;    
                default:
                    break;
                }
        break;
    case STAT:
                lcd.setCursor(1,0);
                lcd.print("Productive Time");
                dispTime(proTimer->proTime);
                switch (event.sig)
                {
                case TIME_TICK:if(event.ss ==10 )
                                {  proTimer->currentState = IDLE ; 
                                    lcd.clear();
                                } 
                    break;  
                default:
                    break;
                }
              

        break;
    default:
        break;
    }
}

