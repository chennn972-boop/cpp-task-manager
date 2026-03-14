#include "TaskManager.h"
#include "FileManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

// -------------------------------------------------------
// ANSI Color codes (duplicated here for UI layer)
// -------------------------------------------------------
namespace Color {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* BLUE    = "\033[34m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* DIM     = "\033[2m";
}

constexpr const char* DATA_FILE = "data/tasks.txt";

// -------------------------------------------------------
// Utility: clear input buffer after bad read
// -------------------------------------------------------
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// -------------------------------------------------------
// Utility: read a non-empty trimmed string from stdin
// -------------------------------------------------------
static std::string readLine(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        // Trim leading/trailing whitespace
        auto start = input.find_first_not_of(" \t");
        auto end   = input.find_last_not_of(" \t");
        if (start != std::string::npos) {
            input = input.substr(start, end - start + 1);
        } else {
            input.clear();
        }
        if (!input.empty()) return input;
        std::cout << Color::YELLOW << "  ⚠  Input cannot be empty. Please try again.\n"
                  << Color::RESET;
    }
}

// -------------------------------------------------------
// Utility: read an optional line (can be empty)
// -------------------------------------------------------
static std::string readOptionalLine(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// -------------------------------------------------------
// Utility: read a valid integer from stdin
// -------------------------------------------------------
static int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        clearInput();
        std::cout << Color::RED << "  ✘  Invalid input. Please enter a number.\n"
                  << Color::RESET;
    }
}

// -------------------------------------------------------
// Utility: ask for yes/no confirmation
// -------------------------------------------------------
static bool confirm(const std::string& message) {
    std::cout << Color::YELLOW << "  " << message << " (y/n): " << Color::RESET;
    std::string response;
    std::getline(std::cin, response);
    if (response.empty() && std::cin.fail()) { clearInput(); return false; }
    return (!response.empty() && (response[0] == 'y' || response[0] == 'Y'));
}

// -------------------------------------------------------
// Prompt user to pick a priority level
// -------------------------------------------------------
static Priority choosePriority() {
    while (true) {
        std::cout << "\n"
                  << Color::CYAN  << "  Priority options:\n" << Color::RESET
                  << "    1. " << Color::CYAN   << "Low\n"    << Color::RESET
                  << "    2. " << Color::YELLOW << "Medium\n" << Color::RESET
                  << "    3. " << Color::RED    << "High\n"   << Color::RESET;
        int choice = readInt("  Select priority [1-3]: ");
        switch (choice) {
            case 1: return Priority::LOW;
            case 2: return Priority::MEDIUM;
            case 3: return Priority::HIGH;
            default:
                std::cout << Color::RED << "  ✘  Invalid choice. Please enter 1, 2, or 3.\n"
                          << Color::RESET;
        }
    }
}

// -------------------------------------------------------
// Print the main menu banner
// -------------------------------------------------------
static void printMenu() {
    std::cout << "\n"
              << Color::BLUE << Color::BOLD
              << "  ╔══════════════════════════════════╗\n"
              << "  ║       SMART TASK MANAGER         ║\n"
              << "  ╚══════════════════════════════════╝\n"
              << Color::RESET
              << Color::DIM << "  ─────────────────────────────────\n" << Color::RESET
              << "  " << Color::BOLD << "1." << Color::RESET << "  Add Task\n"
              << "  " << Color::BOLD << "2." << Color::RESET << "  View All Tasks\n"
              << "  " << Color::BOLD << "3." << Color::RESET << "  Complete Task\n"
              << "  " << Color::BOLD << "4." << Color::RESET << "  Delete Task\n"
              << "  " << Color::BOLD << "5." << Color::RESET << "  Search Task\n"
              << "  " << Color::BOLD << "6." << Color::RESET << "  Sort by Priority\n"
              << "  " << Color::BOLD << "7." << Color::RESET << "  Save Tasks\n"
              << "  " << Color::BOLD << "8." << Color::RESET << "  " << Color::RED << "Exit\n" << Color::RESET
              << Color::DIM << "  ─────────────────────────────────\n" << Color::RESET;
}

// -------------------------------------------------------
// Handler: Add Task
// -------------------------------------------------------
static void handleAddTask(TaskManager& manager) {
    std::cout << "\n" << Color::CYAN << Color::BOLD
              << "  ── Add New Task ──\n" << Color::RESET;

    std::string title = readLine("  Title       : ");
    std::string desc  = readOptionalLine("  Description (optional): ");
    Priority    prio  = choosePriority();

    manager.addTask(title, desc, prio);
}

// -------------------------------------------------------
// Handler: Complete Task
// -------------------------------------------------------
static void handleCompleteTask(TaskManager& manager) {
    if (manager.getTasks().empty()) {
        std::cout << Color::YELLOW << "\n  ℹ  No tasks available.\n" << Color::RESET;
        return;
    }
    manager.showTasks();
    int id = readInt("\n  Enter Task ID to mark as completed: ");

    if (!manager.completeTask(id)) {
        std::cout << Color::RED << "  ✘  Task with ID " << id << " not found.\n"
                  << Color::RESET;
    }
}

// -------------------------------------------------------
// Handler: Delete Task
// -------------------------------------------------------
static void handleDeleteTask(TaskManager& manager) {
    if (manager.getTasks().empty()) {
        std::cout << Color::YELLOW << "\n  ℹ  No tasks available.\n" << Color::RESET;
        return;
    }
    manager.showTasks();
    int id = readInt("\n  Enter Task ID to delete: ");

    // Find and show the task before confirming
    bool found = false;
    for (const auto& t : manager.getTasks()) {
        if (t.id == id) {
            std::cout << "\n  Task to delete:\n";
            manager.showTask(t);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << Color::RED << "  ✘  Task with ID " << id << " not found.\n"
                  << Color::RESET;
        return;
    }

    if (confirm("  Are you sure you want to delete this task?")) {
        manager.deleteTask(id);
    } else {
        std::cout << Color::YELLOW << "  ℹ  Delete cancelled.\n" << Color::RESET;
    }
}

// -------------------------------------------------------
// Handler: Search Task
// -------------------------------------------------------
static void handleSearchTask(const TaskManager& manager) {
    std::string keyword = readLine("\n  Enter keyword to search: ");
    manager.searchTask(keyword);
}

// -------------------------------------------------------
// Handler: Save Tasks
// -------------------------------------------------------
static void handleSaveTasks(const TaskManager& manager, const FileManager& fileManager) {
    if (fileManager.saveTasks(manager)) {
        std::cout << Color::GREEN << "\n  ✔  Tasks saved to \"" << DATA_FILE << "\" successfully!\n"
                  << Color::RESET;
    } else {
        std::cout << Color::RED << "\n  ✘  Failed to save tasks. Check that the 'data/' directory exists.\n"
                  << Color::RESET;
    }
}

// -------------------------------------------------------
// Main entry point
// -------------------------------------------------------
int main() {
    TaskManager manager;
    FileManager fileManager(DATA_FILE);

    // Auto-load tasks on startup
    int loaded = fileManager.loadTasks(manager);
    if (loaded > 0) {
        std::cout << Color::GREEN << "\n  ✔  Loaded " << loaded
                  << " task(s) from \"" << DATA_FILE << "\"\n" << Color::RESET;
    }

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("  Choose an option [1-8]: ");

        switch (choice) {
            case 1: handleAddTask(manager);                          break;
            case 2: manager.showTasks();                             break;
            case 3: handleCompleteTask(manager);                     break;
            case 4: handleDeleteTask(manager);                       break;
            case 5: handleSearchTask(manager);                       break;
            case 6: manager.sortByPriority();                        break;
            case 7: handleSaveTasks(manager, fileManager);           break;
            case 8:
                if (confirm("Save before exiting?")) {
                    handleSaveTasks(manager, fileManager);
                }
                std::cout << Color::BLUE << "\n  Goodbye! 👋\n\n" << Color::RESET;
                running = false;
                break;
            default:
                std::cout << Color::RED
                          << "  ✘  Invalid option. Please choose 1–8.\n"
                          << Color::RESET;
        }
    }

    return 0;
}
