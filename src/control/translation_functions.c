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

