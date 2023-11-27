float tolerance = 10;
float safe_z = 20;

float angleD(float ideal, float actual) {
    float rad_angle;
    if(actual < ideal) {
        rad_angle = asinf(actual/ideal); 
//        return 0; //Error when rotation past walls
    }
    else{
        rad_angle = asinf(ideal/actual); 
    }
    float angle = -1*((180/3.1415926)*rad_angle - 90);


    return angle;
}


void correctAngleLR(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.left + node->movement.linear_movement.right;
    pos = currentPosition(9);
    float actual = pos.left + pos.right;
    float angle_0 = angleD(ideal, actual);
    Serial.print("Angle: ");
    Serial.println(angle_0);

    if(angle_0 < angle_tolerance && angle_0 > -1*angle_tolerance) {
    }
    else {
        //Check cw
        rotate(angle_0*angle_check_percentage);
        delay(50);

        pos = currentPosition(9);
        actual = pos.left + pos.right;
        float angle_1 = angleD(ideal, actual);
        Serial.print("Attempted fix Angle: ");
        Serial.println(angle_1);

        if(angle_1 < angle_0) {
            rotate(angle_1*cw_gain); //roatet cw
            Serial.println("Rotated CW");
        }
        else {
            rotate(-1*angle_1*ccw_gain); //rotate ccw
            Serial.println("Rotated CCW");
        }
    }


}

float correctAngleFB(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    pos = currentPosition(9);
    float actual = pos.bckwd + pos.fwd;
    float angle_0 = angleD(ideal, actual);
    Serial.print("Angle: ");
    Serial.println(angle_0);

    if(angle_0 < angle_tolerance && angle_0 > -1*angle_tolerance) {
    }
    else {
        //Check cw
        rotate(angle_0*angle_check_percentage);
        delay(50);

        pos = currentPosition(9);
        actual = pos.bckwd + pos.fwd;
        float angle_1 = angleD(ideal, actual);
        Serial.print("Attempted fix Angle: ");
        Serial.println(angle_1);

        if(angle_1 < angle_0) {
            rotate(angle_1*cw_gain); //roatet cw
            Serial.println("Rotated CW");
        }
        else {
            rotate(-1*angle_1*ccw_gain); //rotate ccw
            Serial.println("Rotated CCW");
        }
    }


}



