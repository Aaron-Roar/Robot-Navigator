#include <stdlib.h>
#include <stdio.h>
#include "./control/translation_functions.c"

const char applyMotion(struct Motion* node) {
    //Do motor movements and logic to acheive it
    if(node->func >= (sizeof(functions)/sizeof(functions[0]))) {
        printf("[!]Function not in list\n");
        return 0;
    }
    if(!functions[node->func](node, tolerance)) {
        printf("[#]Function failed to complete Reversing\n");
        return 0;
    }
    return 1;

    //If successful return 1
    //If unsecsessful return 0
}

void pathFinder(struct Motion* node) {
    if(node->next[0] == 0) {
        printf("[#]Finished Maze!!\n");
        exit(1); //Finished Maze
    }
    int i = 0;
    while(i < 10) {
        if(node->next[i] == 0)
            break;
        if(applyMotion(node->next[i]))
            pathFinder(node->next[i]);

        //if control ends reverse
        applyMotion(node);
        i += 1;
    }

    return;

    //try motion. If (1) recurse to one of next nodes
}

void init() {
    struct Motion nothing = {0};
    reccursiveParser(config1, &nothing);
    pathFinder(&nothing);
}

void loop() {
}

