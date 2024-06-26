//#include "movement.h"
//#include <stddef.h>
//#include <stdio.h>

#define branch_limit 2
uint16_t motion_count = 0;
size_t forks[branch_limit];
Motion motions[20] = {0};

Motion* addMotion(union MovementType new_movement, uint8_t type, int func, Motion* prev_movement) {


    if(prev_movement != 0) {
        int i = 0;
        while(i < branch_limit) {
            if(prev_movement->next[i] == 0) {
                prev_movement->next[i] = &motions[motion_count];
                break;
            }
            i += 1;
        }
    }

//    motions[motion_count] = (Motion){
//        .type = type,
//        .func = func,
//        .movement = new_movement,
//        .prev = prev_movement,
//        .next = {0}
//    };
    Motion* mot = &motions[motion_count];

    mot->type = type;
    mot->func = func;
    mot->movement = new_movement;
    mot->prev = prev_movement;
    mot->next[0] = {0};
    mot->next[1] = {0};


    motion_count += 1;


    return &motions[motion_count - 1];
}

Motion* addAngularMovement(int angle, int func, Motion* prev_motion) {
    return addMotion(
            (union MovementType) {
                .angle = angle
            },
            2,
            func,
            prev_motion
    );
}

Motion* addLinearMovement(Position movement, int func, Motion* prev_motion) {
    return addMotion(
            (union MovementType) {
               .linear_movement = movement
            },
            1,
            func,
            prev_motion
    );
}

