#include "bench_allocator.h"
#include "../allocator/allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ─── ANSI Colors ─────────────────────────────────────────────────────────────
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define YELLOW      "\033[33m"
#define WHITE       "\033[97m"

// ─── Config ───────────────────────────────────────────────────────────────────
#define BENCH_N         10000
#define BENCH_SMALL     64
#define BENCH_MEDIUM    256
#define BENCH_LARGE     1024
#define VARIED_COUNT    8

static size_t varied_sizes[VARIED_COUNT] = { 16, 32, 64, 128, 256, 512, 768, 1024 };

// ─── Timing ───────────────────────────────────────────────────────────────────
static double elapsed_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec  - start.tv_sec)  * 1000.0
         + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

// ─── Table Helpers ────────────────────────────────────────────────────────────
static void print_header() {
    printf(CYAN BOLD);
    printf("\n  ╔══════════════════════════════════════════════════════════════╗\n");
    printf(    "  ║        ALLOCATOR BENCHMARK — my_malloc vs system malloc    ║\n");
    printf(    "  ║        %d iterations per test                           ║\n", BENCH_N);
    printf(    "  ╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf(CYAN "  ┌────────────────────────┬───────────┬───────────┬────────────┐\n");
    printf(     "  │  Test                  │ my_malloc │  system   │  verdict   │\n");
    printf(     "  ├────────────────────────┼───────────┼───────────┼────────────┤\n" RESET);
}

static void print_footer() {
    printf(CYAN "  └────────────────────────┴───────────┴───────────┴────────────┘\n" RESET);
}

static void print_row(const char* label, double my_ms, double sys_ms) {
    double ratio = my_ms / sys_ms;
    const char* verdict;
    const char* color;

    if      (ratio < 1.2) { verdict = "on par    "; color = GREEN;  }
    else if (ratio < 2.0) { verdict = "close     "; color = YELLOW; }
    else if (ratio < 5.0) { verdict = "slower    "; color = YELLOW; }
    else                  { verdict = "much slower"; color = RED;   }

    printf("  │  %-22s │ %7.3f ms │ %7.3f ms │ %s%.2fx %-5s%s│\n",
        label, my_ms, sys_ms, color, ratio, verdict, RESET);
}

static void print_section(const char* title) {
    printf(CYAN "\n  ├── " BOLD WHITE "%s" RESET CYAN
           " ────────────────────────────────────────────────\n" RESET, title);
}

// ─── Speed Benchmarks ────────────────────────────────────────────────────────

static void bench_seq_alloc() {
    struct timespec s, e;
    void* ptrs[BENCH_N];

    init_heap();
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) ptrs[i] = my_malloc(BENCH_SMALL);
    timespec_get(&e, TIME_UTC);
    double my_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) my_free(ptrs[i]);

    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) ptrs[i] = malloc(BENCH_SMALL);
    timespec_get(&e, TIME_UTC);
    double sys_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) free(ptrs[i]);

    print_row("Sequential alloc (64B)", my_ms, sys_ms);
}

static void bench_seq_free() {
    struct timespec s, e;
    void* ptrs[BENCH_N];

    init_heap();
    for (int i = 0; i < BENCH_N; i++) ptrs[i] = my_malloc(BENCH_SMALL);
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) my_free(ptrs[i]);
    timespec_get(&e, TIME_UTC);
    double my_ms = elapsed_ms(s, e);

    for (int i = 0; i < BENCH_N; i++) ptrs[i] = malloc(BENCH_SMALL);
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) free(ptrs[i]);
    timespec_get(&e, TIME_UTC);
    double sys_ms = elapsed_ms(s, e);

    print_row("Sequential free (64B)", my_ms, sys_ms);
}

static void bench_mixed() {
    struct timespec s, e;
    void* ptrs[BENCH_N];

    init_heap();
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) {
        ptrs[i] = my_malloc(BENCH_MEDIUM);
        if (i % 2 == 0 && i > 0) {
            my_free(ptrs[i - 1]);
            ptrs[i - 1] = NULL;
        }
    }
    timespec_get(&e, TIME_UTC);
    double my_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) if (ptrs[i]) my_free(ptrs[i]);

    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++) {
        ptrs[i] = malloc(BENCH_MEDIUM);
        if (i % 2 == 0 && i > 0) { free(ptrs[i - 1]); ptrs[i-1] = NULL; }
    }
    timespec_get(&e, TIME_UTC);
    double sys_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) if (ptrs[i]) free(ptrs[i]);

    print_row("Mixed alloc+free (256B)", my_ms, sys_ms);
}

static void bench_varied() {
    struct timespec s, e;
    void* ptrs[BENCH_N];

    init_heap();
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++)
        ptrs[i] = my_malloc(varied_sizes[i % VARIED_COUNT]);
    timespec_get(&e, TIME_UTC);
    double my_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) my_free(ptrs[i]);

    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < BENCH_N; i++)
        ptrs[i] = malloc(varied_sizes[i % VARIED_COUNT]);
    timespec_get(&e, TIME_UTC);
    double sys_ms = elapsed_ms(s, e);
    for (int i = 0; i < BENCH_N; i++) free(ptrs[i]);

    print_row("Varied sizes (16-1024B)", my_ms, sys_ms);
}

static void bench_large() {
    struct timespec s, e;
    int n = BENCH_N / 20;
    void* ptrs[BENCH_N / 20];

    init_heap();
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < n; i++) ptrs[i] = my_malloc(BENCH_LARGE);
    timespec_get(&e, TIME_UTC);
    double my_ms = elapsed_ms(s, e);
    for (int i = 0; i < n; i++) my_free(ptrs[i]);

    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < n; i++) ptrs[i] = malloc(BENCH_LARGE);
    timespec_get(&e, TIME_UTC);
    double sys_ms = elapsed_ms(s, e);
    for (int i = 0; i < n; i++) free(ptrs[i]);

    print_row("Large blocks (1KB)", my_ms, sys_ms);
}

// ─── Fragmentation Benchmark ──────────────────────────────────────────────────
static void bench_fragmentation() {
    struct timespec s, e;
    void* ptrs[BENCH_N];

    // Fill heap, free every other block to fragment it
    init_heap();
    int count = 0;
    while (count < BENCH_N) {
        void* p = my_malloc(64);
        if (!p) break;
        ptrs[count++] = p;
    }
    for (int i = 0; i < count; i += 2) {
        my_free(ptrs[i]);
        ptrs[i] = NULL;
    }

    // Now measure how long it takes to search fragmented heap
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < 1000; i++) my_malloc(32);
    timespec_get(&e, TIME_UTC);
    double fragmented_ms = elapsed_ms(s, e);

    // Fresh heap baseline
    init_heap();
    timespec_get(&s, TIME_UTC);
    for (int i = 0; i < 1000; i++) my_malloc(32);
    timespec_get(&e, TIME_UTC);
    double fresh_ms = elapsed_ms(s, e);

    print_row("Fragmented vs fresh heap", fragmented_ms, fresh_ms);
}

// ─── Utilization Report ───────────────────────────────────────────────────────
static void bench_utilization() {
    init_heap();

    // Allocate a mix of blocks
    void* ptrs[100];
    size_t total_requested = 0;
    size_t sizes[8] = { 16, 32, 64, 128, 256, 512, 768, 1024 };

    for (int i = 0; i < 100; i++) {
        size_t s = sizes[i % 8];
        ptrs[i] = my_malloc(s);
        if (ptrs[i]) total_requested += s;
    }

    // Count header overhead
    size_t header_overhead = 0;
    size_t usable           = 0;
    size_t wasted           = 0;

    Block* current = (Block*)heap;
    int    block_count = 0;

    while ((char*)current < heap + HEAP_SIZE) {
        header_overhead += sizeof(Block);
        if (!current->free) {
            usable += current->size;
        } else {
            wasted += current->size;
        }
        block_count++;
        current = (Block*)((char*)current + sizeof(Block) + current->size + sizeof(Footer));
    }

    printf(CYAN "\n  ┌─── UTILIZATION REPORT ──────────────────────────────────────┐\n" RESET);
    printf("  │  Total heap size     : %-10zu bytes                       │\n", (size_t)HEAP_SIZE);
    printf("  │  Blocks allocated    : %-10d                              │\n", block_count);
    printf("  │  Header overhead     : %-10zu bytes  (%.1f%% of heap)      │\n",
        header_overhead, (double)header_overhead / HEAP_SIZE * 100);
    printf("  │  Usable (in use)     : %-10zu bytes  (%.1f%% of heap)      │\n",
        usable, (double)usable / HEAP_SIZE * 100);
    printf("  │  Free (available)    : %-10zu bytes  (%.1f%% of heap)      │\n",
        wasted, (double)wasted / HEAP_SIZE * 100);
    printf("  │  Requested by caller : %-10zu bytes                       │\n", total_requested);
    printf(CYAN "  └──────────────────────────────────────────────────────────────┘\n" RESET);

    for (int i = 0; i < 100; i++) if (ptrs[i]) my_free(ptrs[i]);
}

// ─── Notes ────────────────────────────────────────────────────────────────────
static void print_notes() {
    printf("\n");
    printf(DIM "  Notes:\n");
    printf("  · my_malloc uses segregated free bins — near O(1) average allocation\n");
    printf("  · within each bin, allocation is first-fit\n");
    printf("  · boundary tags (header + footer) enable O(1) coalescing\n");
    printf("  · fragmentation can still impact bin search within size classes\n");
    printf("  · small allocations may suffer from internal fragmentation\n");
    printf("  · metadata overhead is %zu bytes per block (header + footer)\n",
        sizeof(Block) + sizeof(Footer));
    printf("  · minimum block size enforced to avoid unusable splits\n" RESET);
    printf("\n");
}

// ─── Entry Point ─────────────────────────────────────────────────────────────
void run_bench_allocator() {
    print_section("SPEED");
    print_header();
    bench_seq_alloc();
    bench_seq_free();
    bench_mixed();
    bench_varied();
    bench_large();
    print_footer();

    print_section("FRAGMENTATION");
    print_header();
    bench_fragmentation();
    print_footer();

    bench_utilization();
    print_notes();
}
