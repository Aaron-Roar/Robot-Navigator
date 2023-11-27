void setup() {
    Serial.begin(9600);
    Serial.println("[*]Initializing TOF sensors");
    tofInit();
    Serial.println("[*]Initializing motors");
    motorInit();
    Motion nothing = {0};
    Serial.println("[*]Building navigation tree");
    reccursiveParser(config1, &nothing);
    Serial.println("[*]Initializing solving algorithms");
    delay(100);
    pathFinder(&nothing);
}

char has_finished = 0;
void loop() {
    if(has_finished == 0) {
        Serial.println("[*]Program Finished Execution");
        has_finished = 1;
    }
}
