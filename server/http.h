#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>  // size_t

void        parse_request    (char* buffer, char* method, char* path);
void        handle_client    (int client_fd);
void        send_response    (int client_fd, const char* status,
                              const char* content_type,
                              const char* body, size_t body_length);
void        send_404         (int client_fd);
void        send_500         (int client_fd);
void        send_redirect    (int client_fd, const char* location);
const char* get_content_type (const char* path);

#endif // HTTP_H