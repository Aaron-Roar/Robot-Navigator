//#include "src/parser/parser.c"
//#include "src/hardware/motors.c"
//#include "src/hardware/tof.c"
//#include <stdio.h>

float tolerance = 20;
float safe_z = 20;

char lrState(Motion* node, Position pos, float tolerance) {
    int env_lr = pos.left + pos.right;//read left & right
    int node_lr = node->movement.linear_movement.left + node->movement.linear_movement.right;
    char lr_state = !(((env_lr + tolerance) < node_lr) || ((env_lr - tolerance) > node_lr)); //boolean for if lr w in tolerance
    return lr_state;
}

char fbState(Motion* node, Position pos, float tolerance) {
    int env_fb = pos.fwd + pos.bckwd;//read front & back
    int node_fb = node->movement.linear_movement.fwd + node->movement.linear_movement.bckwd;
    char fb_state = !(((env_fb + tolerance) < node_fb) || ((env_fb - tolerance) > node_fb)); //boolean for if lr w in tolerance
    return fb_state;
}


//Checks to see if the node and env 
char checkEnvironmentSum(Motion* node, Position pos, float tolerance) {
    char lr_state = lrState(node, pos, tolerance);
    char fb_state = fbState(node, pos, tolerance);

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
Position deltaEnvironment(Motion* node, Position current, float tolerance) {
//    return (Position) {
//        .left = (int)(readLeft() - node->movement.linear_movement.left),
//        .right = (int)(readRight() - node->movement.linear_movement.right),
//        .fwd = (int)(readFwd() - node->movement.linear_movement.fwd),
//        .bckwd = (int)(readBckwd() - node->movement.linear_movement.bckwd)
//    };
    Position pos;
    pos.left = (int)(current.left - node->movement.linear_movement.left);
    pos.right = (int)(current.right - node->movement.linear_movement.right);
    pos.fwd = (int)(current.fwd - node->movement.linear_movement.fwd);
    pos.bckwd = (int)(current.bckwd - node->movement.linear_movement.bckwd);
    return pos;
}

char boundaryCheckLR(Position pos) {
    if(pos.left > safe_z && pos.right > safe_z)
        return 1;
    return 0;
}
char boundaryCheckFB(Position pos) {
    if(pos.fwd > safe_z && pos.bckwd > safe_z)
        return 1;
    return 0;
}

char boundaryCheck(Position pos) {
    if(boundaryCheckFB(pos)&&boundaryCheckLR(pos))
        return 1;
    return 0;
}

char atSubLocation(int current, int final) {
    if( (current < (final + tolerance))&&(current > (final - tolerance)) )
        return 1;
    return 0;
}
char atLRLocation(Position current, Position final) {
    if(atSubLocation(current.left, final.left)&&atSubLocation(current.right, final.right))
        return 1;
    return 0;
}
char atFBLocation(Position current, Position final) {
    if(atSubLocation(current.fwd, final.fwd)&&atSubLocation(current.bckwd, final.bckwd))
        return 1;
    return 0;
}
char atLocation(Position current, Position final) {
    if(atLRLocation(current, final)&&atFBLocation(current, final))
        return 1;
    return 0;
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

