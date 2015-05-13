// include stepper library header file
#include <AccelStepper.h>

// motor speed in steps per second
#define motorSpeed 2400

// motor acceleration in steps per second per second
#define motorAccel 32000

// left eyeball step and direction pins
#define xstep 2
#define xdir  3

// right eyeball step and direction pins
#define ystep 4
#define ydir  5

// left eyeball accelstepper instance
AccelStepper stepper1 (1, xstep, xdir);

// right eyeball accelstepper instance
AccelStepper stepper2 (1, ystep, ydir);


void setup()
{
    // set step and direction pins to outputs
    pinMode (xstep, OUTPUT);
    pinMode (xdir,  OUTPUT);
    pinMode (ystep, OUTPUT);
    pinMode (ydir,  OUTPUT);

    // left eyeball stepper setup
    stepper1.setMaxSpeed(motorSpeed);
    stepper1.setSpeed(motorSpeed);
    stepper1.setAcceleration(motorAccel);

    // right eyeball stepper setup
    stepper2.setMaxSpeed(motorSpeed);
    stepper2.setSpeed(motorSpeed);
    stepper2.setAcceleration(motorAccel);
}


void loop()
{
/*
    // demo loop for animated gif
    LookDown ();
    delay (250);
    LookLeft ();
    delay (250);
    LookDown ();
    delay (250);
    LookRight ();
    delay (250);
    LookDown ();
    delay (250);
*/    

    // main demo loop
    LookLeft ();
    delay (1000);
    LookRight ();
    delay (1000);
    LookDown ();
    delay (1000);
    RollEyes ();
    delay (1000);
    LookLeft ();
    delay (1000);
    LookDown ();
    delay (1000);
    CrossEyed ();
    delay (1000);
    LookUp ();
    delay (1000);
    LookDown ();
    delay (1000);
}


void LookLeft (void)
{
    stepper1.moveTo(+800);
    stepper2.moveTo(+800);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
}


void LookRight (void)
{
    stepper1.moveTo(-800);
    stepper2.moveTo(-800);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
}


void LookDown (void)
{
    stepper1.moveTo(0);
    stepper2.moveTo(0);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
}


void LookUp (void)
{
    stepper1.moveTo(+1600);
    stepper2.moveTo(-1600);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
}


void RollEyes (void)
{
    stepper1.moveTo(+3200);
    stepper2.moveTo(+3200);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
    
    stepper1.setCurrentPosition (0);
    stepper1.moveTo(0);
    stepper2.setCurrentPosition (0);
    stepper2.moveTo(0);
}


void CrossEyed (void)
{
    stepper1.moveTo(-800);
    stepper2.moveTo(+800);
    
    while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
        stepper1.run();
        stepper2.run();
    }
}

