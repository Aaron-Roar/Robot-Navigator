void setup() {
    Serial.begin(9600);
    motorInit();
    Motion nothing = {0};
    reccursiveParser(config1, &nothing);
    pathFinder(&nothing);
}

void loop() {
}
