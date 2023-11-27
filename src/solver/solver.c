//#include <stdlib.h>
//#include <stdio.h>
//#include "src/control/translation_functions.c"

const char applyMotion(Motion* node) {
    //Do motor movements and logic to acheive it
    if(node->func >= (sizeof(functions)/sizeof(functions[0]))) {
        Serial.print("[!]Function: ");
        Serial.print(node->func);
        Serial.println(", not found in func list");
        delay(100);
        return 0;
    }
    if(!functions[node->func](node, tolerance)) {
        Serial.print("[#]Function: ");
        Serial.print(node->func);
        Serial.println(", FAILED.\nStarting Reversal");
        delay(100);
        return 0;
    }

    Serial.print("[*]Finished Execution Function: ");
    Serial.println(node->func);
    delay(100);
    return 1;

    //If successful return 1
    //If unsecsessful return 0
}

void pathFinder(Motion* node) {
    if(node->next[0] == 0) {
        Serial.println("[*]Program finished Execution!\n");
        delay(100);
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

//int main() {
//    Motion nothing = {0};
//    reccursiveParser(config1, &nothing);
//    pathFinder(&nothing);
//}

