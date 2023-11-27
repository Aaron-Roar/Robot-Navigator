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
echo '#include <math.h>'   >> $file
echo '#include <Wire.h>' >> $file
echo '#include <VL53L0X.h>' >> $file
cat tunes/laminate_floor.h >> $file
cat src/types/movement.c >> $file
cat src/types/movement_assembler.c >> $file
cat src/hardware/motors.c >> $file
cat src/hardware/tof.c >> $file
cat src/control/translation_checker.c >> $file
cat src/control/translation_functions.c >> $file
cat maze-files/formatted-data/config1.h >> $file
cat src/parser/parser.c >> $file
cat src/solver/solver.c >> $file
cat src/main.ino >> $file

arduino-cli compile --fqbn arduino:avr:nano build/
arduino-cli upload -b arduino:avr:nano -p /dev/ttyUSB0 build/build.ino
#stty -F /dev/ttyACM0 9600 raw -clocal -echo
