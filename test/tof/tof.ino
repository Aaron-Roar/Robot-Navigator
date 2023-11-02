#include <stdlib.h>
#include <stdio.h> 
#include <stddef.h>
#include <stdio.h> 
#include <stdint.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X fwd_tof;
VL53L0X bckwd_tof;
VL53L0X left_tof;
VL53L0X right_tof;

struct Tofs {
    int fwd, bckwd, left, right;
};

void tof_init() {
    pinMode(8, OUTPUT);
//    pinMode(3, OUTPUT);
//    pinMode(4, OUTPUT);
//    pinMode(5, OUTPUT);
    digitalWrite(8, LOW);
//    digitalWrite(3, LOW);
//    digitalWrite(4, LOW);
//    digitalWrite(5, LOW);

    Wire.begin();
    pinMode(8, INPUT);
    fwd_tof.init(true);
    fwd_tof.setAddress((uint8_t)22);

    fwd_tof.setTimeout(100);
    Wire.beginTransmission (22);
}

int readFwd() {
    return fwd_tof.readRangeSingleMillimeters() + 0; //+ offset
} //read TOF fwd



void setup() {
    Serial.begin(9600);
    tof_init();
}

void loop() {
Serial.println(readFwd());
delay(1000);
}
