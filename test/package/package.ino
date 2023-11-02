#include <Servo.h>

Servo servo_left;  // create servo object to control a servo
Servo servo_right;
Servo servo_top;

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
    servo_left.attach(8);  // attaches the servo on pin 9 to the servo object
    servo_right.attach(9);  // attaches the servo on pin 9 to the servo object
    servo_top.attach(6);
}

void loop() {
    servo_top.write(90);
    servo_left.write(110);                  // sets the servo position according to the scaled value
    servo_right.write(70);                  // sets the servo position according to the scaled value
    delay(1000);

    servo_top.write(0);
    delay(1000);
    servo_left.write(90);                  // sets the servo position according to the scaled value
    servo_right.write(90);                  // sets the servo position according to the scaled value
    delay(1000);
}
