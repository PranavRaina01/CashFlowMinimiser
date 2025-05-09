# Cash Flow Minimizer

A C++ program that minimizes the number of transactions among multiple banks using different payment modes. The system uses a world bank as an intermediary when banks don't have common payment modes.

## Features

- Minimizes the number of transactions between banks
- Supports multiple payment modes per bank
- Handles cases where banks don't have common payment modes
- User-friendly interface with menu-driven input
- Input validation and error handling
- Formatted output display

## Prerequisites

- C++ compiler (g++ recommended)
- Make (optional, for using Makefile)

## Compilation

```bash
g++ code.cpp -o cash_flow_minimizer
```

Or using the Makefile:
```bash
make
```

## Usage

1. Run the compiled program:
```bash
./cash_flow_minimizer
```

2. Follow the on-screen instructions to:
   - Enter the number of banks
   - Input bank details (name and payment modes)
   - Enter transaction details
   - View minimized transactions

## Example Input

```
Number of banks: 5

Bank 1 (World Bank):
Name: World_Bank
Payment modes: 2
- Google_Pay
- PayTM

Bank 2:
Name: Bank_B
Payment modes: 1
- Google_Pay

Bank 3:
Name: Bank_C
Payment modes: 1
- Google_Pay

Bank 4:
Name: Bank_D
Payment modes: 1
- PayTM

Bank 5:
Name: Bank_E
Payment modes: 1
- PayTM

Number of transactions: 4
Transaction 1: Bank_B World_Bank 300
Transaction 2: Bank_C World_Bank 700
Transaction 3: Bank_D Bank_B 500
Transaction 4: Bank_E Bank_B 500
```

## License

This project is licensed under the MIT License - see the LICENSE file for details. 
