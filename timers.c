#include <avr/io.h>
#include <avr/interrupt.h>
#include "project.h"
#include "timers.h"

uint8_t count; // Generic count variable for time, score, and speed measuring
uint16_t score; // Max score 65536
uint8_t speed; // Ranges from 0 to 80
bool levelUp; // Used for buzzer chime
uint16_t fCnt; // Counter for ISR to know note is 0.5 sec
bool playingNote; // Used for making sure multiple notes don't play at the same time

// Plays sound on buzzer for half a second
void play_note(uint16_t freq)
{
 	//Initialize global variable so that note lasts half a second
    fCnt = freq;
 
     // Set modulus and start timer
    OCR0A = 16000000/1024/(2 * freq);
    TCCR0B |= (0b101 << CS00); // Prescalar = 1024
    
}

// Initialization for Timer0
void timer0_init()
{
    TCCR0B &= ~(0b111 << CS00); // Prescalar = 0
    TCCR0A |= (1 << WGM01); // CTC mode
    TIMSK0 |= (1 << OCIE0A); // Turn on interrupts
    OCR0A = 0;

}

// Initialization for Timer1
void timer1_init()
{
    // Add code to configure TIMER1 by setting or clearing bits in
    // the TIMER1 registers.
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS10); //Sets prescalar to 0
    OCR1A = 2500; //Sets modulus for timer
}

// Initialization for Timer2
void timer2_init()
    {
        TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
        TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
        OCR2A = 255;        // Start at dim
        TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
    }

//For Buzzer
ISR(TIMER0_COMPA_vect){
    PORTC ^= (1 << BUZZER);
    fCnt--;
    playingNote = 1;
    if(fCnt <= 0){ // Check if it's been 1 sec
        TCCR0B &= ~(0b111 << CS00); // Turn off timer
        playingNote = 0;
    }
}

// For score and LED
ISR(TIMER1_COMPA_vect) // Activates every 10 ms
{
    // Increment count
    count += 1;
    if((count % 10) == 0){
        score += 1;
        if(speed != 80 && (score % 10) == 0) speed += 1; // Increase speed every 10 points
        if((OCR2A != 247) && (OCR2A != 100) && ((score % 100) == 0)){ // Level up every 100 points
            OCR2A -= 1;
            levelUp = 1;
        } else if(OCR2A == 247) { // Just to make it extra bright
            OCR2A = 100; 
        }
    }
}
