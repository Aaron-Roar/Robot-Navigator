char test0(Motion* node, float tolerance) {
    return 1;
}
char test1(Motion* node, float tolerance) {
    drive(right, 10000);
    return 1;
}
char test2(Motion* node, float tolerance) {
    drive(left, 1000);
    return 1;
}
char test3(Motion* node, float tolerance) {
    drive(ccw, 1000);
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

