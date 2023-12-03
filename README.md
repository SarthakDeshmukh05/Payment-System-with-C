# Token Transaction System

This is a simple command-line interface program for a token-based transaction system written in C. The program allows users to register, log in, make payments, check their token balance, view transaction history, and change their passwords. Additionally, an admin view is available for modifying user tokens.

## Features

- **User Registration**: Users can register by providing a username, a strong login password, and a strong transaction password.

- **User Authentication**: Users can log in using their username and login password. Failed login attempts are logged.

- **Token Transactions**: Logged-in users can make token payments to other registered users. Transaction history is maintained.

- **Balance Inquiry**: Users can check their current token balance.

- **Transaction History**: Users can view a list of their previous transactions, including sent and received tokens.

- **Password Management**: Users can change their login and transaction passwords securely.

- **Admin View**: An admin can modify the token balance of any registered user.

- **Inactivity Logout**: Users are automatically logged out if there is a period of inactivity.

## Requirements

- This program requires a C compiler to build and run.
