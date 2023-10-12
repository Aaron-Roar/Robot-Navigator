#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>


#include "types/movement.c"
#include "types/movement_assembler.c"
#include "src/hardware/motors.c"
#include "src/hardware/tof.c"
#include "src/control/translation.c"
#include "src/control/translation_functions.c"
#include "maze-files/formatted-data/config1.h"
#include "src/parser/parser.c"
#include "src/solver/solver.c"
int main() {
    tof_init();
    struct Motion nothing = {0};
    reccursiveParser(config1, &nothing);
    pathFinder(&nothing);
}
