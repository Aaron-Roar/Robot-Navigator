#define arm_pin1 15
#define arm_pin2 20

Servo arm_servo1;
Servo arm_servo2;

void servoInit() {
    arm_servo1.attach(arm_pin1);
    arm_servo2.attach(arm_pin2);
}

void lowerGripper() {
    arm_servo1.write(0);
    arm_servo2.write(0);
}

void raiseGripper() {
    arm_servo1.write(0);
    arm_servo2.write(0);
}

void openGripper() {
    arm_servo1.write(230);
    arm_servo2.write(230);
}

void closeGripper() {
    arm_servo1.write(0);
    arm_servo2.write(0);
}

