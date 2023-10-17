int temp_sense_value = 50;

char bckwd[] =   {1, 3, 5, 7}; //33 35 37 39
char fwd[] =     {2, 4, 6, 8}; //34 36 38 40
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

void drive(char* direction, long milliseconds) {
    writePortC(direction, sizeof(char)*4, 1);
    delay(milliseconds);
    writePortC(direction, sizeof(char)*4, 0);
    delay(500);
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


