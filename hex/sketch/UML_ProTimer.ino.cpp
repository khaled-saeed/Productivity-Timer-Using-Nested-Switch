#include <Arduino.h>
#line 1 "d:\\Courses\\Embbedded and Microcontrollers\\Udemy_Embedded_System_Design_using_UML_State_Machines\\StateMachinePlayGround_Code\\UML_ProTimer\\UML_ProTimer.ino"
#include "Func.h"
uint64_t lastTime{};
uint32_t Delayms{300};
ButtonState_t incButtonState{RELEASED},decButtonState{RELEASED},startPauseButtonState{RELEASED};
ProTimer_t proTimer;
uint32_t startingTime{}; 
uint16_t oneSec{100} ; 
#line 8 "d:\\Courses\\Embbedded and Microcontrollers\\Udemy_Embedded_System_Design_using_UML_State_Machines\\StateMachinePlayGround_Code\\UML_ProTimer\\UML_ProTimer.ino"
void setup();
#line 19 "d:\\Courses\\Embbedded and Microcontrollers\\Udemy_Embedded_System_Design_using_UML_State_Machines\\StateMachinePlayGround_Code\\UML_ProTimer\\UML_ProTimer.ino"
void loop();
#line 8 "d:\\Courses\\Embbedded and Microcontrollers\\Udemy_Embedded_System_Design_using_UML_State_Machines\\StateMachinePlayGround_Code\\UML_ProTimer\\UML_ProTimer.ino"
 void setup()
{
    proTimer.currentState = IDLE;
    proTimer.proTime = 0 ; 
    pinMode(PINS::DEC_BUTT,INPUT);
    pinMode(PINS::INC_BUTT,INPUT);
    pinMode(PINS::START_PAUSE_BUTT,INPUT);
    pinMode(PINS::BUZZER,OUTPUT); 
    LCD_init();
}

 void loop()
{   
    static superEvent_t event ; 
   if(millis()- lastTime >= Delayms)
    {
        incButtonState = ButtonDebouncing(PINS::INC_BUTT) ;
        decButtonState = ButtonDebouncing(PINS::DEC_BUTT); 
        startPauseButtonState= ButtonDebouncing(PINS::START_PAUSE_BUTT); 
        
        if(incButtonState == PRESSED &&decButtonState == PRESSED )
        {
            event.sig = ABRT;
            handleEvent(event,&proTimer); 
            
        }
        if(incButtonState == PRESSED)
        {
            event.sig = INCR;
            handleEvent(event,&proTimer); 
            
        }
        if(decButtonState == PRESSED)
        {
            event.sig = DECR;
            handleEvent(event,&proTimer); 
            
        }
        if(startPauseButtonState == PRESSED)
        {
            event.sig = START_PAUSE;
            handleEvent(event,&proTimer); 
            
        }
        lastTime = millis(); 
    }
   
    if(millis() - startingTime >= oneSec)
    {
        event.ss++; 
        if(event.ss >= 11)
            event.ss = 1 ; 
        event.sig = TIME_TICK;
        handleEvent(event,&proTimer); 
        startingTime = millis() ; 
      
    }

    
    
}



