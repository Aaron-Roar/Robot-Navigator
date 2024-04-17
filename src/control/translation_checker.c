float tolerance = 10;
float safe_z = 20;

void measure(Motion* node) {
    Position pos = currentPosition(9);
    Serial.print("Fwd: ");
    Serial.println(pos.fwd);
    Serial.print("Bckwd: ");
    Serial.println(pos.bckwd);
    Serial.print("Left: ");
    Serial.println(pos.left);
    Serial.print("Right: ");
    Serial.println(pos.right);
    Serial.print("Angle FB: ");
    Serial.println(angleD((node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd), (pos.fwd + pos.bckwd)));
    Serial.print("Angle LR: ");
    Serial.println(angleD((node->movement.linear_movement.left + node->movement.linear_movement.right), (pos.left + pos.right)));
    Serial.println("-----------------------");

}

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

    if(angle > 90)
        return 90;

    return angle;
}

float readAngleFB(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    pos = currentPosition(9);
    float actual = pos.bckwd + pos.fwd;
    float angle_0 = angleD(ideal, actual);

    rotate(angle_0*angle_check_percentage);

    pos = currentPosition(9);
    actual = pos.bckwd + pos.fwd;
    float check_angle = angleD(ideal, actual);

    if(check_angle < angle_0)
        return 1*check_angle;

    return -1*check_angle;

}

float readAngleLR(Motion* node) {
    Position pos;

    float ideal = node->movement.linear_movement.left + node->movement.linear_movement.right;
    pos = currentPosition(9);
    float actual = pos.left + pos.right;
    float angle_0 = angleD(ideal, actual);

    rotate(angle_0*angle_check_percentage);

    pos = currentPosition(9);
    actual = pos.left + pos.right;
    float check_angle = angleD(ideal, actual);

    if(check_angle < angle_0)
        return 1*check_angle;

    return -1*check_angle;

}

void correctAngleLR(Motion* node) {
    float d_angle = readAngleLR(node);
    rotate(d_angle);
}

float correctAngleFB(Motion* node) {
        float d_angle = readAngleFB(node);
        rotate(d_angle);

}

float readErrorFB(Motion* node) {
    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    Position pos = currentPosition(9);
    float actual = pos.fwd + pos.bckwd;
    
    return abs(ideal - actual);
}

float readErrorLR(Motion* node) {
    float ideal = node->movement.linear_movement.left + node->movement.linear_movement.right;
    Position pos = currentPosition(9);
    float actual = pos.left + pos.right;
    
    return abs(ideal - actual);

}

float correctAngleFB2(Motion* node) {
    float err = readErrorFB(node);
    while(err > err_tolerance*(node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd)) {
        rotate(30);
        delay(40);
        float err_check = readErrorFB(node);
        delay(40);

        if(err_check > err)
            rotate(-60);
        delay(40);
        err = readErrorFB(node);
        delay(40);
    }
}

float correctAngleLR2(Motion* node) {
    float err = readErrorLR(node);
    while(err > err_tolerance*(node->movement.linear_movement.left + node->movement.linear_movement.right)) {
        rotate(30);
        delay(30);
        float err_check = readErrorLR(node);
        delay(30);

        if(err_check > err)
            rotate(-60);
        delay(30);
        err = readErrorLR(node);
        delay(30);
    }
}


void pidCorrectFB(Motion* node) {
    float ideal = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
     /*How long since we last calculated*/
    int count = 0;
    unsigned long lastTime;
    double Input;
    float Output;
    double Setpoint = 0;
    double errSum, lastErr;
    double kp = 0.02;
    double ki = 0.04;
    double kd = 0.04;

    while(count < 5) {

        float real_angle;


            Input = real_angle;

            unsigned long now = millis();
            double timeChange = (double)(now - lastTime);
            double error = Setpoint - Input;
            errSum += (error * timeChange);
            double dErr = (error - lastErr) / timeChange;
  
            Output = kp * error + ki * errSum + kd * dErr;
            Serial.print("Pid intended angle change: ");
            Serial.println(kp * error + ki * errSum + kd * dErr);
            rotate(Output);

            Position pos2 = currentPosition(9);
            float actual2 = pos2.bckwd + pos2.fwd;
            float angle_3 = angleD(ideal, actual2);
            Serial.print("Pid corrected Angle: ");
            Serial.println(angle_3);
  
            /*Remember some variables for next time*/
            lastErr = error;
            lastTime = now;
        count += 1;
    
    }

}


