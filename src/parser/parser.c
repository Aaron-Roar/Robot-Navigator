//#include "../../types/movement.c"
//#include "../../maze-files/formatted-data/config1.h"
//#include <stdio.h>


//FILE *fptr;

//void fileToBuffer(FILE* file, char* buf) {
//    char letter;
//    int i = 0;
//    while((letter = fgetc(file)) != EOF) {
//        buf[i] = letter;
//        i += 1;
//    }
//}


int lineType(const char* paragraph) {
    char types[] = {'#', '\n', '@', '*', 'T', 'R'};
    int i = 0;
    while(i < sizeof(types)) {
        if(*paragraph == types[i])
            return i;
        i += 1;
    }
    return -1;
}



const char charsTillCharacter(const char* paragraph, char character) {
    char count = 0;
    while(paragraph[count] != character) {
        if(paragraph[count] == '\0' || paragraph == NULL)
            return 0;
        count += 1;
    }
    return count;
}

int powerOfTens(int value, char tens_factor) {
    int result = value;

    char i = tens_factor;
    while(i > 0) {
        result *= 10;
        i -= 1;
    }

    return result;

}

void stringCopyTill(const char* src, char* dest, char size) {
    int i = 0;
    while(i < size) {
        dest[i] = (char)src[i];
        i += 1;
    }
}

int stringToNumber(const char* paragraph, char delimiter) {
    char size = charsTillCharacter(paragraph, delimiter) - 1;
    //printf("Chars in %s, till %c, is %d\n", paragraph, delimiter, size);
    int number = 0;

    int i = size;
    while(i >= 0) {
        if(paragraph[i] == '-')
            return number * (-1);
        number += powerOfTens((int)paragraph[i] - 48, size - i);
        i -= 1;
    }
    return number;
}

void stringToNumbers(const char* paragraph, const char delimiter, int* numbers, char numbers_size) {
    int words = 0;
    char size = 0;
    while(words < numbers_size) {
        numbers[words] = stringToNumber(paragraph + size, delimiter);

        words += 1;
        size += (1 + charsTillCharacter(paragraph + size, delimiter));
    }
}

Position buildPosition(const char* paragraph, const char delimiter) {
    int translation[4] = {0};
    char offset = 1 + charsTillCharacter(paragraph, ':');
    stringToNumbers(paragraph + offset, delimiter, translation, 
            sizeof(translation)/sizeof(translation[0]));

    return (Position){
        .fwd = translation[0],
        .bckwd = translation[1],
        .left = translation[2],
        .right = translation[3]
    };
}

int buildAngle(const char* paragraph, const char delimiter) {
    char offset = 1 + charsTillCharacter(paragraph, ':');
    return stringToNumber(paragraph + offset, delimiter);
}

int buildFunction(const char* paragraph, const char delimiter) {
    int func = stringToNumber(paragraph + 1, delimiter);
    return func;
}

const char* locateSubString(const char* paragraph, const char* sub_string, const char sub_size) {
    int i = 0;
    while(paragraph[i] != '\0') {
        if(paragraph[i] == sub_string[0]) {
            //possible substring found
            int j = 0;
            while(j < sub_size) {
                if(paragraph[i + j] != sub_string[j])
                    break; //maybe breaking both loops
                j += 1;
            }
            if(j == sub_size)
                return paragraph + i;
        }
            i += 1;
    }
    printf("[!]COULD NOT FIND MATCH\n");
    return paragraph;
}

void reccursiveParser(const char* paragraph, Motion* prev_motion) {
    //Check NULL
    int line_type = lineType(paragraph);


    if(line_type == 0) {//\#
        //do nothing is end of sequence
        //printf("FinalLineType: %d\n", line_type);
    } else if(line_type == 1) {//\n
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //skip a line just filler space
    } else if(line_type == 2) {//@
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //do nothing begining of sequence
    } else if(line_type == 3) {//*
        char size_of_term = charsTillCharacter(paragraph, '\n');
        char branch_name[size_of_term - 1];
        stringCopyTill(paragraph, branch_name, size_of_term - 1);
        branch_name[0] = '@';
        const char* branch_loc = locateSubString(paragraph, branch_name, sizeof(branch_name) - 0);
        //printf("Branch: %s\n", branch_loc);
        reccursiveParser(branch_loc, prev_motion);
        //recurse down the branch adding nodes

        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1, prev_motion);
        //Call term after branch with current prev_motion
    } else if(line_type == 4) {//T
        int func = buildFunction(paragraph, ':');
        Position pos = buildPosition(paragraph, ' ');
        reccursiveParser(
                paragraph + charsTillCharacter(paragraph, '\n') + 1,
                addLinearMovement(pos, func, prev_motion));
    } else if(line_type == 5) {//R
        int angle = buildAngle(paragraph, ' ');
        int func = buildFunction(paragraph, ':');
        reccursiveParser(paragraph + charsTillCharacter(paragraph, '\n') + 1,
                addAngularMovement(angle, func, prev_motion));

    } else{
    printf("[!]Couldnt Locate type for: %c\n", *paragraph);
    }

}

//int main() {
//    Motion nothing = {0};
//    reccursiveParser(config1, &nothing);
//    printf("EOP\n");
//    printf("fwd Value: %d\n",
//            nothing.next[0]->next[0]->next[0]->next[2]->movement.linear_movement.fwd);
//
//}
