#ifndef SOCKET_H
#define SOCKET_H

int  create_server_socket(int port);

void start_accept_loop(int server_fd);

void close_server_socket(int server_fd);

#endif // SOCKET_H