#include <Wire.h>
#include <VL53L0X.h>


#define fwd_pin A0
#define bckwd_pin A1
#define left_pin A2
#define right_pin A3

//#define fwd_address 8
//#define bckwd_address 12
//#define left_address 14
//#define right_address 16
#define fwd_address 8
#define bckwd_address 12
#define left_address 14
#define right_address 16

VL53L0X fwd_tof;
VL53L0X bckwd_tof;
VL53L0X left_tof;
VL53L0X right_tof;

float l_offset = -45; //milimeters
float r_offset = -29;
float f_offset = -36;
float b_offset = -33;

void tofInit() {

    pinMode(fwd_pin, OUTPUT);
    pinMode(bckwd_pin, OUTPUT);
    pinMode(left_pin, OUTPUT);
    pinMode(right_pin, OUTPUT);
    digitalWrite(fwd_pin, LOW);
   digitalWrite(bckwd_pin, LOW);
    digitalWrite(left_pin, LOW);
    digitalWrite(right_pin, LOW);

    Wire.begin();
    Wire.setClock(9000000);
    //SENSOR
    pinMode(fwd_pin, INPUT);
    fwd_tof.init(true);
    fwd_tof.setAddress((uint8_t)fwd_address);

    pinMode(bckwd_pin, INPUT);
    bckwd_tof.init(true);
    bckwd_tof.setAddress((uint8_t)bckwd_address);

    pinMode(left_pin, INPUT);
  left_tof.init(true);
  left_tof.setAddress((uint8_t)left_address);
//
  pinMode(right_pin, INPUT);
    right_tof.init(true);
    right_tof.setAddress((uint8_t)right_address);



    fwd_tof.setTimeout(800);
    bckwd_tof.setTimeout(800);
    left_tof.setTimeout(800);
    right_tof.setTimeout(800);


        left_tof.setSignalRateLimit(0.1);
        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 20);
        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 18);
        left_tof.setMeasurementTimingBudget(30000);

        right_tof.setSignalRateLimit(0.1);
        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 20);
        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 18);
        right_tof.setMeasurementTimingBudget(30000);

        fwd_tof.setSignalRateLimit(0.1);
        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 20);
        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 18);
        fwd_tof.setMeasurementTimingBudget(30000);

        bckwd_tof.setSignalRateLimit(0.1);
        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 20);
        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 18);
        bckwd_tof.setMeasurementTimingBudget(30000);

        Wire.beginTransmission (fwd_address);
        Wire.beginTransmission (bckwd_address);
        Wire.beginTransmission (left_address);
        Wire.beginTransmission (right_address);
        delay(100);
}

int readFwd() {
    return fwd_tof.readRangeSingleMillimeters() + f_offset; //+ offset
} //read TOF fwd
int readBckwd() {
    return bckwd_tof.readRangeSingleMillimeters() + b_offset; //+ offset
} //read TOF bck
int readLeft() {
    return left_tof.readRangeSingleMillimeters() + l_offset; //+ offset
} //
int readRight() {
    return right_tof.readRangeSingleMillimeters() + r_offset; //+ offset
} //

int mode(int a[],int n) {
   int maxValue = 0, maxCount = 0, i, j;

   for (i = 0; i < n; ++i) {
      int count = 0;
      
      for (j = 0; j < n; ++j) {
         if (a[j] == a[i])
         ++count;
      }
      
      if (count > maxCount) {
         maxCount = count;
         maxValue = a[i];
      }
   }

   return maxValue;
}

Position currentPosition(int itterations) {
    int cycles = itterations;
    Position pos;

    //if(itterations <= 1) {
        pos.fwd = readFwd();
        pos.bckwd = readBckwd();
        pos.left = readLeft();
        pos.right = readRight();
        return pos;
    //}

    //int fwds[cycles];
    //int bckwds[cycles];
    //int lefts[cycles];
    //int rights[cycles];


    //while(cycles > 0) {
    //    fwds[cycles] = readFwd();
    //    bckwds[cycles] = readBckwd();
    //    lefts[cycles] = readLeft();
    //    rights[cycles] = readRight();

    //    cycles -= 1;
    //}
    //pos.fwd = mode(fwds, sizeof(fwds)/sizeof(fwds[0]));
    //pos.bckwd = mode(bckwds, sizeof(bckwds)/sizeof(bckwds[0]));
    //pos.left = mode(lefts, sizeof(lefts)/sizeof(lefts[0]));
    //pos.right = mode(rights, sizeof(rights)/sizeof(rights[0]));

    //return pos;
}

