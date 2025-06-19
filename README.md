# WalletCplus - Digital Wallet Management System

## Introduction

WalletCplus is a comprehensive digital wallet management system built in C++ that provides secure user authentication, wallet management, and cryptocurrency exchange functionality. The system supports both user and administrator roles, offering a complete solution for managing digital assets and transactions.

### Key Features

- **User Authentication & Management**: Secure registration, login, and role-based access control
- **Wallet Management**: Create, manage, and transfer funds between multiple wallets
- **Cryptocurrency Exchange**: USD to coin conversion with admin-controlled pricing
- **Transaction Tracking**: Complete audit trail of all wallet transactions and exchanges
- **Admin Dashboard**: Comprehensive administrative tools for system management
- **OTP Security**: One-Time Password verification for sensitive operations

## Team Members

### Brad - User Service Lead
**Responsibilities:**
- User authentication and registration system
- Password management with OTP verification
- User profile management (name, birth year, role assignment)
- User data persistence and JSON file management
- Security implementation (password hashing)

**Key Contributions:**
- Implemented secure user registration with duplicate username checking
- Developed OTP-based password change system for enhanced security
- Created efficient user lookup using unordered_map for O(1) performance
- Designed JSON-based data persistence for user information

### Steve - Wallet Service Lead
**Responsibilities:**
- Wallet creation and management system
- Transaction processing and tracking
- Cryptocurrency exchange functionality
- Coin supply management and pricing
- Financial data persistence

**Key Contributions:**
- Built comprehensive wallet management with multi-wallet support
- Implemented secure coin transfer system between wallets
- Developed USD-to-coin exchange with supply management
- Created transaction tracking for complete audit trail
- Designed admin-controlled coin pricing and supply limits

## Features Specification

### User Features
1. **Account Management**
   - User registration with username, password, name, and birth year
   - Secure login with password verification
   - Password change with OTP verification
   - Profile information management

2. **Wallet Operations**
   - Create multiple wallets with custom names
   - View wallet balances and transaction history
   - Transfer coins between personal wallets
   - Add funds to wallets
   - Delete unused wallets

3. **Cryptocurrency Exchange**
   - Exchange USD for coins at current market price
   - View current coin price and available supply
   - Track exchange transaction history
   - Real-time balance updates

### Admin Features
1. **System Management**
   - View all registered users and their information
   - Add new users with role assignment
   - Monitor all wallets and transactions
   - Set coin price and maximum supply

2. **Financial Oversight**
   - View complete transaction history
   - Monitor exchange transactions
   - Control coin supply and pricing
   - System-wide financial reporting

### Security Features
- Password hashing for secure storage
- OTP verification for password changes
- Role-based access control
- Input validation and sanitization
- Secure file-based data persistence

## Code Structure

```
wallet/
├── CMakeLists.txt              # Build configuration
├── main.cpp                    # Application entry point
├── data/                       # JSON data files
│   ├── users.json             # User data storage
│   ├── wallets.json           # Wallet data storage
│   ├── transactions.json      # Transaction records
│   ├── exchange_transactions.json # Exchange records
│   └── coin_exchange.json     # Coin exchange settings
├── models/                     # Data models
│   ├── User.h/.cpp            # User entity
│   ├── Wallet.h/.cpp          # Wallet entity
│   ├── Transaction.h/.cpp     # Transaction entity
│   ├── CoinExchange.h/.cpp    # Coin exchange settings
│   └── ExchangeTransaction.h/.cpp # Exchange transaction entity
├── services/                   # Business logic layer
│   ├── UserService.h/.cpp     # User management service
│   └── WalletService.h/.cpp   # Wallet management service
├── interface/                  # Service interfaces
│   ├── UserService.h          # User service interface
│   └── WalletService.h        # Wallet service interface
├── screen/                     # User interface layer
│   ├── userScreen.h/.cpp      # User dashboard
│   ├── adminScreen.h/.cpp     # Admin dashboard
│   └── walletScreen.h/.cpp    # Wallet management UI
├── helper/                     # Utility functions
│   ├── HashHelper.h/.cpp      # Password hashing utilities
│   ├── OTPHelper.h/.cpp       # OTP generation and verification
│   └── File/                  # File management utilities
│       ├── JsonFileManager.h/.cpp # JSON file operations
├── defined/                    # Constants and configuration
│   ├── constant.h/.cpp        # System constants
└── modules/                    # External libraries
    └── json/                   # nlohmann/json library
        └── json.hpp           # JSON parsing and serialization
```

## Libraries Used

### Core Libraries
- **nlohmann/json**: Modern C++ JSON library for data serialization
  - Used for: User data, wallet data, transaction records, and configuration storage
  - Features: Type-safe JSON parsing, easy serialization/deserialization

### Standard Library Components
- **std::unordered_map**: Efficient user lookup and wallet management
- **std::vector**: Transaction and wallet list management
- **std::shared_ptr**: Smart pointer management for memory safety
- **std::optional**: Safe handling of nullable return values
- **std::string**: String manipulation and storage
- **std::iostream**: Console input/output operations

### Build System
- **CMake**: Cross-platform build system
  - Minimum version: 3.16
  - C++17 standard compliance
  - MSVC and GCC/Clang compiler support

## How to Compile

### Prerequisites
- **CMake** (version 3.16 or higher)
- **C++17 compatible compiler**:
  - Microsoft Visual Studio 2017 or later (Windows)
  - GCC 7.0 or later (Linux/macOS)
  - Clang 5.0 or later (macOS/Linux)

### Build Instructions

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd wallet
   ```

2. **Create build directory**
   ```bash
   cmake -S . -B build
   ```

3. **Configure the project**
   ```bash
   cmake ./build/
   ```

4. **Build the project**
   ```bash
   cmake --build build --config Debug
   ```

### Build Options

- **Debug Build**: `cmake --build . --config Debug`
- **Release Build**: `cmake --build . --config Release`
- **Clean Build**: Delete `build` directory and repeat steps 2-4

### Platform-Specific Notes

**Windows (MSVC):**
- Uses MSVC compiler flags for optimization and debugging
- Generates Visual Studio solution files
- Executable: `build/Debug/ServiceApp.exe`

**Linux/macOS (GCC/Clang):**
- Uses GCC/Clang compiler flags
- May require `stdc++fs` library linking
- Executable: `build/ServiceApp`

## How to Run

### Starting the Application

1. **Navigate to the build directory**
   ```bash
   cd build/Debug  # Windows
   cd build        # Linux/macOS
   ```

2. **Run the executable**
   ```bash
   ./ServiceApp.exe  # Windows
   ./ServiceApp      # Linux/macOS
   ```

### Application Flow

1. **Main Menu**
   - Option 1: Register new user account
   - Option 2: Login with existing credentials
   - Option 3: Exit application

2. **Registration Process**
   - Enter username (must be unique)
   - Enter password
   - Enter full name
   - Enter year of birth
   - System creates user account with USER role

3. **Login Process**
   - Enter username and password
   - System validates credentials
   - Routes to appropriate dashboard based on user role

4. **User Dashboard**
   - Wallet management (create, view, transfer, delete)
   - Coin exchange (USD to coins)
   - Transaction history
   - Profile management

5. **Admin Dashboard**
   - User management (view all users, add new users)
   - System monitoring (all wallets, transactions)
   - Coin exchange settings (price, supply)
   - Financial oversight

### Data Persistence

The application uses JSON files for data storage:
- `data/users.json`: User accounts and profiles
- `data/wallets.json`: Wallet information and balances
- `data/transactions.json`: Transaction records
- `data/exchange_transactions.json`: USD-to-coin exchange records
- `data/coin_exchange.json`: Coin pricing and supply settings

### Default Configuration

- **Default Coin Price**: $1.00 USD per coin
- **Initial Coin Supply**: 1000 coins
- **Source Wallet ID**: "SYSTEM_WALLET"
- **Admin Account**: First registered user can be promoted to admin

### Security Notes

- Passwords are hashed using MD5 (for demonstration purposes)
- OTP verification required for password changes
- All user input is validated
- File-based data persistence ensures data integrity

## Troubleshooting

### Common Issues

1. **Build Errors**
   - Ensure CMake version 3.16+ is installed
   - Verify C++17 compiler support
   - Clean build directory and reconfigure

2. **Runtime Errors**
   - Check data directory exists with proper permissions
   - Verify JSON files are not corrupted
   - Ensure sufficient disk space for data storage

3. **Permission Issues**
   - Run as administrator if needed (Windows)
   - Check file/directory permissions (Linux/macOS)

### Support

For technical support or feature requests, please contact:
- **Brad** (User Service): User authentication and management issues
- **Steve** (Wallet Service): Wallet and transaction-related issues

---

**Version**: 1.0.0  
**Last Updated**: December 2024  
**License**: MIT License 