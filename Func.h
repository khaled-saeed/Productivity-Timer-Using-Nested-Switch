#ifndef __FUNC_H__
#define __FUNC_H__

#include <LiquidCrystal.h>
#include <Arduino.h> 

namespace PINS{
    const int INC_BUTT{6};
    const int DEC_BUTT{9};
    const int START_PAUSE_BUTT{8};
    const int BUZZER{10};
}
typedef enum {
    IDLE,
    TIME_SET,
    PAUSE,
    COUNT_DOWN,
    STAT
}State_t; 

typedef enum{
    RELEASED,
    PRESSED
}ButtonState_t;

typedef struct ProTimer
{
    State_t currentState ;
    uint32_t currentTime ; 
    uint32_t elapsedTime  ; 
    uint32_t proTime ; 
}ProTimer_t;
typedef enum{
    INCR,
    DECR,
    START_PAUSE,
    ABRT ,
    TIME_TICK
}event_t;

typedef enum{
    HANDLED,
    IGNORED,
    TRANSITION
}EventStatus_t;

typedef struct 
{
    uint8_t sig;
}userEvent_t;

typedef struct 
{
    event_t sig;
    uint8_t ss; 
}superEvent_t;

void buzzing();
void LCD_init();
ButtonState_t ButtonDebouncing(const int pin);
void dispTime( uint32_t s); 
void handleEvent(superEvent_t event , ProTimer_t *proTimer);
#endif