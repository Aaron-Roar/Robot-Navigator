typedef struct Gains {
    float Kp;
    float Ki;
    float Kd;
} Gains;

typedef struct Clamps {
    float prop_max;
    float prop_min;

    float int_max;
    float int_min;

    float dif_max;
    float dif_min;

    float out_max;
    float out_min;
} Clamps;

typedef struct PID {
    Gains gain;

    float tau;
    float T;

    float integrator;
    float differentiator;
//    Clamps clamp;

    float prev_error;
    float prev_measurment;

    float out;
} PID;

PID pidInit() {
    PID car;

    car.gain.Kp = control_Kp;
    car.gain.Ki = control_Ki;
    car.gain.Kd = control_Kd;

    car.T = control_T;
    car.tau = control_tau;

    car.integrator = 0;
    car.differentiator = 0;
    car.prev_error = 0;
    car.prev_measurment = 0;
    car.out = 0;
}

void pidReset(PID car) {
    car.integrator = 0;
    car.differentiator = 0;
    car.prev_error = 0;
    car.prev_measurment = 0;
    car.out = 0;
}

float proportional(PID* pid, float error) {
    float proportion = pid->gain.Kp * error;

//    if(proportion > pid->clamp.prop_max)
//        return pid->clamp.prop_max;
//    if(proportion < pid->clamp.prop_min)
//        return pid->clamp.prop_min;

    return proportion;
}

float integrator(PID* pid, float error) {
    float integrator = pid->integrator + (0.5f * pid->gain.Ki * pid->T * (error + pid->prev_error));
    
//    if(integrator > pid->clamp.int_max)
//        return pid->clamp.int_max;
//    if(integrator < pid->clamp.int_min)
//        return pid->clamp.int_min;

    return integrator;
}

float differentiator(PID* pid, float measurment) {
    //Avoids kick by doing derivative of measurement rather than error
    float differentiator = (2.0f * pid->gain.Kd * (measurment - pid->prev_measurment)
                         + (2.0f * pid->tau - pid->T) * pid->differentiator)
                         / (2.0f * pid->tau + pid->T);

    return differentiator;
}

float pidUpdate(PID* pid, float set_point, float measurment) {
    float error = set_point - measurment;

    float proportion = proportional(pid, error);
    float integration = integrator(pid, error);
    float derivative = differentiator(pid, measurment);

    float output = proportion + integration + derivative;
    pid->out = output;

    pid->prev_measurment = measurment;
    pid->prev_error = error;

    return pid->out;
}
