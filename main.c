#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests/allocator_test.h"

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
#define BLUE        "\033[34m"

// ─── Utilities ────────────────────────────────────────────────────────────────
static void clear_screen() {
    printf("\033[2J\033[H");
}

static void divider() {
    printf(DIM CYAN "  ══════════════════════════════════════════════════════\n" RESET);
}

// ─── ASCII Art ────────────────────────────────────────────────────────────────
static void print_banner() {
    printf("\n" CYAN BOLD);
    printf("  ██╗    ██╗███████╗██████╗ ███████╗██╗  ██╗███████╗██╗     ██╗\n");
    printf("  ██║    ██║██╔════╝██╔══██╗██╔════╝██║  ██║██╔════╝██║     ██║\n");
    printf("  ██║ █╗ ██║█████╗  ██████╔╝███████╗███████║█████╗  ██║     ██║\n");
    printf("  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══██║██╔══╝  ██║     ██║\n");
    printf("  ╚███╔███╔╝███████╗██████╔╝███████║██║  ██║███████╗███████╗███████╗\n");
    printf("   ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
    printf(RESET);
    printf(DIM CYAN "  A browser-accessible Unix shell built from scratch in C\n" RESET);
    printf("\n");
    divider();
    printf(DIM "  Modules: Allocator · HTTP Server · Shell · KV Store · Compressor\n" RESET);
    divider();
}

// ─── Phase Status ────────────────────────────────────────────────────────────
static void print_phase_status() {
    printf("\n");
    printf("  " BOLD WHITE "PROJECT PHASES\n" RESET);
    printf("\n");
    printf("  " GREEN  "[✓]" RESET " Phase 1  —  Custom Memory Allocator\n");
    printf("  " YELLOW "[…]" RESET " Phase 2  —  HTTP Server\n");
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
    printf("  " CYAN  "[1]" RESET "  Allocator Tests       " GREEN  "← Phase 1 complete\n" RESET);
    printf("  " DIM   "[2]" RESET DIM "  HTTP Server Tests     " YELLOW "← Coming in Phase 2\n" RESET);
    printf("  " DIM   "[3]" RESET DIM "  Shell Engine Tests    " YELLOW "← Coming in Phase 3\n" RESET);
    printf("  " DIM   "[4]" RESET DIM "  KV Store Tests        " YELLOW "← Coming in Phase 4\n" RESET);
    printf("  " DIM   "[5]" RESET DIM "  Compressor Tests      " YELLOW "← Coming in Phase 5\n" RESET);
    printf("\n");
    printf("  " CYAN  "[s]" RESET "  Show Phase Status\n");
    printf("  " MAGENTA "[q]" RESET "  Quit\n");
    printf("\n");
    divider();
    printf("  " WHITE "Select> " RESET);
}

// ─── Coming Soon ─────────────────────────────────────────────────────────────
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
                clear_screen();
                run_allocator_tests();
                break;
            case '2':
                clear_screen();
                coming_soon("Phase 2 — HTTP Server (not yet implemented)");
                break;
            case '3':
                clear_screen();
                coming_soon("Phase 3 — Shell Engine (not yet implemented)");
                break;
            case '4':
                clear_screen();
                coming_soon("Phase 4 — KV Store (not yet implemented)");
                break;
            case '5':
                clear_screen();
                coming_soon("Phase 5 — Compressor (not yet implemented)");
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
                printf(RED "\n  Invalid option. Try again.\n" RESET);
                printf(DIM "  Press Enter to continue..." RESET);
                getchar(); getchar();
        }
    }

    return 0;
}