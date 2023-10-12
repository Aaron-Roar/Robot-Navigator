
#Should the format be
#T1:blah & T2:blah
#where the 1 and 2 refer to functions for how to acomplish movements
#OR
#T:blah & T:blah
#and the solving method is just smarter
#
#Decision Is Smart Solver as no changes need to be made
#My change this for phase 2

for i in $(ls ./raw-data/)
do

    name=$(echo $i | sed 's/.mz//' )
    echo "" > ./formatted-data/"${name}.h"

    touch ./formatted-data/"${name}.h"
    echo "const char ${name}[]=" >> ./formatted-data/"${name}.h"

    cat ./raw-data/${i} | while read line
    do
        echo "\"${line} \n\"" >> ./formatted-data/"${name}.h"
    done
    echo "\"\0\";" >> ./formatted-data/"${name}.h"

done

