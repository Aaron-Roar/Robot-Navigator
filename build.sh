file='./build/build.ino'
cd ./maze-files/
ash formatter.sh
cd ../

echo "" > $file
echo '#include <stdlib.h>' >> $file
echo '#include <stdio.h> ' >> $file
echo '#include <stddef.h>' >> $file
echo '#include <stdio.h> ' >> $file
echo '#include <stdint.h>' >> $file
cat types/movement.c >> $file
cat types/movement_assembler.c >> $file
cat src/hardware/motors.c >> $file
cat src/hardware/tof.c >> $file
cat src/control/translation.c >> $file
cat src/control/translation_functions.c >> $file
cat maze-files/formatted-data/config1.h >> $file
cat src/parser/parser.c >> $file
cat src/solver/solver.c >> $file
