//#include "src/parser/parser.c"
//#include "src/hardware/motors.c"
//#include "src/hardware/tof.c"
//#include <stdio.h>

float tolerance = 10;

char lrState(Motion* node, float tolerance) {
    int env_lr = readLeft() + readRight();//read left & right
    int node_lr = node->movement.linear_movement.left + node->movement.linear_movement.right;
    char lr_state = !(((env_lr + env_lr*tolerance) < node_lr) || ((env_lr - env_lr*tolerance) > node_lr)); //boolean for if lr w in tolerance
    return lr_state;
}

char fbState(Motion* node, float tolerance) {
    int env_fb = readFwd() + readBckwd();//read front & back
    int node_fb = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    char fb_state = !(((env_fb + env_fb*tolerance) < node_fb) || ((env_fb - env_fb*tolerance) > node_fb)); //boolean for if lr w in tolerance
    return fb_state;
}


//Checks to see if the node and env 
char checkEnvironmentSum(Motion* node, float tolerance) {
    char lr_state = lrState(node, tolerance);
    char fb_state = fbState(node, tolerance);

    //1:ALL True  2:ALL False  3:LR False  4:FB False
    if(lr_state && fb_state)
        return 1; //All w in tol
    if(!(lr_state || fb_state))
        return 2; //fb_state & lr_state not w in tol
    if(!lr_state)
        return 3; //lr_state not in tol
    if(!fb_state)
        return 4; //fb_state not in tol

}

//(Env - Intended Position)
Position deltaEnvironment(Motion* node, float tolerance) {
    Position pos;
    pos.left = (int)(readLeft() - node->movement.linear_movement.left);
    pos.right = (int)(readRight() - node->movement.linear_movement.right);
    pos.fwd = (int)(readFwd() - node->movement.linear_movement.fwd);
    pos.bckwd = (int)(readBckwd() - node->movement.linear_movement.bckwd);
    return pos;

}


//int main() {
//    Position pos = (Position) {
//        .left = 0,
//        .right = 100,
//        .fwd = 0,
//        .bckwd = 100
//    };
//    Motion* first = {0};
//    //Motion* node = addLinearMovement(pos, 1, first);
//
//    //printf("Feedback: %d\n", checkEnvironmentSum(node, 0.1));
//}

