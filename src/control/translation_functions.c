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

