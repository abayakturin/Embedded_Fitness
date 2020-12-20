# Embedded_Fitness
Embedded fitness against COVID: using a STM32 micro-controller for workout

![alt text](https://www.st.com/bin/ecommerce/api/image.PF252419.en.feature-description-include-personalized-no-cpn-medium.jpg)

Real-time embedded systems

• Used C, Assembly and PlatformIO to program a STM32 board and its accelerometer

• The device detects situps, pushups, jumping jacks and squats and can be placed anywhere on the body

• The device indicates what specific exercise has occurred with the 4 LEDs and keeps count for all four exercises

I used the accelerometer on the STM32F407G-DISC1 board to get the readings along the X, Y, Z axis and the angles around plane X (roll) and around plane Y (pitch). 

![alt text](https://hackster.imgix.net/uploads/image/file/163265/no-name?auto=compress%2Cformat&w=740&h=555&fit=max)

I used this data to design the logic behind the program in the board. The board recognizes each exercise and proceeds to the programmed workout specified for the recognized exercise. The board triggers an LED tied to each specific exercise. Each exercise has an LED associated with it: Jumping Jacks - Blue LED, Pushups - Red LED, Sit-ups - Orange LED, Squats - Green LED. If you start doing Squats, the board will trigger Green LED to indicate that it has recognized this exercise, and the specified workout program for Squats will start after you press the USER button.

![alt text](https://ezcontents.org/sites/default/files/2019-05/PB240064.JPG)

You can program the number of sets and the number of repetitions for each exercise. You can also specify the number of repetitions it takes to trigger all 4 LEDs to indicate progress for each exercise. The program keeps track of the exercises that you have completed. Below you can see the log that outputs the relevant readings data along with the overall progress text message.

<img src="https://github.com/abayakturin/Embedded_Fitness/blob/master/Log.PNG" alt="drawing" width="300" position="absolute"/>

<bold>Below, you can find the video demos where I demonstrate how the board works</bold>

Jumping Jacks Video Demo: https://youtu.be/EmRaBL-TCf0

Pushups Video Demo: https://youtu.be/gyXKUe-X6DY

Situps Video Demo: https://youtu.be/rZp9FuJeIPs

Squats Video Demo: https://youtu.be/VQekJc2J7xo

STM32F407G-DISC1: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
