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
int isSitups[] = {140, 260}; // Check if the angle change fit Situps
int isPushups[] = {200, 600}; // Check if the coordinate changes fit Pushups
int isJumpingJacks[] = {30, 200}; // Check if the coordinate changes fit Jumping Jacks
int isSquats[] = {400, 500}; // Check if the coordinate changes fit Squat
string exercisesCompleted = ""; // The progress message shown after each completed workout
int numSets = 1, numReps = 10, numRepMessage = 5; // The number of sets, reps in each set, and the rep message period

// Functions
void lightsOn(bool orange=false, bool red=false, bool blue=false, bool green=false); // Trigger certain LEDs
void indicateProgress(int mode, int detectedExercise); // Flash the LEDs to indicate the workout progress
void doWorkout(int exercise); // Contains the fixed programmed routine
int detectExercise(); // Detect what exercise has occured based on the accelerometer data
void doSitups(int sets, int reps, int repMessage); // Situps
void doPushups(int sets, int reps, int repMessage); // Pushups
void doJumpingJacks(int sets, int reps, int repMessage); // Jumping Jacks
void doSquats(int sets, int reps, int repMessage); // Squats
// Completion status for the exercises
bool pushupsDone = false, situpsDone = false, squatsDone = false, jumpingJacksDone = false;  


int main()
{
  // detectExercise();
  // doSitups(1,100,100);
  // doPushups(1,100,100);
  // doJumpingJacks(1,100,100);
  doSquats(1,100,100);
  return 0;
}

// Switch LEDs
// Situps - orange, Pushups - red, Jumping Jacks - blue, Squats - green
void lightsOn(bool orange, bool red, bool blue, bool green){

  if(orange == true) led3 = !led3;
  if(green == true) led4 = !led4;    
  if(red == true) led5 = !led5;
  if(blue == true) led6 = !led6;   
}

// End of n-reps
void indicateProgress(int mode, int detectedExercise = 0){

  if(mode == 1){

    for(int i = 0; i < 5; i++){

      wait(1);
      lightsOn(true, true, true, true);
      wait(1);
      lightsOn(true, true, true, true);
    }

  } else if(mode == 2){

    if(detectedExercise == 1) lightsOn(true, false, false, false);
    else if(detectedExercise == 2) lightsOn(false, true, false, false);
    else if(detectedExercise == 3) lightsOn(false, false, true, false);
    else if(detectedExercise == 4) lightsOn(true, false, false, true);
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
        serial.printf("Exercises completed so far: %s\n\r", exercisesCompleted.c_str());
        serial.printf("Iteration # %d\n\r", iter / 2 + 1);
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
        serial.printf("-------------------------------------------\n\r");

        if(situpsDone && pushupsDone && jumpingJacksDone && squatsDone){

          serial.printf("You have completed:\n\r%d reps of Situps\n\r%d reps of Pushups\n\r%d reps of Jumping Jacks\n\r%d reps of Squats\n\rWell done!\n\r");
        }

        if(isSitups[0] <= angleDiff && angleDiff <= isSitups[1] && !situpsDone){
          serial.printf("Detected Situps. Starting the workout...\n\r");
          situpsDone = true;
          indicateProgress(2, 1); // Indicates what exercise has been identified
          doWorkout(1);
        }
        if(isPushups[0] <= diffZ && diffZ <= isPushups[1] && !pushupsDone && angleDiff < 50){
          serial.printf("Detected Pushups. Starting the workout...\n\r");
          pushupsDone = true;
          indicateProgress(2, 1);
          doWorkout(2);
        }
        if(isJumpingJacks[0] <= diffY && diffY <= isJumpingJacks[1] && !jumpingJacksDone && angleDiff < 50){
          serial.printf("Detected Jumping Jacks. Starting the workout...\n\r");
          jumpingJacksDone = true;
          indicateProgress(2, 1);
          doWorkout(3);
        }
        if(isSquats[0] <= diffY && diffY <= isSquats[1] && !squatsDone && angleDiff < 50) {
          serial.printf("Detected Squats. Starting the workout...\n\r");
          squatsDone = true;
          indicateProgress(2, 1);
          doWorkout(4);
        }
      }
      wait(2);
    }
  }
}

// The workout routine
void doWorkout(int exercise){

  while (true) {  // run forever
    if (userButton) {  // button is pressed
      if  (!buttonDown) {  // a new button press
        if(exercise == 1) doSitups(numSets,numReps,numRepMessage);
        else if(exercise == 2) doPushups(numSets,numReps,numRepMessage);
        else if(exercise == 3) doJumpingJacks(numSets,numReps,numRepMessage);
        else if(exercise == 4) doSquats(numSets,numReps,numRepMessage);
        buttonDown = true; // record that the button is now down so we don't count one press lots of times
        wait_ms(10); // ignore anything for 10ms, a very basic way to de-bounce the button. 
      } 
    } else { // button isn't pressed
      buttonDown = false;
    }
  }
}

// Situps - orange light. Expected difference range between the start angle and the end angle is 150-250
void doSitups(int sets, int reps, int repMessage){ 

  lightsOn(true, false, false, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;
  serial.printf("Starting Situps...\n\r");

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

        if(isSitups[0] <= angleDiff && angleDiff <= isSitups[1]){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % repMessage == 0){

            lightsOn(true, false, false, false);
            indicateProgress(1);
            lightsOn(true, false, false, false);
            serial.printf("You have done %d reps! Keep going! %d reps left\n\r", repMessage, total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Situps! Atta boy!\n\r");
            lightsOn(true, false, false, false);
            lightsOn(true, true, true, true);
            exercisesCompleted += " Situps";
            detectExercise();
          }
        }
      }
      wait(3);
    }
  }
}

// Pushpus - red light
void doPushups(int sets, int reps, int repMessage){
  
  lightsOn(false, true, false, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;
  serial.printf("Starting Pushups...\n\r");

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

        if(isPushups[0] <= diffZ && diffZ <= isPushups[1]){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % repMessage == 0){

            lightsOn(false, true, false, false);
            indicateProgress(1);
            lightsOn(false, true, false, false);
            serial.printf("You have done %d reps! Keep going! %d reps left\n\r", repMessage, total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Pushups! Atta boy!\n\r");
            lightsOn(false, true, false, false);
            lightsOn(true, true, true, true);
            exercisesCompleted += " Pushups ";
            detectExercise();
          }
        }
      }
      wait(3);
    }
  }
}

// Junping Jacks - blue light
void doJumpingJacks(int sets, int reps, int repMessage){

  lightsOn(false, false, true, false);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;
  serial.printf("Starting Jumping Jacks...\n\r");

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

        if(isJumpingJacks[0] <= diffY && diffY <= isJumpingJacks[1]){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % repMessage == 0){

            lightsOn(false, false, true, false);
            indicateProgress(1);
            lightsOn(false, false, true, false);
            serial.printf("You have done %d reps! Keep going! %d reps left\n\r", repMessage, total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Jumping Jacks! Atta boy!\n\r");
            lightsOn(false, false, true, false);
            lightsOn(true, true, true, true);
            exercisesCompleted += " Jumping Jacks ";
            detectExercise();
          }
        }
      }
      wait(3);
    }
  }
}

// Squats - green light
void doSquats(int sets, int reps, int repMessage){

  lightsOn(false, false, false, true);
  int iter = 0, counted_reps = 0, total_reps = sets * reps;
  serial.printf("Starting Squats...\n\r");

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

        if(isSquats[0] <= diffY && diffY <= isSquats[1]){

          counted_reps++;
          serial.printf("Rep # %d\n\r", counted_reps);

          if(counted_reps % repMessage == 0){

            lightsOn(false, false, false, true);
            indicateProgress(1);
            lightsOn(false, false, false, true);
            serial.printf("You have done %d reps! Keep going! %d reps left\n\r", repMessage, total_reps - counted_reps);
          }

          if(counted_reps == total_reps){
            serial.printf("You have finished Squats! Atta boy!\n\r");
            lightsOn(false, false, false, true);
            lightsOn(true, true, true, true);
            exercisesCompleted += " Squats ";
            detectExercise();
          }
        }
      }
      wait(3);
    }
  }
}

