# Robot Navigator

Short description or tagline about what the project does.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Installation

Provide step-by-step instructions on how to install and use the project. You can include code snippets if necessary.

```bash
$ git clone url
$ cd url
```
Program maze by adding a maze file.
Currently The maze file added will have to be referenced in source code.
Assemble the source with the build script then upload and compile to the arduino.

##Usage

First create a maze file with path options such as

```bash
$ touch new_mazefile.mz .
```

###Basic Maze Format
The maze or mz format follows the below structure of {Fwd Bckwd Left Right}. Where the numerical values are in millimeters to a surface with respect to the vehicles Fwd, Bckwd, and etc surfaces. A change only in the left and right value would result in the vehicle translating to the left or right. The symbol T# represents a translational type of movement, which is further described with the #. Different translational algorithms can be selected by the # symbol.

###Control Flow
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

