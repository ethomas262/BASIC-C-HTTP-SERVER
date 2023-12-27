# HTTP Server using Winsock

This simple C++ program demonstrates the implementation of a basic HTTP server using Winsock. The server listens on the loopback address (127.0.0.1) and port 8080. When a client connects, it reads a request from the client and responds with a predefined HTTP response stored in the "response.http" file.

## Prerequisites

- Windows environment (Winsock library is used)
- C++ compiler

## Usage

1. Compile the program using a C++ compiler.
2. Ensure that the "response.http" file contains the desired HTTP response.
3. Run the compiled executable.

## Code Structure

- **Initialization:** Initializes the Winsock library and creates a socket.
- **Binding:** Binds the socket to the loopback address and port 8080.
- **Listening:** Starts listening for incoming connections.
- **Acceptance:** Accepts incoming client connections.
- **Read Request:** Reads the client's request.
- **Send Response:** Sends the predefined HTTP response to the client.
- **Cleanup:** Closes sockets and cleans up Winsock resources.

## Error Handling

- The program includes error handling for various stages such as initialization, socket creation, binding, and listening.

## Notes

- This is a basic example and may require additional features for real-world usage.
- Customize the "response.http" file content to modify the HTTP response sent to clients.
- Error messages are printed to standard error (cerr).

Feel free to modify and enhance the code based on your specific requirements.

