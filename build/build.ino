
#include <stdlib.h>
#include <stdio.h> 
#include <stddef.h>
#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <Wire.h>
#include <VL53L0X.h>
char wall_speed_zone = 100;
char drive_fb_1cm_magnitude = 10;
char drive_lr_1cm_magnitude = 10;

char stop_all_magnitude = 60;
char stop_fb_magnitude = 60;
char stop_lr_magnitude = 30;
char stop_turn_magnitude = 20;

char crawl_magnitude = 50;

char stop_crawl_all_magnitude = 30;
char stop_crawl_fb_magnitude = 20;
char stop_crawl_lr_magnitude = 30;
char stop_crawl_turn_magnitude = 20;

float rotation_gain = 2.1;
float angle_tolerance = 26;
float angle_check_percentage = 0.6;
float cw_gain = 1;
float ccw_gain = 1;
//#include <stdint.h>

typedef struct Position {
    int fwd, bckwd, left, right;
} Position;

typedef union MovementType {
    struct Position linear_movement;
    int angle;
} MovementType;

typedef struct Motion {
    char type;
    int func;
    union MovementType movement;

    struct Motion* prev;
    struct Motion* next[2];
} Motion;

//Motor Type
typedef struct Direction {
    char* motors;
    char type;
    struct Direction* opo_direction;
} Direction;
//#include "movement.h"
//#include <stddef.h>
//#include <stdio.h>

#define branch_limit 2
uint16_t motion_count = 0;
size_t forks[branch_limit];
Motion motions[20] = {0};

Motion* addMotion(union MovementType new_movement, uint8_t type, int func, Motion* prev_movement) {


    if(prev_movement != 0) {
        int i = 0;
        while(i < branch_limit) {
            if(prev_movement->next[i] == 0) {
                prev_movement->next[i] = &motions[motion_count];
                break;
            }
            i += 1;
        }
    }

//    motions[motion_count] = (Motion){
//        .type = type,
//        .func = func,
//        .movement = new_movement,
//        .prev = prev_movement,
//        .next = {0}
//    };
    Motion* mot = &motions[motion_count];

    mot->type = type;
    mot->func = func;
    mot->movement = new_movement;
    mot->prev = prev_movement;
    mot->next[0] = {0};
    mot->next[1] = {0};


    motion_count += 1;


    return &motions[motion_count - 1];
}

Motion* addAngularMovement(int angle, int func, Motion* prev_motion) {
    return addMotion(
            (union MovementType) {
                .angle = angle
            },
            2,
            func,
            prev_motion
    );
}

Motion* addLinearMovement(Position movement, int func, Motion* prev_motion) {
    return addMotion(
            (union MovementType) {
               .linear_movement = movement
            },
            1,
            func,
            prev_motion
    );
}

int temp_sense_value = 50;
char inTransit = 0;

//DF 9:bckwd 10:fwd
//DR 7:bckwd 8:fwd
//
//PF 5:bckwd 6:fwd
//PR 3:bckwd 4:fwd

char fwd_motors[]   ={4, 6, 8, 10};
char bckwd_motors[] ={3, 5, 7, 9};
char left_motors[]  ={3, 6, 8, 9};
char right_motors[] ={4, 5, 7, 10};
char cw_motors[]    ={3, 5, 8, 10};
char ccw_motors[]   ={4, 6, 7, 9};
char all_motors[]          ={3, 4, 5, 6, 7, 8, 9, 10};

Direction fwd;
Direction bckwd;
Direction right;
Direction left;
Direction cw;
Direction ccw;
Direction all;



void writePortC(char* pins, char pins_size, char state) {
//    int i = 0;
//    long result = 0;
//    while(i < pins_size) {
//        result |= (1 << pins[i]);
//        i += 1;
//    }
//
//    if(state) {
//        PIOC -> PIO_SODR = result;   // set pin DIRECT PORT ACCESS
//    } else{
//        PIOC -> PIO_CODR = result;  // clear pin DIRECT PORT ACCESS
//    }
}


void drive(Direction direction) {
    //writePortC(direction, sizeof(char)*4, 1);
    inTransit = 1;
    int i = 0;
    while(i < 4) {
        digitalWrite(direction.motors[i], HIGH);
        i +=1;
    }
}

void halt(Direction direction) {
    //writePortC(direction, sizeof(char)*4, 0);
    int i = 0;
    while(i < 4) {
        digitalWrite(direction.motors[i], LOW);
        i +=1;
    }
}

void stop(Direction direction) {
    if(inTransit == 1) {
        char duration = 0;
        if(direction.type == 0)
            duration = stop_all_magnitude;
        else if(direction.type == 1)
            duration = stop_fb_magnitude;
        else if(direction.type = 2)
            duration = stop_lr_magnitude;
        else if(direction.type = 3)
            duration = stop_turn_magnitude;

        halt(direction);
        drive(*direction.opo_direction);
        delay(duration);
        halt(*direction.opo_direction);
    }
    inTransit = 0;
}

void driveDistance(Direction direction, float distance) {
    float duration = 0;

    if(direction.type == 1)
        duration = drive_fb_1cm_magnitude;
    else if(direction.type = 2)
        duration = drive_lr_1cm_magnitude;

    drive(direction);
    delay((distance/10)*duration);
    stop(direction);
}



void stopCrawl(Direction direction) {
    if(inTransit == 1) {
        char duration = 0;
        if(direction.type == 1)
            duration = stop_crawl_fb_magnitude;
        else if(direction.type = 2)
            duration = stop_crawl_lr_magnitude;
        else if(direction.type = 3)
            duration = stop_crawl_turn_magnitude;

        halt(direction);
        drive(*direction.opo_direction);
        delay(duration);
        halt(*direction.opo_direction);
    }
    inTransit = 0;
}

void crawl(Direction direction) {
    drive(direction);
    delay(crawl_magnitude);
    stopCrawl(direction);
}

void rotate(float degrees) {
    if(degrees > 0) {
        drive(cw);
        delay(degrees*rotation_gain);
        stop(cw);
    }
    else {
        drive(ccw);
        delay(-1*degrees*rotation_gain);
        stop(ccw);
    }
}


void motorInit() {
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    all.type   = 0;
    fwd.type   = 1;
    bckwd.type = 1;
    right.type = 2;
    left.type  = 2;
    cw.type    = 3;
    ccw.type   = 3;

    all.motors   = all_motors;
    fwd.motors   = fwd_motors;
    bckwd.motors = bckwd_motors;
    right.motors = right_motors;
    left.motors  = left_motors;
    cw.motors    = cw_motors;
    ccw.motors   = ccw_motors;
    
    all.opo_direction = &all;
    fwd.opo_direction = &bckwd;
    bckwd.opo_direction = &fwd;
    right.opo_direction = &left;
    left.opo_direction = &right;
    cw.opo_direction = &ccw;
    ccw.opo_direction = &cw;
}


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



    fwd_tof.setTimeout(100);
    bckwd_tof.setTimeout(100);
    left_tof.setTimeout(100);
    right_tof.setTimeout(100);


//        left_tof.setMeasurementTimingBudget(2000);
//        left_tof.setSignalRateLimit(0.1);
//        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        left_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        right_tof.setMeasurementTimingBudget(2000);
//        right_tof.setSignalRateLimit(0.1);
//        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        right_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        fwd_tof.setMeasurementTimingBudget(2000);
//        fwd_tof.setSignalRateLimit(0.1);
//        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        fwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//
//        bckwd_tof.setMeasurementTimingBudget(2000);
//        bckwd_tof.setSignalRateLimit(0.1);
//        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//        bckwd_tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

        Wire.beginTransmission (fwd_address);
        Wire.beginTransmission (bckwd_address);
        Wire.beginTransmission (left_address);
        Wire.beginTransmission (right_address);
        delay(10);
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

    if(itterations <= 1) {
        pos.fwd = readFwd();
        pos.bckwd = readBckwd();
        pos.left = readLeft();
        pos.right = readRight();
        return pos;
    }

    int fwds[cycles];
    int bckwds[cycles];
    int lefts[cycles];
    int rights[cycles];


    while(cycles > 0) {
        fwds[cycles] = readFwd();
        bckwds[cycles] = readBckwd();
        lefts[cycles] = readLeft();
        rights[cycles] = readRight();

        cycles -= 1;
    }
    pos.fwd = mode(fwds, sizeof(fwds)/sizeof(fwds[0]));
    pos.bckwd = mode(bckwds, sizeof(bckwds)/sizeof(bckwds[0]));
    pos.left = mode(lefts, sizeof(lefts)/sizeof(lefts[0]));
    pos.right = mode(rights, sizeof(rights)/sizeof(rights[0]));

    return pos;
}

float tolerance = 10;
float safe_z = 20;

float angleD(float ideal, float actual) {
    float rad_angle;
    if(actual < ideal) {
        rad_angle = asinf(actual/ideal); 
//        return 0; //Error when rotation past walls
    }
    else{
        rad_angle = asinf(ideal/actual); 
    }
    float angle = -1*((180/3.1415926)*rad_angle - 90);


    return angle;
}


void correctAngleLR(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.left + node->movement.linear_movement.right;
    pos = currentPosition(9);
    float actual = pos.left + pos.right;
    float angle_0 = angleD(ideal, actual);
    Serial.print("Angle: ");
    Serial.println(angle_0);

    if(angle_0 < angle_tolerance && angle_0 > -1*angle_tolerance) {
    }
    else {
        //Check cw
        rotate(angle_0*angle_check_percentage);
        delay(50);

        pos = currentPosition(9);
        actual = pos.left + pos.right;
        float angle_1 = angleD(ideal, actual);
        Serial.print("Attempted fix Angle: ");
        Serial.println(angle_1);

        if(angle_1 < angle_0) {
            rotate(angle_1*cw_gain); //roatet cw
            Serial.println("Rotated CW");
        }
        else {
            rotate(-1*angle_1*ccw_gain); //rotate ccw
            Serial.println("Rotated CCW");
        }
    }


}

float correctAngleFB(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    pos = currentPosition(9);
    float actual = pos.bckwd + pos.fwd;
    float angle_0 = angleD(ideal, actual);
    Serial.print("Angle: ");
    Serial.println(angle_0);

    if(angle_0 < angle_tolerance && angle_0 > -1*angle_tolerance) {
    }
    else {
        //Check cw
        rotate(angle_0*angle_check_percentage);
        delay(50);

        pos = currentPosition(9);
        actual = pos.bckwd + pos.fwd;
        float angle_1 = angleD(ideal, actual);
        Serial.print("Attempted fix Angle: ");
        Serial.println(angle_1);

        if(angle_1 < angle_0) {
            rotate(angle_1*cw_gain); //roatet cw
            Serial.println("Rotated CW");
        }
        else {
            rotate(-1*angle_1*ccw_gain); //rotate ccw
            Serial.println("Rotated CCW");
        }
    }


}



char test0(Motion* node, float tolerance) {
    return 1;
}

//ideally read in parralele to stop driving
char test1(Motion* node, float tolerance) {
    driveToFB(node);
    return 1;
}

char test2(Motion* node, float tolerance) {
    while(currentPosition(10).fwd > node->movement.linear_movement.fwd + tolerance*5) {
        drive(fwd);
    }
    stop(fwd);
    while(currentPosition(10).fwd > node->movement.linear_movement.fwd) {
        crawl(fwd);
    }
}
char test3(Motion* node, float tolerance) {
    int i = 0;
    while(i < 4) {
        correctAngleFB(node);
        i += 1;
    }

    return 1;
}


char driveToLR(Motion* node) {
    Direction* fb_dir = &all;
    Position state = currentPosition(1);

    if(node->movement.linear_movement.left < state.left) {
        fb_dir = &left;
    }
    else {
        fb_dir = &right;
    }

    correctAngleLR(node);
    float ideal = node->movement.linear_movement.left + node->movement.linear_movement.right;
    while(1) {
        state = currentPosition(1);
        float actual = state.left + state.right;
        Serial.print("Angle off: ");
        Serial.println(angleD(ideal, actual));

        if(state.fwd < 20) {
            Serial.println("Going fwd");
            stop(*fb_dir);
            driveDistance(fwd, 10);
        }
        else if(state.bckwd < 20) {
            Serial.println("Going bckwd");
            stop(*fb_dir);
            driveDistance(bckwd, 10);
        }
        else if(angleD(ideal, actual) > angle_tolerance || angleD(ideal, actual) < -1*angle_tolerance ) {
            Serial.println("Fixin angle");
            stop(*fb_dir);
            correctAngleLR(node);
        }
        else if(fb_dir == &left && state.left <= node->movement.linear_movement.left) {
            stop(*fb_dir);
            correctAngleLR(node);
            break;
        }
        else if(fb_dir == &right && state.right <= node->movement.linear_movement.right) {
            stop(*fb_dir);
            correctAngleLR(node);
            break;
        }
        else if(fb_dir == &left && state.left < 200) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else if(fb_dir == &right && state.right < 200) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else {
            Serial.println("driving fwd");
            drive(*fb_dir);
        }
    }
    stop(*fb_dir);
}

char driveToFB(Motion* node) {
    Direction* fb_dir = &all;
    Position state = currentPosition(1);

    if(node->movement.linear_movement.fwd < state.fwd) {
        fb_dir = &fwd;
    }
    else {
        fb_dir = &bckwd;
    }

    correctAngleFB(node);
    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    while(1) {
        state = currentPosition(1);
        float actual = state.fwd + state.bckwd;
        Serial.print("Angle off: ");
        Serial.println(angleD(ideal, actual));

        if(state.left < 20) {
            Serial.println("Going left");
            stop(*fb_dir);
            driveDistance(left, 10);
        }
        else if(state.right < 20) {
            Serial.println("Going right");
            stop(*fb_dir);
            driveDistance(right, 10);
        }
        else if(angleD(ideal, actual) > angle_tolerance || angleD(ideal, actual) < -1*angle_tolerance ) {
            Serial.println("Fixin angle");
            stop(*fb_dir);
            correctAngleFB(node);
        }
        else if(fb_dir == &fwd && state.fwd <= node->movement.linear_movement.fwd) {
            stop(*fb_dir);
            correctAngleFB(node);
            break;
        }
        else if(fb_dir == &bckwd && state.bckwd <= node->movement.linear_movement.bckwd) {
            stop(*fb_dir);
            correctAngleFB(node);
            break;
        }
        else if(fb_dir == &fwd && state.fwd < 200) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else if(fb_dir == &bckwd && state.bckwd < 200) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else {
            Serial.println("driving fwd");
            drive(*fb_dir);
        }
    }
    stop(*fb_dir);

}

char test4(Motion* node, float tolerance) {//fail
    driveToFB(node);
    return 1;
}


char (*functions[]) (Motion*, float) = {test0, test1, test2, test3, test4}; //BUG TEST 0 is used as safe space value and should not be executed but it is one program reversal


const char config1[]=
"@P1 \n"
"T1:45 536 21 21 \n"
"T1:536 45 21 21 \n"
"#P1 \n"
" \n"
"@P2 \n"
"T3:535 43 361 22 \n"
"T3:355 215 22 22 \n"
"T3:22 552 628 22 \n"
"#P2 \n"
" \n"
"@P3 \n"
"T1:10 55 1000 1200 \n"
"#P3 \n"
" \n"
" \n"
"\0";
//#include "../../types/movement.c"
//#include "../../maze-files/formatted-data/config1.h"
//#include <stdio.h>


//FILE *fptr;

//void fileToBuffer(FILE* file, char* buf) {
//    char letter;
//    int i = 0;
//    while((letter = fgetc(file)) != EOF) {
//        buf[i] = letter;
//        i += 1;
//    }
//}


int lineType(const char* paragraph) {
    char types[] = {'#', '\n', '@', '*', 'T', 'R'};
    int i = 0;
    while(i < sizeof(types)) {
        if(*paragraph == types[i])
            return i;
        i += 1;
    }
    return -1;
}



const char charsTillCharacter(const char* paragraph, char character) {
    char count = 0;
    while(paragraph[count] != character) {
        if(paragraph[count] == '\0' || paragraph == NULL)
            return 0;
        count += 1;
    }
    return count;
}

int powerOfTens(int value, char tens_factor) {
    int result = value;

    char i = tens_factor;
    while(i > 0) {
        result *= 10;
        i -= 1;
    }

    return result;

}

void stringCopyTill(const char* src, char* dest, char size) {
    int i = 0;
    while(i < size) {
        dest[i] = (char)src[i];
        i += 1;
    }
}

int stringToNumber(const char* paragraph, char delimiter) {
    char size = charsTillCharacter(paragraph, delimiter) - 1;
    //printf("Chars in %s, till %c, is %d\n", paragraph, delimiter, size);
    int number = 0;

    int i = size;
    while(i >= 0) {
        if(paragraph[i] == '-')
            return number * (-1);
        number += powerOfTens((int)paragraph[i] - 48, size - i);
        i -= 1;
    }
    return number;
}

void stringToNumbers(const char* paragraph, const char delimiter, int* numbers, char numbers_size) {
    int words = 0;
    char size = 0;
    while(words < numbers_size) {
        numbers[words] = stringToNumber(paragraph + size, delimiter);

        words += 1;
        size += (1 + charsTillCharacter(paragraph + size, delimiter));
    }
}

Position buildPosition(const char* paragraph, const char delimiter) {
    int translation[4] = {0};
    char offset = 1 + charsTillCharacter(paragraph, ':');
    stringToNumbers(paragraph + offset, delimiter, translation, 
            sizeof(translation)/sizeof(translation[0]));

    return (Position){
        .fwd = translation[0],
        .bckwd = translation[1],
        .left = translation[2],
        .right = translation[3]
    };
}

int buildAngle(const char* paragraph, const char delimiter) {
    char offset = 1 + charsTillCharacter(paragraph, ':');
    return stringToNumber(paragraph + offset, delimiter);
}

int buildFunction(const char* paragraph, const char delimiter) {
    int func = stringToNumber(paragraph + 1, delimiter);
    return func;
}

const char* locateSubString(const char* paragraph, const char* sub_string, const char sub_size) {
    int i = 0;
    while(paragraph[i] != '\0') {
        if(paragraph[i] == sub_string[0]) {
            //possible substring found
            int j = 0;
            while(j < sub_size) {
                if(paragraph[i + j] != sub_string[j])
                    break; //maybe breaking both loops
                j += 1;
            }
            if(j == sub_size)
                return paragraph + i;
        }
            i += 1;
    }
    printf("[!]COULD NOT FIND MATCH\n");
    return paragraph;
}

void reccursiveParser(const char* paragraph, Motion* prev_motion) {
    //Check NULL
    int line_type = lineType(paragraph);


    if(line_type == 0) {//\#
        //do nothing is end of sequence
        //printf("FinalLineType: %d\n", line_type);
    } else if(line_type == 1) {//\n
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //skip a line just filler space
    } else if(line_type == 2) {//@
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //do nothing begining of sequence
    } else if(line_type == 3) {//*
        char size_of_term = charsTillCharacter(paragraph, '\n');
        char branch_name[size_of_term - 1];
        stringCopyTill(paragraph, branch_name, size_of_term - 1);
        branch_name[0] = '@';
        const char* branch_loc = locateSubString(paragraph, branch_name, sizeof(branch_name) - 0);
        //printf("Branch: %s\n", branch_loc);
        reccursiveParser(branch_loc, prev_motion);
        //recurse down the branch adding nodes

        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //Call term after branch with current prev_motion
    } else if(line_type == 4) {//T
        int func = buildFunction(paragraph, ':');
        Position pos = buildPosition(paragraph, ' ');
        reccursiveParser(
                paragraph + charsTillCharacter(paragraph, '\n') + 1,
                addLinearMovement(pos, func, prev_motion));
    } else if(line_type == 5) {//R
        int angle = buildAngle(paragraph, ' ');
        int func = buildFunction(paragraph, ':');
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1,
                addAngularMovement(angle, func, prev_motion));

    } else{
    printf("[!]Couldnt Locate type for: %c\n", *paragraph);
    }

}

//int main() {
//    Motion nothing = {0};
//    reccursiveParser(config1, &nothing);
//    printf("EOP\n");
//    printf("fwd Value: %d\n",
//            nothing.next[0]->next[0]->next[0]->next[2]->movement.linear_movement.fwd);
//
//}
//#include <stdlib.h>
//#include <stdio.h>
//#include "src/control/translation_functions.c"

const char applyMotion(Motion* node) {
    //Do motor movements and logic to acheive it
    if(node->func >= (sizeof(functions)/sizeof(functions[0]))) {
        Serial.print("[!]Function: ");
        Serial.print(node->func);
        Serial.println(", not found in func list");
        delay(100);
        return 0;
    }
    if(!functions[node->func](node, tolerance)) {
        Serial.print("[#]Function: ");
        Serial.print(node->func);
        Serial.println(", FAILED.\nStarting Reversal");
        delay(100);
        return 0;
    }

    Serial.print("[*]Finished Execution Function: ");
    Serial.println(node->func);
    delay(100);
    return 1;

    //If successful return 1
    //If unsecsessful return 0
}

void pathFinder(Motion* node) {
    if(node->next[0] == 0) {
        Serial.println("[*]Program finished Execution!\n");
        delay(100);
        exit(1); //Finished Maze
    }
    int i = 0;
    while(i < 10) {
        if(node->next[i] == 0)
            break;
        if(applyMotion(node->next[i]))
            pathFinder(node->next[i]);

        //if control ends reverse
        applyMotion(node);
        i += 1;
    }

    return;

    //try motion. If (1) recurse to one of next nodes
}

//int main() {
//    Motion nothing = {0};
//    reccursiveParser(config1, &nothing);
//    pathFinder(&nothing);
//}

void setup() {
    Serial.begin(9600);
    Serial.println("[*]Initializing TOF sensors");
    tofInit();
    Serial.println("[*]Initializing motors");
    motorInit();
    Motion nothing = {0};
    Serial.println("[*]Building navigation tree");
    reccursiveParser(config1, &nothing);
    Serial.println("[*]Initializing solving algorithms");
    delay(100);
    pathFinder(&nothing);
}

char has_finished = 0;
void loop() {
    if(has_finished == 0) {
        Serial.println("[*]Program Finished Execution");
        has_finished = 1;
    }
}
