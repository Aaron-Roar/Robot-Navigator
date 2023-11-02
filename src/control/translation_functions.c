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

