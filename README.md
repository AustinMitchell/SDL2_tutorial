# SDL2_tutorial
Project implementing Lazy Foo's SDL tutorials. Original tutorials located here: https://lazyfoo.net/tutorials/SDL/index.php

This is my attempt to implement the tutorials in a more modern style and replacing some of the practices he employs with other solutions I like better. Currently this project doesn't account for Windows solutions without manual intervention. Requires Python3.

### Initialization

This project uses a git submodule. After cloning the project, run these commands:

```bash
git submodule init
git submodule update --remote
```

### Building

Assuming your SDL2 headers and libraries can be found via system path, select a tutorial and run the following command:

```bash
./build -s src/tutorials/SDL-08-geometry-render.cpp
```

To clean the project:

```bash
./build -c
```

### Running

The executable will be placed in `bin/`. To run, either navigate into `bin/` and run the executable, or execute the `run` script.
