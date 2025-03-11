Copyright Szabo Cristina-Andreea 2023-2024
# REST API Web Client

## Overview
This project implements a C++ command-line client that communicates with a REST API server. It handles user authentication, session management, and CRUD operations for managing a library of books. The client interacts with the server using HTTP requests and processes JSON data using the `nlohmann/json` library.

## Key Features
- **User Authentication**: Registration, login, and logout functionality with session management using cookies and JWT tokens.
- **Library Access**: Secure access to library resources after authentication.
- **Book Management**: Full CRUD operations for managing books in the library.

## How It Works

### 1. **Register**
- Prompts the user to input a username and password.
- Validates that inputs contain no spaces.
- Sends a POST request to the server with the user details in JSON format.
- Displays a success or error message based on the server's response.

### 2. **Login**
- Requests the user's username and password.
- Validates input and sends a POST request to the login endpoint.
- Stores the received cookie for maintaining the session.
- Displays a success or error message based on the server's response.

### 3. **Enter Library**
- Ensures the user is logged in by verifying the cookie.
- Sends a GET request to acquire access to the library.
- Extracts and stores the JWT token from the server's response for authenticated requests.

### 4. **Get All Books**
- Verifies that the user has a valid JWT token.
- Sends a GET request to retrieve all books from the library.
- Displays the response content, which includes the list of books.

### 5. **Add a Book**
- Prompts the user to input book details: title, author, genre, publisher, and page count.
- Validates that the page count is a numeric value.
- Sends a POST request with the book information in JSON format.
- Displays a success or error message based on the server's response.

### 6. **Get a Specific Book**
- Prompts the user for the book's ID and validates that it's numeric.
- Sends a GET request to fetch detailed information about the specific book.
- Displays the retrieved book details.

### 7. **Delete a Book**
- Prompts the user for the book's ID and validates that it's numeric.
- Sends a DELETE request to remove the book from the library.
- Displays a success or error message based on the server's response.

### 8. **Logout**
- Verifies the user's session using the stored cookie.
- Sends a GET request to terminate the session and clear authentication tokens.
- Displays a success or error message.

### 9. **Exit**
- Terminates the application.


## Technical Details
- **Socket Communication**: Handles low-level TCP socket operations for sending and receiving HTTP requests and responses.
- **Session Management**: Utilizes cookies for session persistence and JWT tokens for secure authentication.
- **JSON Handling**: Leverages the `nlohmann/json` library for efficient parsing and serialization of JSON data.
- **Input Validation**: Ensures inputs such as usernames, passwords, and book details meet specific criteria (e.g., no spaces in usernames, valid numeric page counts).
- **Error Handling**: Provides detailed error messages for connection issues, invalid inputs, and unsuccessful HTTP responses.


**Available Commands**:
   - `register` – Register a new user.
   - `login` – Log in as an existing user.
   - `enter_library` – Gain access to the library.
   - `get_books` – Retrieve the list of all books.
   - `get_book` – Retrieve details of a specific book by ID.
   - `add_book` – Add a new book to the library.
   - `delete_book` – Delete a book by ID.
   - `logout` – Log out of the session.
   - `exit` – Exit the application.
