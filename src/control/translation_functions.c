char test0(Motion* node, float tolerance) {
    while(1) {
        measure(node);
    }
    return 1;
}

//ideally read in parralele to stop driving
char test1(Motion* node, float tolerance) {
    driveToFB(node);
    return 1;
}

char test2(Motion* node, float tolerance) {
    driveToLR(node);
    return 1;
}

char test3(Motion* node, float tolerance) {
    drive(fwd);
    delay(5000);
    stop(fwd);
    delay(4000);
    return 1;
}

char test4(Motion* node, float tolerance) {//fail
    rotate(90);
    return 1;
}

char test5(Motion* node, float tolerance) {
    while(currentPosition(9).fwd > 60) {
        drive(fwd);
    }
    return 1;
}

char test6(Motion* node, float tolerance) {
    ramp(node);
}

char test7(Motion* node, float tolerance) {
    Servo servo;  // create servo object to control a servo
    servo.attach(11);  // attaches the servo on pin 9 to the servo object
    servo.write(0);
    delay(2000);
    servo.write(80);
    delay(2000);
}

char test8(Motion* node, float tolerance) {
    drive(fwd);
    delay(500);
    stop(fwd);
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

    int end = 0;
    while(end == 0) {
        state = currentPosition(1);
        float actual = state.left + state.right;
        float err = readErrorLR(node);

        if(state.fwd < 23) {
            Serial.println("Going right");
            stop(*fb_dir);
            crawl(bckwd);
        }
        else if(state.bckwd < 23) {
            Serial.println("Going left");
            stop(*fb_dir);
            crawl(fwd);
        }
        else if(err > err_tolerance*(node->movement.linear_movement.left + node->movement.linear_movement.right)) {
            Serial.println("Fixin angle");
            stop(*fb_dir);
            correctAngleLR2(node);
        }
        else if( (fb_dir == &left && state.left <= node->movement.linear_movement.left) && (err < err_tolerance*(node->movement.linear_movement.left + node->movement.linear_movement.right) ) ) {
            stop(*fb_dir);
            end = 1;
        }
        else if( (fb_dir == &right && state.right <= node->movement.linear_movement.right) && (err < err_tolerance*(node->movement.linear_movement.left + node->movement.linear_movement.right)) ) {
            stop(*fb_dir);
            end = 1;
        }
        else if(fb_dir == &left && state.left < (node->movement.linear_movement.left + crawl_zone) ) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else if(fb_dir == &right && state.right < (node->movement.linear_movement.right + crawl_zone) ) {
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

    int end = 0;
    while(end == 0) {
        state = currentPosition(1);
        float actual = state.fwd + state.bckwd;
        float err = readErrorFB(node);

        if(state.left < 23) {
            Serial.println("Going right");
            stop(*fb_dir);
            crawl(right);
        }
        else if(state.right < 23) {
            Serial.println("Going left");
            stop(*fb_dir);
            crawl(left);
        }
        else if(err > err_tolerance*(node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd)) {
            Serial.println("Fixin angle");
            stop(*fb_dir);
            correctAngleFB2(node);
        }
        else if( (fb_dir == &fwd && state.fwd <= node->movement.linear_movement.fwd) && (err < err_tolerance*(node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd) ) ) {
            stop(*fb_dir);
            end = 1;
        }
        else if( (fb_dir == &bckwd && state.bckwd <= node->movement.linear_movement.bckwd) && (err < err_tolerance*(node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd)) ) {
            stop(*fb_dir);
            end = 1;
        }
        else if(fb_dir == &fwd && state.fwd < (node->movement.linear_movement.fwd + crawl_zone) ) {
            Serial.println("Crawling fwd");
            stop(*fb_dir);
            crawl(*fb_dir);
        }
        else if(fb_dir == &bckwd && state.bckwd < (node->movement.linear_movement.bckwd + crawl_zone) ) {
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


char ramp(Motion* node) {
    Direction* fb_dir = &all;
    Position state = currentPosition(1);

    fb_dir = &fwd;

    int end = 0;
    while(currentPosition(1).fwd > 60) {
        state = currentPosition(1);
        float actual = state.left + state.right;
        float err = readErrorLR(node);

        if(state.left < 23) {
            Serial.println("Going right");
            stop(*fb_dir);
            crawl(right);
        }
        else if(state.right < 23) {
            Serial.println("Going left");
            stop(*fb_dir);
            crawl(left);
        }
        else if(err > err_tolerance*(node->movement.linear_movement.left + node->movement.linear_movement.right)) {
            Serial.println("Fixin angle");
            stop(*fb_dir);
            correctAngleLR2(node);
        }
        else {
            Serial.println("driving fwd");
            crawl(*fb_dir);
        }
    }
    stop(*fb_dir);

}

char offroad(Motion* node) {
    drive(fwd);
    delay(5000);
}


char (*functions[]) (Motion*, float) = {test0, test1, test2, test3, test4, test5, test6, test7}; //BUG TEST 0 is used as safe space value and should not be executed but it is one program reversal

