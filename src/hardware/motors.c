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


