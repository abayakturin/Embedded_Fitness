#include <mbed.h>
#include <stdio.h>
#include "LIS3DSH.h"
#include "USBSerial.h"

// Initializing PINs
LIS3DSH acc(PA_7, PA_6, PA_5, PE_3);
USBSerial serial;
DigitalIn userButton(USER_BUTTON);
DigitalOut led3(LED3); // orange
DigitalOut led4(LED4); // green
DigitalOut led5(LED5); // red
DigitalOut led6(LED6); // blue

// Variables declaration 
int16_t X, Y, Z;        // signed integer variables for X,Y,Z values
float roll, pitch;      // float variables for angle
bool buttonDown = false; // User button default

// Functions
void lightsOn(bool orange=false, bool red=false, bool blue=false, bool green=false); // Trigger certain LEDs
void indicateProgress(); // Flash the LEDs to indicate the workout progress
void doWorkout(); // Contains the fixed programmed routine
int detectExercise(); // Detect what exercise has occured based on the accelerometer data
void doSitups(int sets, int reps); // Situps
void doPushups(int sets, int reps); // Pushups
void doJumpingJacks(int sets, int reps); // Jumping Jacks
void doSquats(int sets, int reps); // Squats

int main()
{
  doWorkout();
  return 0;
}

// Switch LEDs
void lightsOn(bool orange, bool red, bool blue, bool green){

  if(orange == true) led3 = !led3;
  if(green == true) led4 = !led4;    
  if(red == true) led5 = !led5;
  if(blue == true) led6 = !led6;   
}

// End of n-reps
void indicateProgress(){

  for(int i = 0; i < 5; i++){

    wait(1);
    lightsOn(true, true, true, true);
    wait(1);
    lightsOn(true, true, true, true);
  }
}

// The workout routine
void doWorkout(){

    // move = 0 - situps, move = 1 - pushups, move = 2 - jumping jacks, move = 3 - squats
  int move = 1;

  while (true) {  // run forever
    if (userButton) {  // button is pressed
      if  (!buttonDown) {  // a new button press
        move++;
        buttonDown = true;     // record that the button is now down so we don't count one press lots of times
        wait_ms(10);              // ignore anything for 10ms, a very basic way to de-bounce the button. 
      } 
    } else { // button isn't pressed
      buttonDown = false;
    }
  }

  if(move == 1) doSitups(1, 15);
  if(move == 2) doPushups(1, 15);
  if(move == 3) doJumpingJacks(1, 15);
  if(move == 4) doSquats(1, 15);

  move++;
}

// Situps - orange light. Expected difference range between the start angle and the end angle is 150-250
void doSitups(int sets, int reps){ 

  lightsOn(true, false, false, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;
  float startAngle, endAngle, angleDiff;

  // while(reps_count < sets * reps){
  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startAngle = pitch; 
        // serial.printf("LIS3DSH acceleromoter detected! , Reading now. Iteration # %d\n\r", iter / 2 + 1);
        serial.printf("The start angle is %f\n\r", startAngle);
      }
      else{
        endAngle = pitch;
        angleDiff = abs(startAngle - endAngle);
        // serial.printf("Roll(Y-Z angle): %f   Pitch(X-Z angle): %f\n\r", roll, pitch);
        serial.printf("The end angle is %f\n\r", endAngle);
        serial.printf("The difference between angles is %f\n\r", angleDiff);

        if(angleDiff >= 140 && angleDiff <= 260){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % 3 == 0){

            lightsOn(true, false, false, false);
            indicateProgress();
            lightsOn(true, false, false, false);
            serial.printf("You have done 5 reps! Keep going! %d reps left\n\r", total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Situps! Atta boy!\n\r");
            lightsOn(true, false, false, false);
            lightsOn(true, true, true, true);
            break;
          }
        }
      }
      // serial.printf("X: %d  Y: %d  Z: %d\n\r", X, Y, Z);
      wait(3);
    }
  }
}

// Pushpus - red light
void doPushups(int sets, int reps){
  lightsOn(false, true, false, false);
}

// Junping Jacks - blue light
void doJumpingJacks(int sets, int reps){
  lightsOn(false, false, true, false);
}

// Squats - green light
void doSquats(int sets, int reps){
  lightsOn(false, false, false, true);
}

