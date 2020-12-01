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
int16_t startY, startZ, endY, endZ, diffY, diffZ; // Tracking the difference in the coordinates
float startAngle, endAngle, angleDiff; // Tracking the difference in angle
bool isSitups = angleDiff >= 140 && angleDiff <= 260; // Check if the angle change fit Situps
bool isPushups = diffZ >= 0 && diffZ <= 900; // Check if the coordinate changes fit Pushups
bool isJumpingJacks = diffY >= 4000 && diffY <= 16000; // Check if the coordinate changes fit Jumping Jacks
bool isSquats = diffY >= 1000 && diffY <= 2000; // Check if the coordinate changes fit Squat

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
  detectExercise();
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

// Detect the excerise 
int detectExercise(){

  int iter = 0;

  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startZ = Z;
        startY = Y; 
        serial.printf("Start X: %d  Y: %d  Z: %d\n\r", X, Y, Z);
        
        startAngle = pitch; 
        serial.printf("The start angle is %f\n\r", startAngle);
      } else {
        endZ = Z;
        endY = Y;
        diffZ = abs(startZ - endZ);
        diffY = abs(startY - endY);
        serial.printf("Finish X: %d  Y: %d  Z: %d\n\r", X, Y, Z);

        endAngle = pitch;
        angleDiff = abs(startAngle - endAngle);
        serial.printf("The end angle is %f\n\r", endAngle);
        serial.printf("The difference between angles is %f\n\r", angleDiff);

        if(isPushups){
          doWorkout(1);
        } else if(isSquats) {
          doWorkout(2);
        } else if(isJumpingJacks){
          doWorkout(3);
        } else if(isSitups){
          doWorkout(4);
        }
      }
      wait(3);
    }
  }
}

// The workout routine
void doWorkout(int exercise){

  while (true) {  // run forever
    if (userButton) {  // button is pressed
      if  (!buttonDown) {  // a new button press
        if(exercise == 1) doSitups(1, 15);
        else if(exercise == 2) doPushups(1,15);
        else if(exercise == 3) doJumpingJacks(1, 15);
        else if(exercise == 4) doSquats(1, 10);
        buttonDown = true; // record that the button is now down so we don't count one press lots of times
        wait_ms(10); // ignore anything for 10ms, a very basic way to de-bounce the button. 
      } 
    } else { // button isn't pressed
      buttonDown = false;
    }
  }
}

// Situps - orange light. Expected difference range between the start angle and the end angle is 150-250
void doSitups(int sets, int reps){ 

  lightsOn(true, false, false, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;

  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startAngle = pitch; 

        serial.printf("The start angle is %f\n\r", startAngle);
      }
      else{
        endAngle = pitch;
        angleDiff = abs(startAngle - endAngle);

        serial.printf("The end angle is %f\n\r", endAngle);
        serial.printf("The difference between angles is %f\n\r", angleDiff);

        if(isSitups){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % 5 == 0){

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
      wait(3);
    }
  }
}

// Pushpus - red light
void doPushups(int sets, int reps){
  
  lightsOn(false, true, false, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;

  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startZ = Z; 
        serial.printf("Start X: %d  Y: %d  Z: %d\n\r", X, Y, Z);
      }
      else{
        endZ = Z;
        diffZ = abs(startZ - endZ);
        serial.printf("Finish X: %d  Y: %d  Z: %d\n\r", X, Y, Z);

        if(isPushups){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % 5 == 0){

            lightsOn(false, true, false, false);
            indicateProgress();
            lightsOn(false, true, false, false);
            serial.printf("You have done 5 reps! Keep going! %d reps left\n\r", total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Pushups! Atta boy!\n\r");
            lightsOn(false, true, false, false);
            lightsOn(true, true, true, true);
            break;
          }
        }
      }
      wait(3);
    }
  }
}

// Junping Jacks - blue light
void doJumpingJacks(int sets, int reps){

  lightsOn(false, false, true, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;

  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startY = Y; 
        serial.printf("Start X: %d  Y: %d  Z: %d\n\r", X, Y, Z);
      }
      else{
        endY = Y;
        diffY = abs(startY - endY);
        serial.printf("Finish X: %d  Y: %d  Z: %d\n\r", X, Y, Z);

        if(isJumpingJacks){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % 5 == 0){

            lightsOn(false, false, true, false);
            indicateProgress();
            lightsOn(false, false, true, false);
            serial.printf("You have done 5 reps! Keep going! %d reps left\n\r", total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Jumping Jacks! Atta boy!\n\r");
            lightsOn(false, false, true, false);
            lightsOn(true, true, true, true);
            break;
          }
        }
      }
      wait(1);
    }
  }
}

// Squats - green light
void doSquats(int sets, int reps){

  lightsOn(false, false, false, true);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;

  while(1){
    iter++;
    if(acc.Detect() != 1){
      serial.printf("LIS3DSH acceleromoeter not detected!\n\r");
    } else {
      acc.ReadData(&X, &Y, &Z);           // reads X, Y, Z values
      acc.ReadAngles(&roll, &pitch);      // reads roll and pitch angles
      
      if(iter % 2 != 0){
        startY = Y; 
        serial.printf("Start X: %d  Y: %d  Z: %d\n\r", X, Y, Z);
      }
      else{
        endY = Y;
        diffY = abs(startY - endY);
        serial.printf("Finish X: %d  Y: %d  Z: %d\n\r", X, Y, Z);

        if(isSquats){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % 5 == 0){

            lightsOn(false, false, false, true);
            indicateProgress();
            lightsOn(false, false, false, true);
            serial.printf("You have done 5 reps! Keep going! %d reps left\n\r", total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Squats! Atta boy!\n\r");
            lightsOn(false, false, false, true);
            lightsOn(true, true, true, true);
            break;
          }
        }
      }
      wait(3);
    }
  }
}

