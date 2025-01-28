# Simple Port Scanner in C

## Description

This is a simple command-line port scanner written in C. It allows you to scan a range of TCP ports on a specified target IP address or hostname to determine which ports are open.

**Please use this tool responsibly and ethically. Only scan systems you have explicit permission to scan.**  Unauthorized port scanning may be illegal and is generally considered unethical.

## Features

*   **Basic TCP Port Scanning:**  Performs a TCP connect scan to check if ports are open.
*   **Port Range Scanning:**  Scans a user-defined range of ports.
*   **Clear Output:**  Reports open ports to the console.
*   **Simple and Lightweight:**  Written in C with minimal dependencies.

## Prerequisites

To build and run this port scanner, you will need:

*   **A C Compiler:**  Such as GCC (GNU Compiler Collection).  This is typically available on Linux and macOS systems. For Windows, you can use MinGW or Cygwin.
*   **A Terminal or Command Prompt:** To compile and run the program from the command line.
*   **A Linux, macOS, or compatible environment:**  The code utilizes system header files (`sys/socket.h`, `arpa/inet.h`, `netinet/in.h`, `unistd.h`) commonly found in Unix-like operating systems. Windows users will need a compatible environment like MinGW or Cygwin.

## Building (Compilation)

1.  **Save the C code:** Save the provided C code (e.g., `spc.c`) into a file.
2.  **Open your terminal or command prompt.**
3.  **Navigate to the directory where you saved `spc.c`.**
4.  **Compile the code using GCC:**

    ```bash
    gcc spc.c -o spc
    ```

    This command will compile `spc.c` and create an executable file named `spc` in the same directory.

## Usage (Running)

To run the port scanner, use the following command syntax in your terminal:

```bash
./spc <target_ip_address> <start_port> <end_port>
