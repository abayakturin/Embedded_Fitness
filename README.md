# Embedded_Fitness
Embedded fitness against COVID: using a STM32 micro-controller for workout

Real-time embedded systems

• Used C, Assembly and PlatformIO to program a STM32 board and its accelerometer

• The device detects situps, pushups, jumping jacks and squats and can be placed anywhere on the body

• The device indicates what specific exercise has occurred with the 4 LEDs and keeps count for all four exercises

I used the accelerometer on the STM32F407G-DISC1 board to get the readings along the X, Y, Z axis and the angles around plane X (roll) and around plane Y (pitch). I used this data to program the algorithms in the board. The board recognizes each exercise and proceeds to the programmed workout specified for the recognized exercise. Each exercise has an LED associated with it: Jumping Jacks - Blue LED, Pushups - Red LED, Situps - Orange LED, Sqauts - Green LED. You can program the number of sets and the number of repetitions for each exercise. You can also specify the number of repetitions it takes to trigger all 4 LEDs to indicate progress for each exercise.

Jumping Jacks Video Demo: https://youtu.be/EmRaBL-TCf0

Pushups Video Demo: https://youtu.be/gyXKUe-X6DY

Situps Video Demo: https://youtu.be/rZp9FuJeIPs

Squats Video Demo: https://youtu.be/VQekJc2J7xo

STM32F407G-DISC1: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
