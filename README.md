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
To use a maze file it first must be translated into C source code by
```bash
./maze-files/formatter.sh
```
