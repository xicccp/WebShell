# WebShell

A browser-accessible Unix shell written in C/C++, with four built-in subsystems: a custom memory allocator, a key-value store, a file compressor, and an HTTP server as the backbone. Type commands into a webpage, they run on the server and stream output back in real time.

---

## Architecture

```
Browser
   │  HTTP / WebSocket
   ▼
HTTP Server
   │
   ▼
Shell Parser & Executor
   ├── fork/exec ──────────→ System commands (ls, cat, grep, etc.)
   ├── kv [set/get/del] ──→ Key-Value Store  (uses custom allocator)
   └── compress/decompress → Huffman Compressor (uses custom allocator)
```

---

## Modules

### Custom Memory Allocator
A `malloc`/`free` implementation built from scratch using a free-bin strategy with block coalescing. Used internally by the KV store and compressor instead of the standard library.

### HTTP Server
A single-threaded TCP server that parses raw HTTP requests, serves the frontend UI, and streams shell output back to the browser via chunked transfer encoding.

### Shell Engine
Forks and execs system commands, handles pipes (`ls | grep foo`) and redirection (`> file.txt`), and exposes built-in commands (`cd`, `clear`, `help`, `exit`). Output is captured and streamed to the browser.

### Key-Value Store
An in-memory hash map exposed as shell built-ins. Persists to a `.kvdb` file on exit and reloads on startup.

```bash
kv set name John
kv get name
kv del name
kv list
```

### File Compressor
Huffman encoding-based compression and decompression. Tree nodes and encoding tables are allocated through the custom allocator.

```bash
compress file.txt        # outputs file.txt.huf
decompress file.txt.huf  # restores original
```

---

## Development Checklist

### Phase 1 — Custom Memory Allocator
- [X] Define memory pool / heap structure
- [X] Implement `my_malloc()` using free-list
- [X] Implement `my_free()` with block coalescing
- [X] Implement `my_realloc()`
- [X] Write test suite (alloc, free, fragmentation tests)
- [X] Benchmark against standard `malloc`

### Phase 2 — HTTP Server
- [ ] Create TCP socket, bind, listen, accept loop
- [ ] Parse raw HTTP GET requests
- [ ] Serve static files (HTML/CSS/JS)
- [ ] Handle basic HTTP responses (200, 404, 500)
- [ ] Stream chunked responses (needed for shell output)
- [ ] Test in browser — "Hello World" page loads

### Phase 3 — Shell Engine
- [ ] Input parser (tokenizer, handle quotes/spaces)
- [ ] Built-in commands (`cd`, `exit`, `help`, `clear`)
- [ ] `fork()` + `exec()` for system commands
- [ ] Pipe support (`ls | grep foo`)
- [ ] Redirection support (`>`, `<`, `>>`)
- [ ] Capture stdout/stderr and stream to HTTP response
- [ ] Connect shell to browser UI (input box → output display)

### Phase 4 — Key-Value Store
- [ ] Hash map implementation (open addressing or chaining)
- [ ] Plug in custom allocator for all KV memory
- [ ] `kv set <key> <value>`
- [ ] `kv get <key>`
- [ ] `kv del <key>`
- [ ] `kv list` — show all entries
- [ ] Disk persistence (serialize to `.kvdb` file on exit)
- [ ] Load from disk on startup

### Phase 5 — File Compressor
- [ ] Byte frequency counter
- [ ] Build Huffman tree (min-heap / priority queue)
- [ ] Generate encoding table (bit codes per byte)
- [ ] Bit-level file writer (`bitio`)
- [ ] `compress <file>` → outputs `<file>.huf`
- [ ] `decompress <file>.huf` → restores original
- [ ] Plug in custom allocator for tree nodes
- [ ] Verify correctness (compress → decompress → diff original)

### Phase 6 — Integration & Frontend
- [ ] Wire all modules into `main.c`
- [ ] Build clean terminal UI in `index.html` (dark theme, monospace)
- [ ] Command history (↑ ↓ arrow keys in browser)
- [ ] Display KV store state in a side panel
- [ ] Show compression stats (original size vs compressed)
- [ ] Memory allocator usage stats display (bytes used/free)

### Phase 7 — Polish & Portfolio Prep
- [ ] Error handling audit across all modules
- [ ] Memory leak check with `valgrind`
- [ ] Write a `Makefile` with clean build
- [ ] Record a demo video
- [ ] Push to GitHub with good commit history

---

## Build

```bash
make
./webshell --port 8080
```

Then open `http://localhost:8080` in your browser.

---

## Concepts Demonstrated

- Memory management (custom allocator, free-list, coalescing)
- OS primitives (`fork`, `exec`, `pipe`, `dup2`)
- TCP sockets and HTTP parsing
- Hash maps and disk persistence
- Huffman encoding and bit-level I/O
- Modular C/C++ project structure