#include "http.h"
#include "router.h"
#include "../allocator/allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Router* g_router = NULL;

Router* router_create() {
    Router* router = my_malloc(sizeof(Router));
    if(!router) return NULL;
    router->routes = my_malloc(sizeof(Route) * 16);
    if(!router->routes) {
        my_free(router);
        return NULL;
    }
    router->count = 0;
    router->capacity = 16;
    return router;
}

void router_register(Router* router, const char* method, const char* prefix, RouteHandler handler) {
    if (router->count >= router->capacity) {
        router->capacity *= 2;
        router->routes = my_realloc(router->routes, sizeof(Route) * router->capacity);
    }
    Route* r = &router->routes[router->count++];
    strncpy(r->method, method, sizeof(r->method) - 1);
    strncpy(r->prefix, prefix, sizeof(r->prefix) - 1);
    r->handler = handler;
}

void router_dispatch(Router* router, const char* method, const char* path, int client_fd) {
    for (size_t i = 0; i < router->count; i++) {
        Route* r = &router->routes[i];
        if (strcmp(r->method, method) == 0 && strncmp(r->prefix, path, strlen(r->prefix)) == 0) {
            r->handler(client_fd, path);
            return;
        }
    }
    send_404(client_fd);
}   

void static_handler(int client_fd, const char* path) {
    if (strstr(path, "..")) {
        send_404(client_fd);
        return;
    }

    char full_path[512];

    if (strcmp(path, "/") == 0) {
        snprintf(full_path, sizeof(full_path), "frontend/index.html");
    } else {
        snprintf(full_path, sizeof(full_path), "frontend%s", path);
    }

    FILE* file = fopen(full_path, "rb");
    if (!file) {
        send_404(client_fd);
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0 , SEEK_SET);
    char* file_content = my_malloc(file_size);

    if (!file_content) {
        fclose(file);
        send_500(client_fd);
        return;
    }

    fread(file_content, 1, file_size, file);
    fclose(file);
    const char * content_type = get_content_type(full_path);
    send_response(client_fd, "200 OK", content_type, file_content, file_size);

    my_free(file_content);
}

void shell_handler(int client_fd, const char* path) {
    const char* body = "<html><body><h1>Shell — coming in Phase 3</h1></body></html>";
    send_response(client_fd, "200 OK", "text/html", body, strlen(body));
}

void kv_handler(int client_fd, const char* path) {
    const char* body = "<html><body><h1>KV Store — coming in Phase 4</h1></body></html>";
    send_response(client_fd, "200 OK", "text/html", body, strlen(body));
}

void compressor_handler(int client_fd, const char* path) {
    const char* body = "<html><body><h1>Compressor — coming in Phase 5</h1></body></html>";
    send_response(client_fd, "200 OK", "text/html", body, strlen(body));
}

void router_free(Router* router) {
    my_free(router->routes);
    my_free(router);
}
