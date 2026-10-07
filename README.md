# c-http-server

A small HTTP server written from scratch in C using POSIX sockets.
This project is primarily for learning how TCP sockets and HTTP
work underneath higher-level web frameworks and libraries.

## Currently supports

- IPv4 TCP sockets
- Binding to a port
- Listening for connections
- Accepting a client
- Receiving an HTTP request
- Sending an HTTP response
- Basic socket error handling

## Limitations

- Handles one client at a time
- Minimal HTTP parsing
- No concurrency
- No HTTPS
- Not intended for production use
