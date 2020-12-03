# Embedded_Fitness
Embedded fitness against COVID: using a STM32 micro-controller for workout

![alt text](https://www.st.com/bin/ecommerce/api/image.PF252419.en.feature-description-include-personalized-no-cpn-medium.jpg)

Real-time embedded systems

• Used C, Assembly and PlatformIO to program a STM32 board and its accelerometer

• The device detects situps, pushups, jumping jacks and squats and can be placed anywhere on the body

• The device indicates what specific exercise has occurred with the 4 LEDs and keeps count for all four exercises

I used the accelerometer on the STM32F407G-DISC1 board to get the readings along the X, Y, Z axis and the angles around plane X (roll) and around plane Y (pitch). 

![alt text](https://hackster.imgix.net/uploads/image/file/163265/no-name?auto=compress%2Cformat&w=740&h=555&fit=max)

I used this data to design the logic behind the program in the board. The board recognizes each exercise and proceeds to the programmed workout specified for the recognized exercise. The board triggers an LED tied to each specific exercise. Each exercise has an LED associated with it: Jumping Jacks - Blue LED, Pushups - Red LED, Situps - Orange LED, Sqauts - Green LED. If you start doing Squats, the board will trigger Green LED to indicate that it has recognized this exercise, and the specified workout program for Squats will start after you press the USER button.

![alt text](https://ezcontents.org/sites/default/files/2019-05/PB240064.JPG)

You can program the number of sets and the number of repetitions for each exercise. You can also specify the number of repetitions it takes to trigger all 4 LEDs to indicate progress for each exercise. The program keeps track of the exercises that you have completed. Below you can see the log that outputs the relevant readings data along with the overall progress text message.

Exercises completed so far:
Iteration # 1
Start X: 9055  Y: 9706  Z: 10857
The start angle is 228.551773
Finish X: 9071  Y: -5743  Z: -2577
The end angle is 24.227766
The difference between angles is 204.324005
-------------------------------------------
Exercises completed so far:
Iteration # 2
Start X: 16415  Y: -791  Z: 1830
The start angle is 292.619812
Finish X: 2571  Y: 15821  Z: -2444
The end angle is 171.369202
The difference between angles is 121.250610
-------------------------------------------
Detected Situps. Starting the workout...
Starting Situps...
Start X: 5617  Y: 14456  Z: 6504
The start angle is 204.274460
Finish X: 2641  Y: 16448  Z: 545
The end angle is 181.481461
The difference between angles is 22.792999
Start X: 17174  Y: -2806  Z: 2218
The start angle is 321.709808
Finish X: 5171  Y: 15803  Z: -2569
The end angle is 170.869812
The difference between angles is 150.839996
Start X: 14007  Y: -6797  Z: 6197
The start angle is 318.144928
Finish X: 6561  Y: 15186  Z: -3400
The end angle is 167.357834
The difference between angles is 150.787094
Rep # 1
Start X: 12869  Y: -8290  Z: 5805
The start angle is 324.743622
Finish X: 5008  Y: 15722  Z: -2321
The end angle is 171.797638
The difference between angles is 152.945984
Rep # 2
Start X: 12653  Y: -8574  Z: 6241
The start angle is 323.746124
Finish X: 8971  Y: 15220  Z: -886
The end angle is 176.790512
The difference between angles is 146.955612
Start X: 12892  Y: -7412  Z: 6785
The start angle is 317.290588
Finish X: 7018  Y: 15278  Z: -984
The end angle is 176.820160
The difference between angles is 140.470428
Rep # 3
Start X: 12578  Y: -8015  Z: 6863
The start angle is 319.398682
Finish X: 7474  Y: 15051  Z: -1376
The end angle is 175.146896
The difference between angles is 144.251785
Rep # 4
Start X: 12248  Y: -8547  Z: 7178
The start angle is 320.194397
Finish X: 8136  Y: 14821  Z: -390
The end angle is 178.908783
The difference between angles is 141.285614
Rep # 5
You have done 5 reps! Keep going! 5 reps left
Start X: 17305  Y: -2029  Z: -6170
The start angle is 71.965775
Finish X: 1241  Y: 16331  Z: -292
The end angle is 179.003662
The difference between angles is 107.037888
Start X: 9069  Y: -10348  Z: 8858
The start angle is 319.658203
Finish X: 12229  Y: 12096  Z: -819
The end angle is 176.633545
The difference between angles is 143.024658
Start X: 15053  Y: -4340  Z: 5300
The start angle is 309.035461
Finish X: 4535  Y: 15991  Z: -2636
The end angle is 170.949280
The difference between angles is 138.086182
Rep # 6
Start X: 13615  Y: -7229  Z: 5831
The start angle is 321.203430
Finish X: 5603  Y: 15541  Z: -745
The end angle is 177.397430
The difference between angles is 143.806000
Rep # 7
Start X: 14742  Y: -7075  Z: 3651
The start angle is 333.434937
Finish X: 5054  Y: 15898  Z: -1063
The end angle is 176.423660
The difference between angles is 157.011276
Rep # 8
Start X: 14536  Y: -6793  Z: 4957
The start angle is 323.901672
Finish X: 7500  Y: 14975  Z: 824
The end angle is 182.700638
The difference between angles is 141.201035
Rep # 9
Start X: 14820  Y: -6035  Z: 3589
The start angle is 329.237244
Finish X: 8377  Y: 14781  Z: -403
The end angle is 178.898285
The difference between angles is 150.338959
Rep # 10
You have done 5 reps! Keep going! 0 reps left
You have finished Situps! Atta boy!
Exercises completed so far:  Situps
Iteration # 1
Start X: 6798  Y: 14780  Z: 4280
The start angle is 196.090836
Finish X: 6644  Y: 13383  Z: 8199
The end angle is 211.675491
The difference between angles is 15.584656
-------------------------------------------
Exercises completed so far:  Situps
Iteration # 2
Start X: 6384  Y: 13903  Z: 7731
The start angle is 208.855682
Finish X: 4033  Y: 14799  Z: 7347
The end angle is 206.565079
The difference between angles is 2.290604
-------------------------------------------
Exercises completed so far:  Situps
Iteration # 3
Start X: 4890  Y: 12411  Z: 8147
The start angle is 212.932251
Finish X: 9796  Y: 7496  Z: 11108
The end angle is 235.804443
The difference between angles is 22.872192
-------------------------------------------
Exercises completed so far:  Situps
Iteration # 4
Start X: 3628  Y: 16505  Z: 1039
The start angle is 183.423874
Finish X: 5205  Y: 16379  Z: 4037
The end angle is 193.570450
The difference between angles is 10.146576
-------------------------------------------
Detected Squats. Starting the workout...

Jumping Jacks Video Demo: https://youtu.be/EmRaBL-TCf0

Pushups Video Demo: https://youtu.be/gyXKUe-X6DY

Situps Video Demo: https://youtu.be/rZp9FuJeIPs

Squats Video Demo: https://youtu.be/VQekJc2J7xo

STM32F407G-DISC1: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
