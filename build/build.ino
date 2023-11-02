
#include <stdlib.h>
#include <stdio.h> 
#include <stddef.h>
#include <stdio.h> 
#include <stdint.h>
#include <Wire.h>
#include <VL53L0X.h>
//#include <stdint.h>

typedef struct Position {
    int fwd, bckwd, left, right;
} Position;

typedef union MovementType {
    struct Position linear_movement;
    int angle;
} MovementType;

typedef struct Motion {
    uint8_t type;
    int func;
    union MovementType movement;

    struct Motion* next[10];
} Motion;
//#include "movement.h"
//#include <stddef.h>
//#include <stdio.h>

#define branch_limit 10
uint16_t motion_count = 0;
size_t forks[branch_limit];
Motion motions[500] = {0};

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

    motions[motion_count] = (Motion){
        .type = type,
        .func = func,
        .movement = new_movement,
        .next = {0}
    };
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

                               //PIN# - 32
char fwd[] =     {2, 4, 6, 8}; //34 36 38 40
char bckwd[] =   {1, 3, 5, 7}; //33 35 37 39
char right[] =   {7, 6, 4, 1}; //39 38 36 33
char left[] =    {8, 5, 3, 2}; //40 37 35 34
char cw[] =      {2, 4, 5, 7}; //34 36 37 39
char ccw[] =     {1, 3, 6, 8}; //33 35 38 40


void writePortC(char* pins, char pins_size, char state) {
    int i = 0;
    long result = 0;
    while(i < pins_size) {
        result |= (1 << pins[i]);
        i += 1;
    }

    if(state) {
        PIOC -> PIO_SODR = result;   // set pin DIRECT PORT ACCESS
    } else{
        PIOC -> PIO_CODR = result;  // clear pin DIRECT PORT ACCESS
    }
}

void drive(char* direction) {
    writePortC(direction, sizeof(char)*4, 1);
}

void stop(char* direction) {
    writePortC(direction, sizeof(char)*4, 0);
}


void motorInit() {
    pinMode (33, OUTPUT);
    pinMode (34, OUTPUT);
    pinMode (35, OUTPUT);
    pinMode (36, OUTPUT);
    pinMode (37, OUTPUT);
    pinMode (38, OUTPUT);
    pinMode (39, OUTPUT);
    pinMode (40, OUTPUT);
    pinMode (41, OUTPUT);
}


//#include <Wire.h>
//#include <VL53L0X.h>

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

Position currentPosition() {
    Position pos;

    pos.left = readLeft();
    pos.right = readRight();
    pos.fwd = readFwd();
    pos.bckwd = readBckwd();

    return pos;
}


//#include "src/parser/parser.c"
//#include "src/hardware/motors.c"
//#include "src/hardware/tof.c"
//#include <stdio.h>

float tolerance = 20;
float safe_z = 20;

char lrState(Motion* node, Position pos, float tolerance) {
    int env_lr = pos.left + pos.right;//read left & right
    int node_lr = node->movement.linear_movement.left + node->movement.linear_movement.right;
    char lr_state = !(((env_lr + tolerance) < node_lr) || ((env_lr - tolerance) > node_lr)); //boolean for if lr w in tolerance
    return lr_state;
}

char fbState(Motion* node, Position pos, float tolerance) {
    int env_fb = pos.fwd + pos.bckwd;//read front & back
    int node_fb = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    char fb_state = !(((env_fb + tolerance) < node_fb) || ((env_fb - tolerance) > node_fb)); //boolean for if lr w in tolerance
    return fb_state;
}


//Checks to see if the node and env 
char checkEnvironmentSum(Motion* node, Position pos, float tolerance) {
    char lr_state = lrState(node, pos, tolerance);
    char fb_state = fbState(node, pos, tolerance);

    //1:ALL True  2:ALL False  3:LR False  4:FB False
    if(lr_state && fb_state)
        return 1; //All w in tol
    if(!(lr_state || fb_state))
        return 2; //fb_state & lr_state not w in tol
    if(!lr_state)
        return 3; //lr_state not in tol
    if(!fb_state)
        return 4; //fb_state not in tol

}

//(Env - Intended Position)
Position deltaEnvironment(Motion* node, Position current, float tolerance) {
//    return (Position) {
//        .left = (int)(readLeft() - node->movement.linear_movement.left),
//        .right = (int)(readRight() - node->movement.linear_movement.right),
//        .fwd = (int)(readFwd() - node->movement.linear_movement.fwd),
//        .bckwd = (int)(readBckwd() - node->movement.linear_movement.bckwd)
//    };
    Position pos;
    pos.left = (int)(current.left - node->movement.linear_movement.left);
    pos.right = (int)(current.right - node->movement.linear_movement.right);
    pos.fwd = (int)(current.fwd - node->movement.linear_movement.fwd);
    pos.bckwd = (int)(current.bckwd - node->movement.linear_movement.bckwd);
    return pos;
}

char boundaryCheckLR(Position pos) {
    if(pos.left > safe_z && pos.right > safe_z)
        return 1;
    return 0;
}
char boundaryCheckFB(Position pos) {
    if(pos.fwd > safe_z && pos.bckwd > safe_z)
        return 1;
    return 0;
}

char boundaryCheck(Position pos) {
    if(boundaryCheckFB(pos)&&boundaryCheckLR(pos))
        return 1;
    return 0;
}

char atSubLocation(int current, int final) {
    if( (current < (final + tolerance))&&(current > (final - tolerance)) )
        return 1;
    return 0;
}
char atLRLocation(Position current, Position final) {
    if(atSubLocation(current.left, final.left)&&atSubLocation(current.right, final.right))
        return 1;
    return 0;
}
char atFBLocation(Position current, Position final) {
    if(atSubLocation(current.fwd, final.fwd)&&atSubLocation(current.bckwd, final.bckwd))
        return 1;
    return 0;
}
char atLocation(Position current, Position final) {
    if(atLRLocation(current, final)&&atFBLocation(current, final))
        return 1;
    return 0;
}



//int main() {
//    Position pos = (Position) {
//        .left = 0,
//        .right = 100,
//        .fwd = 0,
//        .bckwd = 100
//    };
//    Motion* first = {0};
//    //Motion* node = addLinearMovement(pos, 1, first);
//
//    //printf("Feedback: %d\n", checkEnvironmentSum(node, 0.1));
//}

char test0(Motion* node, float tolerance) {
    return 1;
}

//ideally read in parralele to stop driving
char test1(Motion* node, float tolerance) {
    Serial.print("Fwd Distance");
    Serial.println(readFwd());
    Serial.print("Bckwd Distance");
    Serial.println(readBckwd());
    Serial.print("Left Distance");
    Serial.println(readLeft());
    Serial.print("Right Distance");
    Serial.println(readRight());
}

char test2(Motion* node, float tolerance) {
    while(!atLocation(currentPosition(), node->movement.linear_movement)) {//Not at the next spot yet
        //Fix subLocation value LR, if near wall FB swap to FB and fix
        while(!atLRLocation(currentPosition(), node->movement.linear_movement)) {
            if(!boundaryCheckFB(currentPosition())) {
                stop(left);
                stop(right);
                break; //Fix FB at wall
            }
            if(!boundaryCheckLR(currentPosition())) {
                stop(left);
                stop(right);
                if(deltaEnvironment(node, currentPosition(), tolerance).right > 0) {
                    stop(left);
                    drive(right);
                } else {
                    stop(right);
                    drive(left);
                }
            }
            //If angle off fix angle
            if(checkEnvironmentSum(node, currentPosition(), tolerance) == 2) {//Not orientated correctly
                stop(left);
                stop(right);
                //Fix angle
            }
            //
             if(deltaEnvironment(node, currentPosition(), tolerance).right > 0) {
                 stop(left);
                 drive(right);
             } else {
                 stop(right);
                 drive(left);
             }
            //translate direction we need to go
        }
        stop(left);
        stop(right);

        //Fix sublocation value FB, if near wall LR swap to LR and fix
        while(!atFBLocation(currentPosition(), node->movement.linear_movement)) {
            if(!boundaryCheckLR(currentPosition())) {
                stop(fwd);
                stop(bckwd);
                break; //Fix FB at wall
            }
            if(!boundaryCheckFB(currentPosition())) {
                stop(fwd);
                stop(bckwd);
                if(deltaEnvironment(node, currentPosition(), tolerance).fwd > 0) {
                    stop(bckwd);
                    drive(fwd);
                } else {
                    stop(fwd);
                    drive(bckwd);
                }
            }
            //If angle off fix angle
            if(checkEnvironmentSum(node, currentPosition(), tolerance) == 2) {//Not orientated correctly
                stop(fwd);
                stop(bckwd);
                //Fix angle
            }
            //
             if(deltaEnvironment(node, currentPosition(), tolerance).fwd > 0) {
                 stop(bckwd);
                 drive(fwd);
             } else {
                 stop(fwd);
                 drive(bckwd);
             }
            //translate direction we need to go
        }
        stop(fwd);
        stop(bckwd);

    }
    stop(fwd);
    stop(bckwd);
    stop(left);
    stop(right);
    return 1;
}
char test3(Motion* node, float tolerance) {
    drive(ccw);
    return 1;
}
char test4(Motion* node, float tolerance) {//fail
    printf("Test4 FAIL\n");
    return 0;
}

//Robot is facing fwd to ramp
char ramp(Motion* node, float tolerance) {
//    Position delta = deltaEnvironment(node, tolerance);
//    while(delta.fwd > tolerance) {
//        //not at the point we want to be at in fwd direction
//
//        //Make sure correct orientation
//        int deg = 5; //5 degrees
//        while(!lrState(node, tolerance)) { //While lr not alligned
//            rotate(deg);
//        }
//        //If Left and Right not adding up rotate to fix 
//                 //orientation
//        delta = deltaEnvironment(node, tolerance);
//        drive(left, delta.left);
//        drive(forward, 10); //10,,
//
//        delta = deltaEnvironment(node, tolerance); //update state
//    }
    return 1;
}

//Test Function
char toWall(Motion* node, float tolerance) {
//    int deg = 5;
//    Position delta = deltaEnvironment(node, tolerance);
//    while(delta.fwd > tolerance) {
//    
//        delta = deltaEnvironment(node, tolerance);
//        forward();
//        delta = deltaEnvironment(node, tolerance);
//    }
    return 1;
}

char straight(Motion* node, float tollerance) {
//    Position delta = deltaEnvironment(node, tolerance);
//    while(delta.fwd > tolerance) {
//        drive(forward, delta.fwd/2);
//    }
    return 1;
}

char obstacle(Motion* node, float tolerance) {
}

char (*functions[]) (Motion*, float) = {test0, test1, test2, test3, test4}; //BUG TEST 0 is used as safe space value and should not be executed but it is one program reversal


const char config1[]=
"@P1 \n"
"T1:100 10 1000 1200 \n"
"T1:100 55 1000 1200 \n"
"#P1 \n"
" \n"
"@P2 \n"
"T1:10 55 1000 1200 \n"
"#P2 \n"
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
        Serial.println("[!]Function not in list\n");
        return 0;
    }
    if(!functions[node->func](node, tolerance)) {
        Serial.println("[#]Function failed to complete Reversing\n");
        return 0;
    }
    Serial.print("Movement: ");
    Serial.println(node->func);
    return 1;

    //If successful return 1
    //If unsecsessful return 0
}

void pathFinder(Motion* node) {
    if(node->next[0] == 0) {
        Serial.println("[#]Finished Maze!!\n");
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
    motorInit();
    tofInit();
    Motion nothing = {0};
    reccursiveParser(config1, &nothing);
    pathFinder(&nothing);
}

void loop() {
}
