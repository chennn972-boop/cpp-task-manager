#pragma once
#include "Task.h"
#include <vector>
#include <string>

// ============================================================
// TaskManager: Core business logic for managing tasks
// ============================================================
class TaskManager {
public:
    TaskManager();

    // CRUD operations
    void addTask(const std::string& title, const std::string& description, Priority priority);
    bool completeTask(int id);
    bool deleteTask(int id);

    // Display
    void showTasks() const;
    void showTask(const Task& task) const;

    // Search
    void searchTask(const std::string& keyword) const;

    // Sorting (by priority: High > Medium > Low)
    void sortByPriority();

    // Access to raw task list (used by FileManager)
    std::vector<Task>& getTasks();
    const std::vector<Task>& getTasks() const;

    // Used when loading from file to sync ID counter
    void setNextId(int id);

private:
    std::vector<Task> tasks_;
    int               nextId_;

    // Print the table header
    void printHeader() const;
    // Print the table footer/divider
    void printDivider() const;
    // Find task index by ID, returns -1 if not found
    int findIndex(int id) const;
};
