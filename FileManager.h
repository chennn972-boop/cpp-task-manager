#pragma once
#include "TaskManager.h"
#include <string>

// ============================================================
// FileManager: Handles persistence — saving and loading tasks
// ============================================================
class FileManager {
public:
    explicit FileManager(const std::string& filepath);

    // Save all tasks to file; returns true on success
    bool saveTasks(const TaskManager& manager) const;

    // Load tasks from file into manager; returns number of tasks loaded
    int loadTasks(TaskManager& manager) const;

private:
    std::string filepath_;

    // Delimiter used between fields in the data file
    static constexpr char DELIM = '|';
};
