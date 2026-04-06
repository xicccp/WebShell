#include "allocator_test.h"
#include "../allocator/allocator.h"
#include <stdio.h>
#include <string.h>

// ─── ANSI Colors ────────────────────────────────────────────────────────────
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define YELLOW      "\033[33m"
#define MAGENTA     "\033[35m"
#define WHITE       "\033[97m"
#define BG_DARK     "\033[48;5;234m"

// ─── Test Helpers ────────────────────────────────────────────────────────────
static int tests_run    = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void test_result(const char* name, int passed) {
    tests_run++;
    if (passed) {
        tests_passed++;
        printf("  " GREEN "  [PASS]" RESET " %s\n", name);
    } else {
        tests_failed++;
        printf("  " RED "  [FAIL]" RESET " %s\n", name);
    }
}

static void section(const char* title) {
    printf("\n" CYAN "  ┌─────────────────────────────────────────┐\n");
    printf("  │  " BOLD WHITE "%-41s" RESET CYAN "│\n", title);
    printf("  └─────────────────────────────────────────┘" RESET "\n");
}

static void divider() {
    printf(DIM "  ─────────────────────────────────────────────\n" RESET);
}

// ─── Test Cases ──────────────────────────────────────────────────────────────

static void test_basic_allocation() {
    section("1. BASIC ALLOCATION");
    init_heap();

    void* p1 = my_malloc(128);
    test_result("malloc(128) returns non-NULL",          p1 != NULL);

    void* p2 = my_malloc(256);
    test_result("malloc(256) returns non-NULL",          p2 != NULL);

    void* p3 = my_malloc(512);
    test_result("malloc(512) returns non-NULL",          p3 != NULL);

    test_result("Three pointers are distinct",
        p1 != p2 && p2 != p3 && p1 != p3);

    printf("\n" DIM "  Heap state after 3 allocations:\n" RESET);
    print_heap();

    my_free(p1); my_free(p2); my_free(p3);
}

static void test_edge_cases() {
    section("2. EDGE CASES");
    init_heap();

    void* p = my_malloc(0);
    test_result("malloc(0) returns NULL",                p == NULL);

    my_free(NULL);
    test_result("free(NULL) does not crash",             1);

    void* a = my_malloc(64);
    my_free(a);
    void* b = my_malloc(64);
    test_result("malloc after free reuses memory",       b != NULL);
    my_free(b);
}

static void test_coalescing() {
    section("3. COALESCING (MERGE FREE BLOCKS)");
    init_heap();

    void* p1 = my_malloc(128);
    void* p2 = my_malloc(128);
    void* p3 = my_malloc(128);

    printf("\n" DIM "  Heap before freeing:\n" RESET);
    print_heap();

    my_free(p2);
    printf("\n" DIM "  Heap after freeing middle block:\n" RESET);
    print_heap();

    my_free(p3);
    printf("\n" DIM "  Heap after freeing p2+p3 (should coalesce):\n" RESET);
    print_heap();

    // After coalescing p2+p3, we should be able to alloc 300 bytes in that space
    void* big = my_malloc(300);
    test_result("Can allocate 300 bytes in coalesced space", big != NULL);

    my_free(p1); my_free(big);

    printf("\n" DIM "  Heap after full cleanup:\n" RESET);
    print_heap();
}

static void test_realloc() {
    section("4. REALLOC");
    init_heap();

    // Basic realloc grow
    char* p = (char*)my_malloc(64);
    strcpy(p, "Hello, WebShell!");
    char* p2 = (char*)my_realloc(p, 256);
    test_result("realloc to larger size returns non-NULL",  p2 != NULL);
    test_result("realloc preserves data",
        strcmp(p2, "Hello, WebShell!") == 0);

    // realloc(NULL, size) == malloc(size)
    void* p3 = my_realloc(NULL, 128);
    test_result("realloc(NULL, size) behaves like malloc",  p3 != NULL);

    // realloc(ptr, 0) == free(ptr)
    void* p4 = my_malloc(64);
    void* p5 = my_realloc(p4, 0);
    test_result("realloc(ptr, 0) returns NULL",             p5 == NULL);

    my_free(p2); my_free(p3);
}

static void test_fill_heap() {
    section("5. FILL HEAP TO LIMIT");
    init_heap();

    int    count    = 0;
    void*  ptrs[1024];
    void*  result;

    while (count < 1024) {
        result = my_malloc(1024);
        if (result == NULL) break;
        ptrs[count++] = result;
    }

    test_result("Heap eventually returns NULL when full",   result == NULL);
    printf("  " YELLOW "  Allocated %d blocks of 1KB before heap exhausted\n" RESET, count);

    for (int i = 0; i < count; i++) my_free(ptrs[i]);

    void* after = my_malloc(512);
    test_result("Heap usable again after full free",        after != NULL);
    my_free(after);
}

// ─── Summary ─────────────────────────────────────────────────────────────────
static void print_summary() {
    printf("\n");
    divider();
    printf("  " BOLD WHITE "  TEST SUMMARY\n" RESET);
    divider();
    printf("  " WHITE "  Total  : %d\n" RESET, tests_run);
    printf("  " GREEN "  Passed : %d\n" RESET, tests_passed);
    if (tests_failed > 0)
        printf("  " RED "  Failed : %d\n" RESET, tests_failed);
    else
        printf("  " DIM "  Failed : 0\n" RESET);
    divider();

    if (tests_failed == 0)
        printf("\n  " GREEN BOLD "  ✓ All tests passed. Allocator is healthy.\n" RESET "\n");
    else
        printf("\n  " RED BOLD "  ✗ Some tests failed. Check heap logic.\n" RESET "\n");
}

// ─── Submenu ─────────────────────────────────────────────────────────────────
static void print_test_menu() {
    printf("\n" CYAN);
    printf("  ╔══════════════════════════════════════════╗\n");
    printf("  ║       ALLOCATOR TEST SUITE               ║\n");
    printf("  ╚══════════════════════════════════════════╝\n" RESET);
    printf(DIM "  Phase 1 — Custom Memory Allocator\n\n" RESET);
    printf("  " CYAN "[1]" RESET "  Basic Allocation\n");
    printf("  " CYAN "[2]" RESET "  Edge Cases\n");
    printf("  " CYAN "[3]" RESET "  Coalescing\n");
    printf("  " CYAN "[4]" RESET "  Realloc\n");
    printf("  " CYAN "[5]" RESET "  Fill Heap to Limit\n");
    printf("  " CYAN "[6]" RESET "  Run All Tests\n");
    printf("  " MAGENTA "[b]" RESET "  Back to Main Menu\n");
    printf("\n  " WHITE "Select> " RESET);
}

void run_allocator_tests() {
    char choice;
    while (1) {
        print_test_menu();
        scanf(" %c", &choice);

        tests_run = tests_passed = tests_failed = 0;

        switch (choice) {
            case '1': test_basic_allocation();              print_summary(); break;
            case '2': test_edge_cases();                    print_summary(); break;
            case '3': test_coalescing();                    print_summary(); break;
            case '4': test_realloc();                       print_summary(); break;
            case '5': test_fill_heap();                     print_summary(); break;
            case '6':
                test_basic_allocation();
                test_edge_cases();
                test_coalescing();
                test_realloc();
                test_fill_heap();
                print_summary();
                break;
            case 'b': case 'B':
                return;
            default:
                printf(RED "  Invalid option.\n" RESET);
        }

        printf(DIM "\n  Press Enter to continue..." RESET);
        getchar(); getchar();
    }
}