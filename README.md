# SDL2_tutorial
Project implementing Lazy Foo's SDL tutorials. Original tutorials located here: https://lazyfoo.net/tutorials/SDL/index.php

This is my attempt to implement the tutorials in a more modern style and replacing some of the practices he employs with other solutions I like better. Currently this project doesn't account for Windows solutions without manual intervention.


## Requirements

- Currently only tested with clang++ and C++17
- Python3
- MSVC build tools

## Initialization

This project uses a git submodule. After cloning the project, run these commands:

```bash
git submodule init
git submodule update --remote
```

## Building

Assuming your SDL2 headers and libraries can be found via system path, select a tutorial and run the following command with any of the cpp files from the tutorial folder:

```bash
./build -s src/tutorials/SDL-08-geometry-render.cpp
```

To clean the project:

```bash
./build -c
```

## Running

The executable will be placed in `bin/`. To run, either navigate into `bin/` and run the executable, or execute the `run` script.

## Troubleshooting

#### missing 'sal.h' when trying to compile on windows
This happens when you don't have microsoft's build tools installed on your machine. Try installing them from [here](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)
