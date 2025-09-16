# DinoGame 🦖🎮

🎥 Demo Video: https://youtube.com/shorts/N5O_s678hBg?feature=share

## 📖 Project Story

In Spring 2025, I took Intro to Embedded Systems (EE109), a class that ended up reshaping my academic path. I enjoyed it so much that I switched my major from Computer Science to Computer Engineering and Computer Science (after talking with Prof. Weber).

In the course, we built projects with an Arduino Uno and LCD shield—everything from interrupt-driven systems to a temperature monitor. When the semester ended, I hesitated to return my kit. After another conversation with Prof. Weber, he let me keep it for the summer to build whatever I wanted.

That freedom—and some inspiration from the Chrome Dinosaur game—led me to create DinoGame, an embedded survival game played entirely on an LCD screen with tactile buttons, LEDs, and a buzzer.

This README is the story of how I built it, the challenges I faced, and the lessons I learned.

## ⚙️ Implementation Journey

I wanted to use as many EE109 concepts as possible. Here’s how the project evolved:

### Core Idea

-LCD has only 2 rows → no jumping, so I designed a dodge mechanic instead.

-Obstacles (like m and w) move right → left.

-Player (A) dodges up and down.

### Expanding Features

I created a feature wishlist at the top of my code:

- ✅ LED (red/green with brightness control)

- ✅ Local memory storage for high score (EEPROM)

- ✅ Increasing speed of obstacles

- ✅ Seed function for randomized runs

- ✅ Buzzer (level-up chime + game-over sound)

- ✅ Real tactile buttons (with debouncing & interrupts)

- ✅ Scoring system

- 🔲 Custom obstacles (attempted, see Challenges below)

I would pick a feature, try to implement it from memory, fail, recheck docs/Stack Overflow, make progress, and eventually succeed. That cycle became my core process for development.

### Notable Technical Challenges

- Debouncing & interrupts → Forgot how tricky they were; had to revisit class notes before finally stabilizing input.

- Timers → Configured timer1 (16-bit) for scoring to support long play sessions (up to 100 minutes) without overflow. timer0 was used for the buzzer. timer2 was used for PWM.

- PWM & LEDs → Spent significant time relearning PWM for LED brightness levels.

- EEPROM Storage → Wanted per-seed high scores but hit memory limits (EEPROM too small to store thousands of scores).

- Random Seeds → Learned that srand isn’t consistent across files/environments; debugging this was frustrating.

- Custom Characters → Spent hours on Arduino docs; discovered only IDE-specific functions supported it—so I had to cut this feature (for now).

## 📝 Reflection

This project helped me:

- Reinforce key EE109 topics (interrupts, EEPROM, PWM, timers).

- Develop a repeatable problem-solving process (idea → attempt → docs → Stack Overflow → persistence → solution).

- Experience the joy of building a tactile, physical game—something friends and family could immediately play.

### User Feedback & Insights

- Players thought the starting speed was too slow.

- Older testers took longer to figure out the button layout vs. younger ones.

- My sister didn’t like the buzzer at first; I iterated to make a more harmonious octave sound.

- Most players struggled to see that “A” was the player, reinforcing the need for custom characters.

### Future Improvements

- Faster initial speed.

- More harmonious buzzer chime.

- Simpler button layout.

- Seed-specific high scores (with optimized storage).

- Custom characters for better visuals.

## 🌟 Takeaways

- Persistence beats frustration—debugging was always a cycle of progress, even when it felt stuck.

- Stack Overflow is invaluable, but only after trying my own reasoning.

- Hands-on embedded systems create a different kind of joy than pure software—it feels alive.

## 🧑‍💻 Tech & Tools

- Hardware: Arduino Uno, LCD shield, breadboard, tri-color LED, buzzer, tactile buttons.

- Software: C.

- Concepts Used: Interrupts, debouncing, timers, PWM, EEPROM, pseudo-random seeds.

-------------------------------------------------------------------------------------------

That’s DinoGame—a playful experiment that turned into one of my most rewarding projects.
