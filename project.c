#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "timers.h"
#include "project.h"

volatile bool selectPressed = 0;
volatile bool upPressed = 0;
volatile bool downPressed = 0;

// Helper functions
void debounce(uint8_t);
void shiftString(char*, char);
char generateObstacle(uint8_t);

uint16_t frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };

int main(void){
    // Initialize the LCD
    lcd_init();

    // Initialize Buttons
    DDRC &= ~ (1 << SELECT | 1 << UP | 1 << DOWN);
    PORTC |= (1 << SELECT | 1 << UP | 1 << DOWN);

    // Initialize LED
    DDRC |= (1 << RED);
    DDRB |= (1 << GREEN);

    PORTC |= (1 << RED);
    PORTB |= (1 << GREEN);

    // Initialize Buzzer
    DDRC |= (1 << BUZZER);

    // Initialize the timers
    timer0_init();
    timer1_init();
    timer2_init();

    // Enable interrupts
    sei();
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT9 | 1 << PCINT10 | 1 << PCINT11);


    // Write splash screen and delay for 1 second
    lcd_writecommand(1);
    lcd_moveto(0,0);
    lcd_stringout("Peyton Crawford");
    lcd_moveto(1,2);
    lcd_stringout("Dino Game");
    _delay_ms(2000);
    lcd_writecommand(1);

    // Load High Score / Name From EEPROM
    int8_t highScoreNum[9];
    int8_t highNameNum[3];
    char highScoreStr[10] = "        0";
    char highNameStr[4] = "   ";
    for(int i = 0; i < 9; i++){
        highScoreNum[i] = eeprom_read_byte((void *) i);
    }

    for(int i = 0; i < 3; i++){
        highNameNum[i] = eeprom_read_byte((void *) (10 + i));
    }

    // See if we got good data from EEPROM
    bool scoreClean = 1;
    for(uint8_t i = 0; i < 9; i++){
        if(highScoreNum[i] < 0 || highScoreNum[i] > 9)
            scoreClean = 0;
    }
    
    bool nameClean = 1;
    for(uint8_t i = 0; i < 3; i++){
        if(highNameNum[i] < 0 || highNameNum[i] > 26)
            nameClean = 0;
    }

    // If both are good, update strings and highScoreVal
    uint16_t highScoreVal = 0;
    if(scoreClean && nameClean){
        bool hadFirstNum = 0; // So we don't print leading 0's but do print trailing 0's
        // Convert highScoreNum to a string
        for(uint8_t i = 0; i < 9; i++){
            if(highScoreNum[i] != 0 || hadFirstNum){
                highScoreStr[i] = highScoreNum[i] + '0';
                hadFirstNum = 1;
            } else
                highScoreStr[i] = ' ';
        }
        highScoreStr[9] = '\0';
        highScoreVal = atoi(highScoreStr); //Value to compare score to when Game Over

        //Convert highNameNum to a string
        for(uint8_t i = 0; i < 3; i++){
            highNameStr[i] = highNameNum[i] + 'A';
        }
        highNameStr[3] = '\0';
    }

    // Display High Score
    lcd_writecommand(1);
    lcd_moveto(0,2);
    lcd_stringout("High Score");
    lcd_moveto(1,1);
    lcd_stringout(highNameStr);
    lcd_moveto(1,6);
    lcd_stringout(highScoreStr);
    lcd_moveto(1,16);
    _delay_ms(2000);
    lcd_writecommand(1);

    //Setup + Gameplay
    while(1){
        //Initialize/Reset variables
        bool playAgain = 0;
        uint8_t noteIdx = 0;
        score = 0;
        count = 0;
        speed = 0;
        char topOut[16] = "               ";
        char botOut[16] = "               ";
        bool topPast = 0;
        bool botPast = 0; 
        count = 0;
        uint8_t currPos = 0;

        // Set seed by choosing 4 digits
        lcd_moveto(0,2);
        lcd_stringout("Select Seed");
        lcd_moveto(1,5);
        char seed[5] = "0000";
        lcd_stringout(seed);
        uint8_t cursorIdx = 0;
        lcd_moveto(1,5 + cursorIdx);
        while(1){
            if(upPressed){
                if(seed[cursorIdx] == '9'){
                    seed[cursorIdx] = '0';
                } else {
                    seed[cursorIdx] += 1;
                }
                lcd_stringout(&seed[cursorIdx]);
                lcd_moveto(1,5 + cursorIdx);
                debounce(UP);
            } else if(downPressed){
                if(seed[cursorIdx] == '0'){
                    seed[cursorIdx] = '9';
                } else {
                    seed[cursorIdx] -= 1;
                }
                lcd_stringout(&seed[cursorIdx]);
                lcd_moveto(1,5 + cursorIdx);
                debounce(DOWN);
            } else if(selectPressed){
                debounce(SELECT);
                // Special way to reset the high score as needed (but only if there is one)
                if(highNameStr[0] != ' ' && ((PINC & (1 << UP)) == 0) && ((PINC & (1 << DOWN)) == 0)){
                    strcpy(highScoreStr, "        0");
                    strcpy(highNameStr, "   ");
                    highScoreVal = 0;
                    eeprom_update_byte((void *) 0, -1); // Bad value so it would go back to default if no new score

                    // Display message
                    lcd_writecommand(1);
                    lcd_moveto(0,0);
                    lcd_stringout("High Score Reset");

                    debounce(DOWN);
                    _delay_ms(2000);

                    //Return to how it should be
                    lcd_writecommand(1);
                    lcd_moveto(0,2);
                    lcd_stringout("Select Seed");
                    lcd_moveto(1,5);
                    lcd_stringout(seed);
                    lcd_moveto(1,5 + cursorIdx);
                } else if(cursorIdx != 3){
                    cursorIdx++;
                    lcd_moveto(1,5 + cursorIdx);
                } else {
                    srand(1000*(seed[3] - '0') + 100*(seed[2] - '0') + 10*(seed[1] - '0') + 1*(seed[0] - '0'));
                    lcd_writecommand(1);
                    break;
                }
            }   
        }
        
        // Display player and start timer
        lcd_moveto(0, currPos);
        lcd_stringout("A");
        lcd_moveto(1,16); // Move cursor off screen so it doesn't get in the way
        TCCR1B |= (1 << CS11 | 1 << CS10); //Sets prescalar to 64 for 10 ms period and starts timer

        // Gameplay
        while(1){
            // Check if Game Over
            if((currPos == 0 && topPast) || (currPos == 1 && botPast)){
                // Turn LED to just RED
                TCCR1B &= ~(1 << CS11 | 1 << CS10); //Sets prescalar to 0
                PORTC &= ~(1 << RED);
                OCR2A = 255;

                //Stop buzzer from playing chime if it is
                if(playingNote){
                    TCCR0B &= ~(0b111 << CS00); // Turn off timer
                    PORTC &= ~(1 << BUZZER); // Make sure buzzer is off
                    playingNote = 0;
                }

                lcd_writecommand(1);
                lcd_moveto(0,3);
                lcd_stringout("Game Over");

                play_note(73); _delay_ms(500); // Sync sound with text
                play_note(73); _delay_ms(500);

                lcd_moveto(1,0);
                lcd_stringout("Score:");

                play_note(62); _delay_ms(500); // Sync sound with text
                play_note(62); _delay_ms(500);

                char scoreStr[10];
                snprintf(scoreStr, 10, "%9d", score);
                lcd_stringout(scoreStr);
                
                play_note(52); _delay_ms(500); // Sync sound with text
                play_note(52); _delay_ms(500);
                
                lcd_stringout(">");
                lcd_moveto(1,15);
                while(1){
                    if(selectPressed){
                        debounce(SELECT);
                        break;
                    }
                }

                // Update highScore only if current score is greater
                if(score > highScoreVal){
                    strcpy(highScoreStr, scoreStr);
                    lcd_writecommand(1);
                    lcd_moveto(0,1);
                    lcd_stringout("New High Score!");
                    lcd_moveto(1,7);
                    lcd_stringout(scoreStr);
                    PORTC |= (1 << RED); //Turn off Red
                    // Play C chord C,E,G and sync brighter and brighter green
                    OCR2A = 245;
                    play_note(frequency[0]); _delay_ms(500);
                    
                    OCR2A = 200;
                    play_note(frequency[2]); _delay_ms(500);

                    OCR2A = 0;
                    play_note(frequency[4]); _delay_ms(500);

                    _delay_ms(1000);

                    lcd_writecommand(1);
                    lcd_moveto(0,2);
                    lcd_stringout("Enter Name");
                    lcd_moveto(1,6);

                    char name[4] = "AAA";
                    lcd_stringout(name);

                    //Select Name
                    uint8_t cursorIdx = 0;
                    lcd_moveto(1,6 + cursorIdx);
                    while(1){
                        if(upPressed){
                            if(name[cursorIdx] == 'Z'){
                                name[cursorIdx] = 'A';
                            } else {
                                name[cursorIdx] += 1;
                            }
                            lcd_stringout(&name[cursorIdx]);
                            lcd_moveto(1,6 + cursorIdx);
                            debounce(UP);
                        } else if(downPressed){
                            if(name[cursorIdx] == 'A'){
                                name[cursorIdx] = 'Z';
                            } else {
                                name[cursorIdx] -= 1;
                            }
                            lcd_stringout(&name[cursorIdx]);
                            lcd_moveto(1,6 + cursorIdx);
                            debounce(DOWN);
                        } else if(selectPressed){
                            if(cursorIdx != 2){
                                cursorIdx++;
                                lcd_moveto(1,6 + cursorIdx);
                                debounce(SELECT);
                            } else {
                                debounce(SELECT);
                                strcpy(highNameStr, name);
                                break;
                            }
                        }   
                    }

                    //Update EEPROM
                    for(int i = 0; i < 9; i++){
                        if(highScoreStr[i] != ' ')
                            eeprom_update_byte((void *) i, highScoreStr[i] - '0');
                        else
                            eeprom_update_byte((void *) i, 0); //Turn spaces into 0's
                    }
                    for(int i = 0; i < 3; i++){
                        eeprom_update_byte((void *) (10 + i), highNameStr[i] - 'A');
                    }
                }

                // Play Again Screen
                lcd_writecommand(1);
                lcd_moveto(0,2);
                lcd_stringout("Play Again?");
                lcd_moveto(1,6);
                lcd_stringout("Y  N");

                // Start Green on Y
                uint8_t pos = 0; // 0 for Y and 1 for N
                OCR2A = 0;
                PORTC |= (1 << RED); //Turn off Red
                while(1){
                    lcd_moveto(1,(6 + 3*pos));
                    //At most one is true
                    if(upPressed && pos == 1) {
                        debounce(UP);
                        pos = 0;
                        OCR2A = 0;
                        PORTC |= (1 << RED); //Turn off Red
                    } else if(downPressed && pos == 0) {
                        debounce(DOWN);
                        pos = 1;
                        PORTC &= ~(1 << RED);
                        OCR2A = 255;
                    }else if(selectPressed){
                        debounce(SELECT);
                        break;
                    }
                }

                if(pos == 0){ // Play Again
                    playAgain = 1;
                    PORTC |= (1 << RED); //Turn off Red
                    OCR2A = 255;
                }
                break;
            }

            //Player movement
            if((upPressed && currPos == 1) || (downPressed && currPos == 0)){
                lcd_moveto(currPos, 0);
                lcd_stringout(" ");
                currPos = (currPos + 1) % 2;
                lcd_moveto(currPos, 0);
                lcd_stringout("A");
                lcd_moveto(1,16); // Move cursor off screen so it doesn't get in the way
            }

            //Generate Chime Sound
            if(levelUp){
                if(noteIdx != 8)
                    play_note(frequency[noteIdx++]);
                levelUp = 0;
            }

            // Shift & Generate Obstacle
            if(count >= (100 - speed)){
                count = 0;
                // Functionality to go "past" player
                if(topPast){
                    topPast = 0;
                    lcd_moveto(0,0);
                    lcd_stringout(" ");
                }
                if(botPast){
                    botPast = 0;
                    lcd_moveto(1,0);
                    lcd_stringout(" ");
                }
                if(topOut[0] != ' '){
                    topPast = 1;
                    lcd_moveto(0,0);
                    lcd_stringout(&topOut[0]);
                }
                if(botOut[0] != ' '){
                    botPast = 1;
                    lcd_moveto(1,0);
                    lcd_stringout(&botOut[0]);
                }

                lcd_moveto(0,1);
                shiftString(topOut, generateObstacle(0));
                lcd_stringout(topOut);
                lcd_moveto(1,1);
                shiftString(botOut, generateObstacle(1));
                lcd_stringout(botOut);
                lcd_moveto(1,16); // Move cursor off screen so it doesn't get in the way
            }
        }
        lcd_writecommand(1);
        lcd_moveto(1,16);
        if(!playAgain) break;
    }

}

//Shifts string over one character and adds a new character at the end
void shiftString(char* s, char a){
    uint8_t size = strlen(s) + 1;
    char newStr[size];
    for(int i = 0; i < size - 2; i++){
        newStr[i] = s[i + 1];
    }
    newStr[size - 2] = a;
    newStr[size - 1] = '\0';
    strcpy(s, newStr);
}

// Uses a random number generator to either pick an obstacle or space
char generateObstacle(uint8_t row){
    if((rand() % 10) == 0){ // 10% chance of obstacle
        return row == 0 ? 'W' : 'M';
    } else {
        return ' ';
    }
}

// Debounce function
void debounce(uint8_t bit){
    _delay_ms(5);
    while((PINC & (1 << bit)) == 0);
    _delay_ms(5);
}

ISR(PCINT1_vect){ //For buttons on Port C
    if(((PINC & (1 << SELECT)) == 0))
        selectPressed = 1;
    else
        selectPressed = 0;
    if(((PINC & (1 << UP)) == 0))
        upPressed = 1;
    else
        upPressed = 0;
    if(((PINC & (1 << DOWN)) == 0))
        downPressed = 1;
    else
        downPressed = 0;
}