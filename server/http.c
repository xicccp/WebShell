#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../allocator/allocator.h"
#include "router.h"

void parse_request(char* buffer, char* method, char* path) {
    sscanf(buffer, "%s %s", method, path);
}

void handle_client(int client_fd) {
    char buffer[1024];
    char method[16] = {0};
    char path[256]  = {0};

    size_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        parse_request(buffer, method, path);
    } else {
        send_500(client_fd);
        return;
    }

    router_dispatch(g_router, method, path, client_fd); // dispatch to router
}

void send_response(int client_fd, const char* status, const char* content_type, const char* body, size_t body_length) {
    char header[1024];
    int header_length = snprintf(header, sizeof(header),
        "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n",
        status, content_type, body_length);

    if (header_length < 0 || header_length >= (int)sizeof(header)) {
        return;
    }

    send(client_fd, header, header_length, 0);
    if (body_length > 0 && body != NULL) {
        send(client_fd, body, body_length, 0);
    }
}

void send_404(int client_fd) {
    const char* body = "<html><body><h1>404 The mandate was not found</h1></body></html>";
    send_response(client_fd, "404 Not Found", "text/html" ,body, strlen(body));
}

void send_500(int client_fd) {
    const char* body = "<html><body><h1>500 The palace has crumbled</h1></body></html>";
    send_response(client_fd, "500 Internal Server Error", "text/html", body, strlen(body));
}

void send_redirect(int client_fd, const char* location) {
    char response[1024];
    int response_length = snprintf(response, sizeof(response),
        "HTTP/1.1 302 Found\r\nLocation: %s\r\nContent-Length: 0\r\n\r\n", location);
    send(client_fd, response, response_length, 0);
}

const char* get_content_type(const char* path) {
    const char* text =strrchr(path, '.');
    if (text) {
        if (strcmp(text, ".html") == 0) return "text/html";
        if (strcmp(text, ".css") == 0) return "text/css";
        if (strcmp(text, ".js") == 0) return "application/javascript";
        if (strcmp(text, ".png") == 0) return "image/png";
        if (strcmp(text, ".jpg") == 0) return "image/jpeg";
    }
    return "application/octet-stream";
}