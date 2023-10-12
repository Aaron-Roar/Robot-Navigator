//#include <Wire.h>
//#include <VL53L0X.h>

//VL53L0X fwd_tof;
//VL53L0X bckwd_tof;
//VL53L0X left_tof;
//VL53L0X right_tof;

struct Tofs {
    int fwd, bckwd, left, right;
};

void tof_init() {
//    pinMode(9, OUTPUT);
//    pinMode(10, OUTPUT);
//    pinMode(11, OUTPUT);
//    pinMode(12, OUTPUT);
//    digitalWrite(9, LOW);
//    digitalWrite(10, LOW);
//    digitalWrite(11, LOW);
//    digitalWrite(12, LOW);
//
//    Wire.begin();
//    //SENSOR
//    pinMode(9, INPUT);
//    fwd_tof.init(true);
//    fwd_tof.setAddress((uint8_t)22);
//
//    pinMode(10, INPUT);
//    bckwd_tof.init(true);
//    bckwd_tof.setAddress((uint8_t)25);
//
//    pinMode(11, INPUT);
//    left_tof.init(true);
//    left_tof.setAddress((uint8_t)29);
//
//    pinMode(12, INPUT);
//    right_tof.init(true);
//    right_tof.setAddress((uint8_t)32);
//
//
//    fwd_tof.setTimeout(10);
//    bckwd_tof.setTimeout(10);
//    left_tof.setTimeout(10);
//    right_tof.setTimeout(10);
//
//
//    byte count = 0;
//    for (byte i = 1; i < 40; i++)
//    {
//        Wire.beginTransmission (i);
//    }
}

int readFwd() {
//    return fwd_tof.readRangeSingleMillimeters() + 0; //+ offset
} //read TOF fwd
int readBckwd() {
 //   return bckwd_tof.readRangeSingleMillimeters() + 0; //+ offset
} //read TOF bck
int readLeft() {
//    return left_tof.readRangeSingleMillimeters() + 0; //+ offset
} //
int readRight() {
//    return right_tof.readRangeSingleMillimeters() + 0; //+ offset
} //



