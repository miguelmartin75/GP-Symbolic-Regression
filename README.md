The goal: symbolic regression. This is just a repository to hold the code I am working on.

Note it is not necessary to compile the GUI yourself, I've provided a pre-compiled version
[here](https://github.com/miguelmartin75/GP-Symbolic-Regression/releases/download/0.1/viewer.app.zip). I am not sure this works on other systems other than my own, hence why I reccomend compiling it for yourself.

# How it's structured

- `lib`
    - The underlying code that performs symbolic regression. It is compiled as a library.
- `cli`
    - All the command line interface code.
- `viewer`
    - The code that implements the GUI, written with Qt.

# Dependencies

- boost is required to compile
- a compiler that supports C++11
    - GCC 4.8+ should work; clang also works, I'm willing to bet 3.0 or 3.1 can compile the code
- Qt for the GUI
- `cmake` to generate makefiles (or protect files) to compile

# How to compile

1. Compile the 'library':
    - `./compile.sh lib`
2. Compile the command line interface:
    - `./compile.sh cli`
3. Compile the viewer
    - Open up the .pro file in QtCreator
