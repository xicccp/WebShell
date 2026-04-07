#ifndef ROUTER_H
#define ROUTER_H

#include <stddef.h>

// handler function pointer type
typedef void (*RouteHandler)(int client_fd, const char* path);

// a single registered route
typedef struct Route {
    char         prefix[64];
    char         method[8];
    RouteHandler handler;
} Route;

// the router
typedef struct Router {
    Route*  routes;
    size_t  count;
    size_t  capacity;
} Router;

extern Router* g_router; // global router instance

Router* router_create();
void    router_register(Router* router, const char* method,
                        const char* prefix, RouteHandler handler);
void    router_dispatch(Router* router, const char* method,
                        const char* path, int client_fd);
void    router_free     (Router* router);
void    static_handler  (int client_fd, const char* path);
void    shell_handler  (int client_fd, const char* path);
void    compressor_handler  (int client_fd, const char* path);
void    kv_handler  (int client_fd, const char* path);

#endif // ROUTER_H