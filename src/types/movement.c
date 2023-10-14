//#include <stdint.h>

typedef struct Position {
    int fwd, bckwd, left, right;
} Position;

typedef union MovementType {
    struct Position linear_movement;
    int angle;
} MovementType;

typedef struct Motion {
    uint8_t type;
    int func;
    union MovementType movement;

    struct Motion* next[10];
} Motion;
