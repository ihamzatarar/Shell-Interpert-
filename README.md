# Custom C Shells: CLI & GUI

## About

This repository hosts two distinct shells implemented in C: a command-line interface (CLI) shell and a graphical user interface (GUI) shell. Inspired by a [Hacker Noon tutorial](https://hackernoon.com/u/MIMA) and the practical approach demonstrated in the [lets-build-a-linux-shell](https://github.com/moisam/lets-build-a-linux-shell) GitHub repository, these shells are designed to provide hands-on experience with the internals of command interpreters in Linux.

### Features

- **Command-Line Shell**: A basic shell that runs in the terminal, allowing users to execute commands and scripts in a traditional text-based environment.
- **GUI Shell**: An extension of the command-line shell with a graphical interface, making it more accessible and user-friendly, especially for those who prefer graphical over text-based interaction.

## Getting Started

### Prerequisites

- GCC (GNU Compiler Collection)
- GTK+ 3.0 (for the GUI shell)
- Basic knowledge of C and Linux environment

### Installation

1. **Clone the Repository**

    ```bash
    git clone https://github.com/ihamzatarar/Custom-Shell-Interpert.git
    cd [Custom-Shell-Interpert]
    ```

2. **Build the Command-Line Shell**

    Navigate to the command-line shell directory:

    ```bash
    cd CLI Shell
    make
    ```

3. **Build the GUI Shell**

    Ensure GTK+ 3.0 is installed, then navigate to the GUI shell directory:

    ```bash
    cd ../GUI Shell
    gcc -o shell executor.c initsh.c main.c node.c parser.c prompt.c scanner.c source.c builtins/builtins.c builtins/dump.c symtab/symtab.c $(pkg-config --cflags --libs gtk+-3.0)
    ```

### Usage

- **Running the CLI Shell**: In the `CLI Shell` directory, execute `./shell`.
- **Running the GUI Shell**: In the `GUI Shell` directory, execute `./shell`.

## Documentation

For detailed information about the functionality and the design of the shells, refer to the `docs` folder.

## License

[GNU GENERAL PUBLIC LICENSE](LICENSE)

## Acknowledgments

- Inspiration and foundational concepts were adapted from [Hacker Noon's tutorial](https://hackernoon.com/u/MIMA).
- Core functionalities were based on the examples provided in [lets-build-a-linux-shell](https://github.com/moisam/lets-build-a-linux-shell).
