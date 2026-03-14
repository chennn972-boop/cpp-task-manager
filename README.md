#  Smart Task Manager

A professional **Command Line Interface (CLI) Task Manager** built with modern **C++17**.  
Clean architecture, colored output, file persistence, and an interactive menu — all in a single lightweight binary.

---

##  Features

| Feature | Description |
|---|---|
|  Add Task | Create tasks with title, optional description, and priority |
|  View Tasks | Display all tasks in a formatted, color-coded table |
|  Complete Task | Mark any task as completed by its ID |
|  Delete Task | Remove a task with a confirmation prompt |
|  Search Task | Case-insensitive search by title or description |
|  Sort by Priority | Reorder tasks: High → Medium → Low |
|  Save Tasks | Persist tasks to `data/tasks.txt` |

### Bonus
-  **Colored CLI output** using ANSI escape codes
-  **Confirmation prompt** before deleting
-  **Descriptions** shown inline below each task

---

##  Project Structure

```
smart-task-manager/
│
├── main.cpp          # Entry point — CLI loop and user interaction
├── Task.h / .cpp     # Task entity: id, title, description, priority, status
├── TaskManager.h/.cpp# Business logic: CRUD, search, sort, display
├── FileManager.h/.cpp# File persistence: save and load tasks
├── Makefile          # Build automation
│
└── data/
    └── tasks.txt     # Auto-generated task storage file
```

---

##  Architecture & Design

```
┌─────────────┐     uses     ┌─────────────────┐     uses     ┌──────────────┐
│   main.cpp  │ ──────────▶  │  TaskManager    │ ──────────▶  │     Task     │
│  (CLI / UI) │              │  (Business Logic)│              │   (Entity)   │
└─────────────┘              └─────────────────┘              └──────────────┘
       │                              ▲
       │           uses               │
       ▼                              │
┌─────────────┐                       │
│ FileManager │ ──────────────────────┘
│ (Persistence)│
└─────────────┘
```

### Classes

**`Task`**  
Plain data class representing a single task.
```cpp
int id;            // Unique auto-incremented identifier
string title;      // Short task name
string description;// Optional details
Priority priority; // LOW | MEDIUM | HIGH (enum class)
bool completed;    // false = Pending, true = Completed
```

**`TaskManager`**  
Core business logic. Owns the `vector<Task>` container and provides all CRUD and display operations.

**`FileManager`**  
Handles reading and writing `data/tasks.txt`. Each line is a pipe-delimited record:
```
id|title|description|priority|completed
```

---

##  Getting Started

### Prerequisites

- **g++** with C++17 support (GCC 7+ or Clang 5+)
- **make** (optional — you can also compile manually)

### Compile with Make

```bash
git https://github.com/chennn972-boop/cpp-task-manager
cd smart-task-manager
make
```

### Compile Manually

```bash
g++ -std=c++17 -Wall -O2 -o task-manager \
    main.cpp Task.cpp TaskManager.cpp FileManager.cpp
```

### Run

```bash
./smart-task-manager
```

> **Note:** Make sure the `data/` directory exists before running:
> ```bash
> mkdir -p data
> ```

---

## 🖥️ Usage

```
  ╔══════════════════════════════════╗
  ║       SMART TASK MANAGER         ║
  ╚══════════════════════════════════╝
  ─────────────────────────────────
  1.  Add Task
  2.  View All Tasks
  3.  Complete Task
  4.  Delete Task
  5.  Search Task
  6.  Sort by Priority
  7.  Save Tasks
  8.  Exit
  ─────────────────────────────────
  Choose an option [1-8]:
```

### Example: Adding a Task

```
  ── Add New Task ──
  Title       : Learn CMake
  Description (optional): Build system for large C++ projects

  Priority options:
    1. Low
    2. Medium
    3. High
  Select priority [1-3]: 2

  ✔  Task "Learn CMake" added successfully!
```

### Example: View All Tasks

```
  ---------------------------------------------------------
  ID   Title                       Priority    Status
  ---------------------------------------------------------
  1    Study C++                   High        Pending
       ↳ Read chapters 10-15 on templates
  2    Build GitHub Portfolio      High        Completed
       ↳ Create 3 portfolio projects
  3    Learn Docker                Medium      Pending
  ---------------------------------------------------------
  Total: 3 task(s)
```

### Example: Search

```
  Enter keyword to search: docker

  Found 1 result(s) for "docker":
  ---------------------------------------------------------
  ID   Title                       Priority    Status
  ---------------------------------------------------------
  3    Learn Docker                Medium      Pending
       ↳ Complete Docker tutorial
  ---------------------------------------------------------
```

---

## 🛠️ Build Targets

```bash
make          # Build the binary
make clean    # Remove object files and binary
make rebuild  # Clean then build
```

---

## 💾 Data Format

Tasks are stored in `data/tasks.txt` as pipe-delimited text:

```
1|Study C++|Read chapters 10-15 on templates|High|0
2|Build GitHub Portfolio|Create 3 portfolio projects|High|1
3|Learn Docker|Complete Docker tutorial|Medium|0
```

Fields: `id | title | description | priority | completed (0/1)`

---

## 📦 C++ Concepts Demonstrated

- **OOP** — Classes with clear separation of concerns
- **STL Containers** — `std::vector<Task>` for task storage
- **Enums** — `enum class Priority` for type-safe priority values
- **File I/O** — `std::ifstream` / `std::ofstream` for persistence
- **Algorithm** — `std::sort`, `std::transform` for sorting and search
- **Error Handling** — Try/catch on malformed file data
- **RAII** — File streams closed automatically via destructors
- **C++17** — `constexpr`, structured bindings, and modern idioms

---


