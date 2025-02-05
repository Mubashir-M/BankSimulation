# BankSimulation

BankSimulation is a C++ application that simulates basic banking operations, providing a user-friendly interface for managing bank accounts, performing transactions, and viewing account information.

## Features

- **Account Management**: Create new bank accounts with unique IDs and secure passwords.
- **User Authentication**: Secure login system to access individual accounts.
- **Transactions**: Perform deposits, withdrawals, and transfers between accounts.
- **Account Balance**: View real-time account balances and transaction history.

## Requirements

- **C++17** or later
- **CMake** 3.10 or higher
- **SQLite3**: For database management
- **SQLiteCpp**: C++ wrapper for SQLite3
- **ImGui**: For graphical user interface

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Mubashir-M/BankSimulation.git
   cd BankSimulation
   ```
2. **Install Dependencies**: This involves installing SQLite3, SQLiteCpp, and ImGui.

   - **SQLite3**: Download and install SQLite3 from the [official website](https://www.sqlite.org/index.html). Follow the instructions for your specific operating system.

   - **SQLiteCpp**:

   ```
   git clone https://github.com/SRombauts/SQLiteCpp.git
   cd SQLiteCpp
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
   ```

   - **ImGui**: The recommended way to intergrate ImGui as a Git submodule:

   ```
   git submodule add [https://github.com/ocornut/imgui.git](https://github.com/ocornut/imgui.git) external/imgui
   git submodule update --init --recursive
   ```

   - Alternatively, you can download ImGui directly, but the submodule method is generally preferred for dependency management.

3. **Build the Project:**
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

## **Usage**

```
./BankSimulation
```

## **Acknowledgements**

[SQLiteCpp](https://github.com/SRombauts/SQLiteCpp) for the SQLite C++ wrapper.

[ImGui](https://github.com/ocornut/imgui) for the graphical user interface library.
