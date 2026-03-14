#include "TaskManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// -------------------------------------------------------
// ANSI Color codes for a professional CLI experience
// -------------------------------------------------------
namespace Color {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* WHITE   = "\033[37m";
    constexpr const char* DIM     = "\033[2m";
}

// -------------------------------------------------------
// Constructor
// -------------------------------------------------------
TaskManager::TaskManager() : nextId_(1) {}

// -------------------------------------------------------
// Add a new task
// -------------------------------------------------------
void TaskManager::addTask(const std::string& title,
                          const std::string& description,
                          Priority           priority) {
    tasks_.emplace_back(nextId_++, title, description, priority);
    std::cout << Color::GREEN << "\n  ✔  Task \"" << title
              << "\" added successfully!\n" << Color::RESET;
}

// -------------------------------------------------------
// Mark a task as completed by ID
// -------------------------------------------------------
bool TaskManager::completeTask(int id) {
    int idx = findIndex(id);
    if (idx == -1) return false;

    if (tasks_[idx].completed) {
        std::cout << Color::YELLOW << "\n  ⚠  Task is already marked as completed.\n"
                  << Color::RESET;
        return true;
    }
    tasks_[idx].completed = true;
    std::cout << Color::GREEN << "\n  ✔  Task #" << id
              << " marked as Completed!\n" << Color::RESET;
    return true;
}

// -------------------------------------------------------
// Delete a task by ID
// -------------------------------------------------------
bool TaskManager::deleteTask(int id) {
    int idx = findIndex(id);
    if (idx == -1) return false;

    std::string title = tasks_[idx].title;
    tasks_.erase(tasks_.begin() + idx);
    std::cout << Color::GREEN << "\n  ✔  Task \"" << title
              << "\" deleted successfully!\n" << Color::RESET;
    return true;
}

// -------------------------------------------------------
// Display all tasks in a formatted table
// -------------------------------------------------------
void TaskManager::showTasks() const {
    if (tasks_.empty()) {
        std::cout << Color::YELLOW
                  << "\n  ℹ  No tasks found. Start by adding a task!\n"
                  << Color::RESET;
        return;
    }

    printDivider();
    // Table header
    std::cout << Color::BOLD
              << std::left
              << "  " << std::setw(5)  << "ID"
              << std::setw(28) << "Title"
              << std::setw(12) << "Priority"
              << std::setw(14) << "Status"
              << "\n" << Color::RESET;
    printDivider();

    for (const auto& t : tasks_) {
        // Color-code by priority
        const char* prioColor = Color::WHITE;
        if      (t.priority == Priority::HIGH)   prioColor = Color::RED;
        else if (t.priority == Priority::MEDIUM)  prioColor = Color::YELLOW;
        else                                       prioColor = Color::CYAN;

        // Color-code status
        const char* statusColor = t.completed ? Color::GREEN : Color::MAGENTA;

        std::cout << "  "
                  << Color::BOLD << std::setw(5) << t.id << Color::RESET
                  << std::setw(28) << t.title
                  << prioColor    << std::setw(12) << priorityToString(t.priority) << Color::RESET
                  << statusColor  << std::setw(14) << t.statusString()             << Color::RESET
                  << "\n";

        // Show description if present
        if (!t.description.empty()) {
            std::cout << Color::DIM
                      << "       ↳ " << t.description
                      << Color::RESET << "\n";
        }
    }
    printDivider();
    std::cout << Color::DIM << "  Total: " << tasks_.size() << " task(s)\n"
              << Color::RESET;
}

// -------------------------------------------------------
// Display a single task (reusable)
// -------------------------------------------------------
void TaskManager::showTask(const Task& task) const {
    printDivider();
    std::cout << Color::BOLD
              << std::left
              << "  " << std::setw(5)  << "ID"
              << std::setw(28) << "Title"
              << std::setw(12) << "Priority"
              << std::setw(14) << "Status"
              << "\n" << Color::RESET;
    printDivider();

    const char* prioColor = (task.priority == Priority::HIGH)   ? Color::RED    :
                            (task.priority == Priority::MEDIUM)  ? Color::YELLOW : Color::CYAN;
    const char* statusColor = task.completed ? Color::GREEN : Color::MAGENTA;

    std::cout << "  "
              << Color::BOLD << std::setw(5) << task.id << Color::RESET
              << std::setw(28) << task.title
              << prioColor    << std::setw(12) << priorityToString(task.priority) << Color::RESET
              << statusColor  << std::setw(14) << task.statusString()             << Color::RESET
              << "\n";

    if (!task.description.empty()) {
        std::cout << Color::DIM << "       ↳ " << task.description
                  << Color::RESET << "\n";
    }
    printDivider();
}

// -------------------------------------------------------
// Search tasks by keyword (case-insensitive)
// -------------------------------------------------------
void TaskManager::searchTask(const std::string& keyword) const {
    // Build lowercase copy of keyword for comparison
    std::string kw = keyword;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

    std::vector<const Task*> results;
    for (const auto& t : tasks_) {
        std::string titleLow = t.title;
        std::string descLow  = t.description;
        std::transform(titleLow.begin(), titleLow.end(), titleLow.begin(), ::tolower);
        std::transform(descLow.begin(),  descLow.end(),  descLow.begin(),  ::tolower);

        if (titleLow.find(kw) != std::string::npos ||
            descLow.find(kw)  != std::string::npos) {
            results.push_back(&t);
        }
    }

    if (results.empty()) {
        std::cout << Color::YELLOW
                  << "\n  ℹ  No tasks found matching \"" << keyword << "\"\n"
                  << Color::RESET;
        return;
    }

    std::cout << Color::CYAN << "\n  Found " << results.size()
              << " result(s) for \"" << keyword << "\":\n\n" << Color::RESET;

    printDivider();
    std::cout << Color::BOLD
              << std::left
              << "  " << std::setw(5)  << "ID"
              << std::setw(28) << "Title"
              << std::setw(12) << "Priority"
              << std::setw(14) << "Status"
              << "\n" << Color::RESET;
    printDivider();

    for (const auto* t : results) {
        const char* prioColor = (t->priority == Priority::HIGH)   ? Color::RED    :
                                (t->priority == Priority::MEDIUM)  ? Color::YELLOW : Color::CYAN;
        const char* statusColor = t->completed ? Color::GREEN : Color::MAGENTA;

        std::cout << "  "
                  << Color::BOLD << std::setw(5) << t->id << Color::RESET
                  << std::setw(28) << t->title
                  << prioColor    << std::setw(12) << priorityToString(t->priority) << Color::RESET
                  << statusColor  << std::setw(14) << t->statusString()             << Color::RESET
                  << "\n";

        if (!t->description.empty()) {
            std::cout << Color::DIM << "       ↳ " << t->description
                      << Color::RESET << "\n";
        }
    }
    printDivider();
}

// -------------------------------------------------------
// Sort tasks: High > Medium > Low, then by ID
// -------------------------------------------------------
void TaskManager::sortByPriority() {
    std::sort(tasks_.begin(), tasks_.end(), [](const Task& a, const Task& b) {
        // Higher priority value = higher enum int = comes first
        if (a.priority != b.priority)
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        return a.id < b.id;
    });
    std::cout << Color::GREEN << "\n  ✔  Tasks sorted by priority.\n" << Color::RESET;
}

// -------------------------------------------------------
// Accessors
// -------------------------------------------------------
std::vector<Task>& TaskManager::getTasks()             { return tasks_; }
const std::vector<Task>& TaskManager::getTasks() const { return tasks_; }

void TaskManager::setNextId(int id) { nextId_ = id; }

// -------------------------------------------------------
// Private helpers
// -------------------------------------------------------
void TaskManager::printDivider() const {
    std::cout << Color::DIM
              << "  " << std::string(57, '-')
              << "\n" << Color::RESET;
}

int TaskManager::findIndex(int id) const {
    for (int i = 0; i < static_cast<int>(tasks_.size()); ++i) {
        if (tasks_[i].id == id) return i;
    }
    return -1;
}
