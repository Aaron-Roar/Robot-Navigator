//#include <stdint.h>

typedef struct Position {
    int fwd, bckwd, left, right;
} Position;

typedef union MovementType {
    struct Position linear_movement;
    int angle;
} MovementType;

typedef struct Motion {
    char type;
    int func;
    union MovementType movement;

    struct Motion* prev;
    struct Motion* next[2];
} Motion;

//Motor Type
typedef struct Direction {
    char* motors;
    char type;
    struct Direction* opo_direction;
} Direction;
