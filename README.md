Project I built over the summer using my EE109 project kit (Thank you Prof. Weber for lending it to me so I could build cool things like this).
It was a lot of fun reviewing concepts from EE 109 like interrupts, EEPROM, and PWM. I tried to use as many of the concepts that I knew in this project and very much enjoyed it.

I ended up using all 3 timers of the arduino: 1 for the buzzer, 1 for the score, and 1 for the PWM for the LED. I was really glad I didn't have to start from complete scratch as I had some files for the lcd written already and had some other basic ISR calls I could reference. One of the hardest things to implement was honestly nothing and everything. It felt like any time I tried to do anything a bunch of problems came up, I had to think about it, check my previous labs and lab writeups, then check Stack Overflow, and then I finally fixed it. This includes adding the buttons to the breadboard (rather than using the LCD buttons), adding the LED, adding the Buzzer, adding speed, adding EEPROM capabilities, and adding the helper functions for the actual game functionality.

There are a few ways I could've made this game better, some of which I tried others which I did not (which may give me a reason to come back to this). I tried to add specialized obstacles but found it impossible without coding in the Arduino IDE so I had to use characters that were symmetrical in nature (I chose W and M to act as stalactites and stalagmites). The levelUp sound is not super elegant but my sister said it was good enough after telling me my previous version was bad so I left that as is. I left the obstacle generation as completely random (so walls could appear and make it impossible to pass). Funnily enough, somehow when I was first making this game I glitched through one of the walls but I was unable to replicate this move every other time I tried. The only time I used AI was to try to figure out the best seed but it didn't give me a good value, I decided to create my own program to determine this, and faced a bunch of issues (it just being wrong about how long one could go). I then checked Stack Overflow and people there said rand/srand are not good so I think my separate file was just coming up with different RNG's.

My biggest takeaways from this project are the value of Stack Overflow, the awesomeness of a highly tactile game, and the importance of persistance when debugging.

Overall this project took about 2 weeks of work spread out over a month and total working hours was probably around 25. I got my inspiration from the Dinosaur Game on Chrome.


step 1: add as much specifcity as possible!

my proposed order:

contextL i did ee109, we build stuff on arduino.. dino game is cool... i decided to try and build dino game on arduino

order of implementation:
1
2
3

reflection: overall the process was generally like this. here is a specific example of how i implemented on of the things in this list.

here are challenges i faced/ things i would want to implement in the future:
talk about the stuff your sister said, the specialized objects

overall it was a fun project, i enjoyed reviewing concepts from EE 109 like interrupts, EEPROM, and PWM. 

my main takeaways are

upload video to youtube

