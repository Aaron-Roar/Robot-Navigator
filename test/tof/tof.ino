
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X fwd_tof;
VL53L0X bckwd_tof;
VL53L0X left_tof;
VL53L0X right_tof;

float l_offset = 27; //milimeters
float r_offset = 27;
float f_offset = 20;
float b_offset = 20;

void tofInit() {

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    Wire.begin();
    //SENSOR
    pinMode(2, INPUT);
    fwd_tof.init(true);
    fwd_tof.setAddress((uint8_t)22);

    pinMode(3, INPUT);
    bckwd_tof.init(true);
    bckwd_tof.setAddress((uint8_t)25);

  pinMode(5, INPUT);
  left_tof.init(true);
  left_tof.setAddress((uint8_t)28);

    pinMode(4, INPUT);
    right_tof.init(true);
    right_tof.setAddress((uint8_t)31);


    fwd_tof.setTimeout(100);
    bckwd_tof.setTimeout(100);
    left_tof.setTimeout(100);
    right_tof.setTimeout(100);


//        left_tof.setMeasurementTimingBudget(200000);
//        left_tof.setSignalRateLimit(0.1);
//        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        right_tof.setMeasurementTimingBudget(200000);
//        right_tof.setSignalRateLimit(0.1);
//        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        fwd_tof.setMeasurementTimingBudget(200000);
//        fwd_tof.setSignalRateLimit(0.1);
//        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        bckwd_tof.setMeasurementTimingBudget(200000);
//        bckwd_tof.setSignalRateLimit(0.1);
//        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

        Wire.beginTransmission (22);
        Wire.beginTransmission (25);
      Wire.beginTransmission (28);
        Wire.beginTransmission (31);
}

int readFwd() {
    return fwd_tof.readRangeSingleMillimeters() + f_offset - 10; //+ offset
} //read TOF fwd
int readBckwd() {
    return bckwd_tof.readRangeSingleMillimeters() + b_offset - 10; //+ offset
} //read TOF bck
int readLeft() {
    return left_tof.readRangeSingleMillimeters() + l_offset - 10; //+ offset
} //
int readRight() {
    return right_tof.readRangeSingleMillimeters() + r_offset - 10; //+ offset
} //


void setup() {
    Serial.println("Setup");
    tofInit();
    Serial.println("Setup Done");
}

void loop() {
    Serial.print("Fwd: ");
    Serial.println(readFwd());
    Serial.print("Bckwd: ");
    Serial.println(readBckwd());
    Serial.print("Left: ");
    Serial.println(readLeft());
    Serial.print("Right: ");
    Serial.println(readRight());
    Serial.println("-----------------------------------");
    delay(2000);
}
