#include "benchmark_menu.h"
#include "bench_allocator.h"
#include "bench_http.h"
#include "bench_shell.h"
#include "bench_kv.h"
#include "bench_compressor.h"
#include <stdio.h>

// ─── ANSI Colors ─────────────────────────────────────────────────────────────
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define MAGENTA     "\033[35m"
#define WHITE       "\033[97m"

static void clear_screen() { printf("\033[2J\033[H"); }

static void divider() {
    printf(DIM CYAN "  ══════════════════════════════════════════════════════\n" RESET);
}

static void print_benchmark_menu() {
    clear_screen();
    printf("\n" CYAN BOLD);
    printf("  ██████╗ ███████╗███╗   ██╗ ██████╗██╗  ██╗███╗   ███╗ █████╗ ██████╗ ██╗  ██╗\n");
    printf("  ██╔══██╗██╔════╝████╗  ██║██╔════╝██║  ██║████╗ ████║██╔══██╗██╔══██╗██║ ██╔╝\n");
    printf("  ██████╔╝█████╗  ██╔██╗ ██║██║     ███████║██╔████╔██║███████║██████╔╝█████╔╝ \n");
    printf("  ██╔══██╗██╔══╝  ██║╚██╗██║██║     ██╔══██║██║╚██╔╝██║██╔══██║██╔══██╗██╔═██╗ \n");
    printf("  ██████╔╝███████╗██║ ╚████║╚██████╗██║  ██║██║ ╚═╝ ██║██║  ██║██║  ██║██║  ██╗\n");
    printf("  ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝\n");
    printf(RESET);
    printf(DIM CYAN "  Performance benchmarks: my_malloc vs system, speed, throughput & more\n" RESET);
    printf("\n");
    divider();
    printf("\n  " BOLD WHITE "SELECT MODULE\n\n" RESET);
    printf("  " CYAN  "[1]" RESET "  Allocator Benchmarks     " GREEN  "← Phase 1 complete\n" RESET);
    printf("  " DIM   "[2]" RESET DIM "  HTTP Server Benchmarks   " YELLOW "← Phase 2\n" RESET);
    printf("  " DIM   "[3]" RESET DIM "  Shell Engine Benchmarks  " YELLOW "← Phase 3\n" RESET);
    printf("  " DIM   "[4]" RESET DIM "  KV Store Benchmarks      " YELLOW "← Phase 4\n" RESET);
    printf("  " DIM   "[5]" RESET DIM "  Compressor Benchmarks    " YELLOW "← Phase 5\n" RESET);
    printf("\n");
    printf("  " MAGENTA "[b]" RESET "  Back to Main Menu\n");
    printf("\n");
    divider();
    printf("  " WHITE "Select> " RESET);
}

static void coming_soon(const char* phase) {
    printf("\n");
    divider();
    printf("  " YELLOW BOLD "  %s\n" RESET, phase);
    printf("  " DIM "  This benchmark hasn't been built yet.\n");
    printf("  " DIM "  Complete the phase first!\n" RESET);
    divider();
    printf(DIM "\n  Press Enter to continue..." RESET);
    getchar(); getchar();
}

void run_benchmark_menu() {
    char choice;
    while (1) {
        print_benchmark_menu();
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                clear_screen();
                run_bench_allocator();
                printf(DIM "\n  Press Enter to continue..." RESET);
                getchar(); getchar();
                break;
            case '2':
                coming_soon("Phase 2 — HTTP Server Benchmarks (not yet implemented)");
                break;
            case '3':
                coming_soon("Phase 3 — Shell Engine Benchmarks (not yet implemented)");
                break;
            case '4':
                coming_soon("Phase 4 — KV Store Benchmarks (not yet implemented)");
                break;
            case '5':
                coming_soon("Phase 5 — Compressor Benchmarks (not yet implemented)");
                break;
            case 'b': case 'B':
                return;
            default:
                printf(YELLOW "\n  Invalid option.\n" RESET);
                printf(DIM "  Press Enter to continue..." RESET);
                getchar(); getchar();
        }
    }
}
