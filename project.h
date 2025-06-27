#ifndef PROJECT_H
#define PROJECT_H

#define SELECT 1
#define UP 2
#define DOWN 3 
#define RED 4
#define GREEN 3
#define BUZZER 5

typedef uint8_t bool;

extern uint8_t count; // Generic count variable for time, score, and speed measuring
extern uint16_t score; // Max score 65536
extern uint8_t speed; // Ranges from 0 to 80
extern bool levelUp; // Used for buzzer chime
extern uint16_t fCnt; // Counter for ISR to know note is 0.5 sec
extern bool playingNote; // Used for making sure multiple notes don't play at the same time

#endif