# Robot Navigator

Short description or tagline about what the project does.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Installation
The project depends on the arduino compiler and uploader, specifically the cli. 
This can be bypassed by removing the last three lines in the build.sh and manually compiling and uploading the the microcontroller.

Installing arduino-cli https://arduino.github.io/arduino-cli/0.35/
```
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/local/bin sh
```

Clone the git repository from github and change into the project directory
```bash
$ git clone url
$ cd url
```
Program maze by adding a maze file.
Currently The maze file added will have to be referenced in source code.
Assemble the source with the build script then upload and compile to the arduino.

## Usage
The robot navigates by maze files which are first programmed by a user then translated into navigational instructions.

To create a maze file create a file with the .mz extension in the raw-data maze-files directory.
```bash
$ touch new_mazefile.mz ./maze-files/raw-data
```

To use a maze file it first must be created following the maze format briefly described in the following sections.

### Basic Maze Format
The maze or mz format follows the below structure of {Fwd Bckwd Left Right}. Where the numerical values are in millimeters to a surface with respect to the vehicles Fwd, Bckwd, and etc surfaces. A change only in the left and right value would result in the vehicle translating to the left or right. The symbol T# represents a translational type of movement, which is further described with the #. Different translational algorithms can be selected by the # symbol.

### Control Flow
The file is parsed in order from top to bottom with the exception of links. A link is identified with the * symbol, representing an alternative path to attempt before the insturctions after the link. If the path fails due to a multitude of factors the translations and rotations are reversed where the instructions after the link are then executed.

```
@Main
T1:50 300 5 95
*Solution2
T1:10 340 5 95
#Main

@Solution2
T2:50 20 95 5
T2:10 60 95 5
#Solution2
```

![Alt text](https://github.com/Aaron-Roar/Robot-Navigator/blob/main/doc/sample_maze.png "Sample Maze Diagram")

### Using a Maze File
To use a created maze file it must be compiled with the rest program. This can be acheived by adding the line below to build.sh.
```bash
cat maze-files/formatted-data/new_mazefile.h >> $file
```

To use reference the maze paths specified in the newly added maze file it must be referenced in the C source which can acheived by modifiying the main.ino as seen below.
```c
    reccursiveParser(new_mazefile, &nothing);
```

### Modifying Maze Algorithms
As path solving algorithms are quite dependent on application specific tasks they will most likley require alteration. This can easily be acheived by modifying the file src/control/translation_functions.c.

A sample function could be as simple as the below. Where the Motion* node and float tolerance arguements respectivley describe the current node location representing the position and intended path in the decision tree aswell as the allowed tolerance in mm for the next translation. 
```c
char newMotion(Motion* node, float tolerance) {
    drive(fwd);

    while(currentPosition(1).fwd > node->movement.linear_movement.fwd) {
        //Scan environment for obstacles until the current fwd psotion in at or below the fwd psoition in the maze file

        //If object detected and the movement cant be completed we must revert to the trunk of the current link
        //return 0;
    }
    stop(fwd);

    return 1; //The movement completed succesfully continue down current link
}
```

This function can then be added to the available functions in the solver by appending to the function list as seen below.
```bash
char (*functions[]) (Motion*, float) = {failureCase, normalMovements, newMotion};
```

The index number of this function corrosponds to how its referenced in the maze file. In this case newMotion is at index 3 so a corrosponding maze file to use this function would be as seen below.

```
@Main
T1:50 300 5 95
*Solution
T1:10 340 5 95
#Main

@NewSolution
T2:50 20 95 5
T2:10 60 95 5
#NewSolution
```

## Contributing
This project has reached EOL, End Of Life and is no longer being developed or taking pull requests. However feel free to fork the project and make it your own. I am very loose on legal so do as you like, just make it good :).
