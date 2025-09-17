# dinoGame 🦖🎮
[Demo](https://youtube.com/shorts/N5O_s678hBg?feature=share)

## ⚡ TL;DR / Summary

DinoGame is a tactile Arduino-based reimagining of the Google Chrome Dinosaur Game, built on an Arduino Uno with an LCD shield, buzzer, tricolor LED, and breadboard components. I designed it during Summer 2025 after completing [EE109: Introduction to Embedded Systems at USC](https://bytes.usc.edu/files/ee109/documents/EE109_Syllabus.pdf), where I gained hands-on experience with interrupts, timers, EEPROM, and PWM.

Key features include:

- 🕹️ Dodging-based gameplay on a 2-row LCD screen
- 🎲 Seeded randomization for obstacle generation
- 💾 EEPROM-backed high scores (persistent even after power-off)
- 🌈 Dynamic LED feedback with PWM brightness control
- 🎶 Level-up buzzer chimes and progressive difficulty

Challenges included debouncing real buttons, configuring 16-bit timers, managing limited EEPROM space, and experimenting with [custom LCD characters](https://arduinointro.com/articles/projects/create-custom-characters-for-the-i2c-lcd-easily). The process reinforced my embedded systems foundation and gave me a tactile, fun game to share.

## 🎓 Context

In Spring 2025, I took a very engaging and interesting class called [Intro to Embedded Systems (EE109)](https://bytes.usc.edu/files/ee109/documents/EE109_Syllabus.pdf). I liked it so much that I ended up switching my major from pure Computer Science to Computer Engineering and Computer Science (after talking it over with Prof. Weber).

Throughout the course, we built some really fun projects with the Arduino Uno and an LCD shield — including a temperature monitoring system. At the end of the class, I was supposed to return my project kit. But (partially out of laziness and partially because I didn’t want to give up the cool tools I’d been playing with), I delayed. After another conversation with Prof. Weber, he let me keep the kit over the summer to build something on my own.

This project — *dinoGame* — is the result. It’s inspired by the Google Chrome dinosaur game, although I promise I wasn’t playing it in class. The following is the story of my implementation, challenges, and insights.

## ⚙️ Implementation

My goal was to use as many concepts from EE109 as possible. Because the LCD had only two rows, jumping wasn’t feasible, so I implemented a dodging mechanic: obstacles would move from right to left, and the player could dodge up and down.

I first tried using the built-in LCD buttons to keep the setup compact. But once I decided to add an LED and buzzer, I moved to a breadboard with larger buttons. Once I started having more and more feature ideas I created a list at the top of my code:

- LED with changing colors (red/green with adjustable brightness)
- Local memory storage for high score (persisting after power loss)
- Increasing obstacle speed
- Seed function for randomized obstacles
- Buzzer (sound effect for level-ups)
- Real buttons
- Designed/custom obstacles
- Scoring system
- (with space for new ideas as they came)

I started with the basics: the player (A) dodging symmetrical obstacles (m and w). To keep it simple I made it so there was a 10% chance of an obstacle appearing, which could result in “impossible walls" (more later).

When I transitioned to breadboard buttons, I quickly realized I’d forgotten about debouncing. That sent me back to the [EE109 documentation](bytes.usc.edu/ee109/labs) to relearn interrupts and button handling. After rewriting more code than expected, I finally got it working.

Next came scoring. I used timer1 (a 16-bit timer) so that a player could theoretically play for up to 1000 minutes without issues. Initially, the score increased every second, but I would usually score ~14 points, so I changed it to every 10ms to get a more reasonable score (and I didn't think anyone would make it 100 minutes). Configuring this required revisiting both the EE109 notes and the [Arduino timer documentation](https://bytes.usc.edu/files/ee109/documents/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf).

After scoring, I implemented the tricolor LED: red for losing, and a green brightness that increased with progress. For the levelUp feature, I added a buzzer chime. I made it so you leveled up for every 10 seconds of gameplay and there were 8 levels (for a full octave, though I needed my sister's advice to implement this conceptually, see more below). Setting up the LED required re-learning resistor calculations and configuring [Pulse Width Modulation](https://bytes.usc.edu/files/ee109/documents/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf).

This became my development rhythm:

1. Think of a feature.
2. Try to implement it from memory.
3. Fail.
4. Review EE109 docs and Arduino references.
5. Make some progress.
6. Check Stack Overflow.
7. Eventually, succeed.

I used this exact process for implementing speed progression, the buzzer, high scores, EEPROM storage, a seed function, and attempted custom obstacles.

EEPROM was tricky — I wanted seed-specific high scores, but the memory was too small. Even with just 1,000 possible seeds and high scores potentially exceeding 1,000 points, the math quickly broke storage limits. I had 10,000 possible seeds and thought it was cooler to have more possibilites (but I may revisit this later).

The seed function was another challenge. First I used ChatGPT for the first and only time in this project. It suggested a “best” seed (1200). But when I tested it, I lost immediately. So, I coded my own solution, getting 0400, which worked slightly better but was still far from my personal high score. That’s when I discovered via [Stack Overflow](https://stackoverflow.com/questions/7115459/c-rand-and-srand-gets-different-output-on-different-machines) that srand can behave inconsistently across different environments. I still don’t have a full solution there.

Custom obstacles were my final major attempt. Each LCD pixel is 5×8, so I envisioned stalactites and stalagmites. I dove into the [Arduino custom character documentation](https://arduinointro.com/articles/projects/create-custom-characters-for-the-i2c-lcd-easily) and experimented for over an hour on multiple occasions because I really wanted this feature. But the methods seemed limited to functions in the Arduino IDE, and I couldn't figure it out. In hindsight, maybe with more time I could have cracked it (or maybe with ChatGPT).

## 💭 Reflection

This project was incredibly rewarding. It let me revisit nearly every EE109 concept — interrupts, timers, EEPROM, PWM — and put them into practice in a tactile, interactive game.

When I showed the game to friends and family, I got some helpful feedback:

- Most said the game started off too slow.
- Younger people (my sister, friends) picked up the controls quickly, while older players (my parents, grandpa) struggled with the button layout.
- My sister didn’t like the levelUp chime until I tweaked it into an octave-based chime.
- Nearly everyone took too long to recognize the A character as the player. This made me wish I had implemented custom characters.

Here are my future improvements:

- Faster starting speed
- More harmonious buzzer chime
- Simplified button layout
- Custom characters for clarity
- Smarter seed handling with seed-specific high scores

## ✨ Takeaways

- The value of Stack Overflow (especially when srand didn’t behave as expected).
- The fun of tactile, hardware-based games — they feel more engaging than screen-only projects.
- The importance of persistence — nearly every feature required me to fail, study, retry, and push through before succeeding.

Overall, this project deepened my love for embedded systems and confirmed that switching majors was the right call.
