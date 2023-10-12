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
