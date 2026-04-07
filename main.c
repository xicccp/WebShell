#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests/allocator_test.h"
#include "benchmarks/benchmark_menu.h"
#include "allocator/allocator.h"
#include "server/socket.h"
#include "server/router.h"

// ─── ANSI Colors ─────────────────────────────────────────────────────────────
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define YELLOW      "\033[33m"
#define MAGENTA     "\033[35m"
#define WHITE       "\033[97m"

// ─── Utilities ────────────────────────────────────────────────────────────────
static void clear_screen() { printf("\033[2J\033[H"); }

static void divider() {
    printf(DIM CYAN "  ══════════════════════════════════════════════════════\n" RESET);
}

// ─── ASCII Banner ─────────────────────────────────────────────────────────────
static void print_banner() {
    printf("\n" CYAN BOLD);
    printf("  ██╗    ██╗███████╗██████╗ ███████╗██╗  ██╗███████╗██╗     ██╗\n");
    printf("  ██║    ██║██╔════╝██╔══██╗██╔════╝██║  ██║██╔════╝██║     ██║\n");
    printf("  ██║ █╗ ██║█████╗  ██████╔╝███████╗███████║█████╗  ██║     ██║\n");
    printf("  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══██║██╔══╝  ██║     ██║\n");
    printf("  ╚███╔███╔╝███████╗██████╔╝███████║██║  ██║███████╗███████╗███████╗\n");
    printf("   ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
    printf(RESET);
    printf(DIM CYAN "  A browser-accessible Unix shell, built from scratch in C\n" RESET);
    printf("\n");
    divider();
    printf(DIM "  Modules: Allocator · HTTP Server · Shell · KV Store · Compressor\n" RESET);
    divider();
}

// ─── Phase Status ─────────────────────────────────────────────────────────────
static void print_phase_status() {
    printf("\n");
    printf("  " BOLD WHITE "PROJECT PHASES\n\n" RESET);
    printf("  " GREEN  "[✓]" RESET " Phase 1  —  Custom Memory Allocator\n");
    printf("  " GREEN  "[✓]" RESET " Phase 2  —  HTTP Server\n");
    printf("  " YELLOW "[…]" RESET " Phase 3  —  Shell Engine\n");
    printf("  " YELLOW "[…]" RESET " Phase 4  —  Key-Value Store\n");
    printf("  " YELLOW "[…]" RESET " Phase 5  —  File Compressor\n");
    printf("  " YELLOW "[…]" RESET " Phase 6  —  Integration & Frontend\n");
    printf("  " YELLOW "[…]" RESET " Phase 7  —  Polish & Portfolio\n");
    printf("\n");
}

// ─── Main Menu ────────────────────────────────────────────────────────────────
static void print_menu() {
    printf("\n");
    printf("  " BOLD WHITE "MAIN MENU\n\n" RESET);
    printf("  " CYAN  "[1]" RESET "  Module Tests             " GREEN  "← run per-phase test suites\n"  RESET);
    printf("  " CYAN  "[2]" RESET "  Benchmarks               " GREEN  "← performance vs system libs\n" RESET);
    printf("  " CYAN  "[3]" RESET "  Start HTTP Server        " GREEN  "← Phase 2\n" RESET);
    printf("\n");
    printf("  " CYAN  "[s]" RESET "  Show Phase Status\n");
    printf("  " MAGENTA "[q]" RESET "  Quit\n");
    printf("\n");
    divider();
    printf("  " WHITE "Select> " RESET);
}

// ─── Tests Submenu ────────────────────────────────────────────────────────────
static void coming_soon(const char* phase) {
    printf("\n");
    divider();
    printf("  " YELLOW BOLD "  %s\n" RESET, phase);
    printf("  " DIM "  This module hasn't been built yet.\n");
    printf("  " DIM "  Complete the current phase first!\n" RESET);
    divider();
    printf(DIM "\n  Press Enter to continue..." RESET);
    getchar(); getchar();
}

static void print_tests_menu() {
    clear_screen();
    printf("\n" CYAN BOLD);
    printf("  ████████╗███████╗███████╗████████╗███████╗\n");
    printf("  ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝\n");
    printf("     ██║   █████╗  ███████╗   ██║   ███████╗\n");
    printf("     ██║   ██╔══╝  ╚════██║   ██║   ╚════██║\n");
    printf("     ██║   ███████╗███████║   ██║   ███████║\n");
    printf("     ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝\n");
    printf(RESET DIM CYAN "  Per-phase test suites — validate correctness before benchmarking\n\n" RESET);
    divider();
    printf("\n  " BOLD WHITE "SELECT PHASE\n\n" RESET);
    printf("  " CYAN  "[1]" RESET "  Allocator Tests          " GREEN  "← Phase 1 complete\n" RESET);
    printf("  " CYAN  "[2]" RESET "  HTTP Server Tests        " YELLOW "← coming soon\n" RESET);
    printf("  " DIM   "[3]" RESET DIM "  Shell Engine Tests       " YELLOW "← Phase 3\n" RESET);
    printf("  " DIM   "[4]" RESET DIM "  KV Store Tests           " YELLOW "← Phase 4\n" RESET);
    printf("  " DIM   "[5]" RESET DIM "  Compressor Tests         " YELLOW "← Phase 5\n" RESET);
    printf("\n");
    printf("  " MAGENTA "[b]" RESET "  Back\n");
    printf("\n");
    divider();
    printf("  " WHITE "Select> " RESET);
}

static void run_tests_menu() {
    char choice;
    while (1) {
        print_tests_menu();
        scanf(" %c", &choice);
        switch (choice) {
            case '1': clear_screen(); run_allocator_tests(); break;
            case '2': coming_soon("Phase 2 — HTTP Server Tests (not yet implemented)");    break;
            case '3': coming_soon("Phase 3 — Shell Engine Tests (not yet implemented)");   break;
            case '4': coming_soon("Phase 4 — KV Store Tests (not yet implemented)");       break;
            case '5': coming_soon("Phase 5 — Compressor Tests (not yet implemented)");     break;
            case 'b': case 'B': return;
            default:
                printf(YELLOW "\n  Invalid option.\n" RESET);
                printf(DIM "  Press Enter to continue..." RESET);
                getchar(); getchar();
        }
    }
}

// ─── Server Initialization ───────────────────────────────────────────────────

static void start_server() {
    clear_screen();
    printf("\n" CYAN BOLD "  Starting WebShell Server...\n\n" RESET);

    init_heap();
    g_router = router_create();
    router_register(g_router, "GET", "/shell",      shell_handler);
    router_register(g_router, "GET", "/kv",         kv_handler);
    router_register(g_router, "GET", "/compress",   compressor_handler);
    router_register(g_router, "GET", "/",           static_handler); // ← always last

    printf("  " GREEN "[✓]" RESET " Router initialized — %zu routes registered\n", g_router->count);

    int server_fd = create_server_socket(8080);
    printf("  " GREEN "[✓]" RESET " Listening on " CYAN "http://localhost:8080\n" RESET);
    printf("  " DIM   "  Press Ctrl+C to stop the server\n\n" RESET);

    start_accept_loop(server_fd);

    // only reached on shutdown
    close_server_socket(server_fd);
    router_free(g_router);
}

// ─── Entry Point ─────────────────────────────────────────────────────────────
int main() {
    char choice;
    while (1) {
        clear_screen();
        print_banner();
        print_menu();
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                run_tests_menu();
                break;
            case '2':
                run_benchmark_menu();
                break;
            case '3':
                start_server();
                break;
            case 's': case 'S':
                clear_screen();
                print_banner();
                print_phase_status();
                printf(DIM "\n  Press Enter to continue..." RESET);
                getchar(); getchar();
                break;
            case 'q': case 'Q':
                clear_screen();
                printf("\n" CYAN BOLD "  Goodbye. Keep building.\n\n" RESET);
                return 0;
            default:
                printf(RED "\n  Invalid option.\n" RESET);
                printf(DIM "  Press Enter to continue..." RESET);
                getchar(); getchar();
        }
    }
    return 0;
}