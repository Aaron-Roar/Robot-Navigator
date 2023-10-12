
#include <stdlib.h>
#include <stdio.h> 
#include <stddef.h>
#include <stdio.h> 
#include <stdint.h>
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

void forward(); //Move forward 1mm
void backward(); //Move bckwrd 1mm
void left(); //move left 1mm
void right(); //move right 1mm

int temp_sense_value = 50;



void drive(void direction(), int distance) { //move in direction by mm
    int cycles = distance;
    while(cycles > 0) {
        direction();
        cycles -= 1;
    }
}

void rotate(int degrees);
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



//#include "src/parser/parser.c"
//#include "src/hardware/motors.c"
//#include "src/hardware/tof.c"
//#include <stdio.h>

float tolerance = 10;

char lrState(Motion* node, float tolerance) {
    int env_lr = readLeft() + readRight();//read left & right
    int node_lr = node->movement.linear_movement.left + node->movement.linear_movement.right;
    char lr_state = !(((env_lr + env_lr*tolerance) < node_lr) || ((env_lr - env_lr*tolerance) > node_lr)); //boolean for if lr w in tolerance
    return lr_state;
}

char fbState(Motion* node, float tolerance) {
    int env_fb = readFwd() + readBckwd();//read front & back
    int node_fb = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    char fb_state = !(((env_fb + env_fb*tolerance) < node_fb) || ((env_fb - env_fb*tolerance) > node_fb)); //boolean for if lr w in tolerance
    return fb_state;
}


//Checks to see if the node and env 
char checkEnvironmentSum(Motion* node, float tolerance) {
    char lr_state = lrState(node, tolerance);
    char fb_state = fbState(node, tolerance);

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
Position deltaEnvironment(Motion* node, float tolerance) {
    Position pos;
    pos.left = (int)(readLeft() - node->movement.linear_movement.left);
    pos.right = (int)(readRight() - node->movement.linear_movement.right);
    pos.fwd = (int)(readFwd() - node->movement.linear_movement.fwd);
    pos.bckwd = (int)(readBckwd() - node->movement.linear_movement.bckwd);
    return pos;

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

//#include "src/control/translation.c"

#define PF_F 2 //Passenger_Front_Forward signal
#define PF_B 3 //Passenger_Front_Backward signal

#define DF_F 4 //Driver_Front_Forward signal
#define DF_B 5 //Driver_Front_Backward signal

#define PR_F 6 //Passenger_Rear_Forward signal
#define PR_B 7 //Passenger_Rear_Backward signal

#define DR_F 8 //Driver_Rear_Forward signal
#define DR_B 9 //Driver_Rear_Backward signal

void forward(){
//  digitalWrite(PF_F, HIGH);
//  digitalWrite(DF_F, HIGH);
//  digitalWrite(PR_F, HIGH);
//  digitalWrite(DR_F, HIGH);
//  delay(10);
//
//  digitalWrite(PF_F, LOW);
//  digitalWrite(DF_F, LOW);
//  digitalWrite(PR_F, LOW);
//  digitalWrite(DR_F, LOW);
}

void backward(){
//  digitalWrite(PF_B, HIGH);
//  digitalWrite(DF_B, HIGH);
//  digitalWrite(PR_B, HIGH);
//  digitalWrite(DR_B, HIGH);
//  delay(10);
//
//  digitalWrite(PF_B, LOW);
//  digitalWrite(DF_B, LOW);
//  digitalWrite(PR_B, LOW);
//  digitalWrite(DR_B, LOW);
}

void left(){
//  digitalWrite(PF_F, HIGH);
//  digitalWrite(DF_B, HIGH);
//  digitalWrite(PR_B, HIGH);
//  digitalWrite(DR_F, HIGH);
//  delay(10);
//
//  digitalWrite(PF_F, LOW);
//  digitalWrite(DF_B, LOW);
//  digitalWrite(PR_B, LOW);
//  digitalWrite(DR_F, LOW);
}

void right(){
//  digitalWrite(PF_B, HIGH);
//  digitalWrite(DF_F, HIGH);
//  digitalWrite(PR_F, HIGH);
//  digitalWrite(DR_B, HIGH);
//  delay(10);
//
//  digitalWrite(PF_B, LOW);
//  digitalWrite(DF_F, LOW);
//  digitalWrite(PR_F, LOW);
//  digitalWrite(DR_B, LOW);
}

void motor_init() {
//pinMode(DF_F,OUTPUT);
//pinMode(DF_B,OUTPUT);
//
//pinMode(DR_F,OUTPUT);
//pinMode(DR_B,OUTPUT);
//
//pinMode(PF_F,OUTPUT);
//pinMode(PF_B,OUTPUT);
//
//pinMode(PR_F,OUTPUT);
//pinMode(PR_B,OUTPUT);
}

char test1(Motion* node, float tolerance) {
    printf("Test1\n");
    forward();
    backward();
    return 1;
}
char test2(Motion* node, float tolerance) {
    printf("Test2\n");
    left();
    return 1;
}
char test3(Motion* node, float tolerance) {
    printf("Test3\n");
    right();
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

char (*functions[]) (Motion*, float) = {test1, test1, test2, test3, test4};


const char config1[]=
"@P1 \n"
"*P2 \n"
"T2:10 55 1000 1200 \n"
"T3:10 55 1000 1200 \n"
"#P1 \n"
" \n"
"@P2 \n"
"T1:10 55 1000 1200 \n"
"T2:10 55 1000 1200 \n"
"T3:10 55 1000 1200 \n"
"T4:10 55 1000 1200 \n"
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
        printf("[!]Function not in list\n");
        return 0;
    }
    if(!functions[node->func](node, tolerance)) {
        printf("[#]Function failed to complete Reversing\n");
        return 0;
    }
    return 1;

    //If successful return 1
    //If unsecsessful return 0
}

void pathFinder(Motion* node) {
    if(node->next[0] == 0) {
        printf("[#]Finished Maze!!\n");
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

