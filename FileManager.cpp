#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// -------------------------------------------------------
// Constructor
// -------------------------------------------------------
FileManager::FileManager(const std::string& filepath)
    : filepath_(filepath) {}

// -------------------------------------------------------
// Save all tasks to the data file
// Format per line:  id|title|description|priority|completed
// -------------------------------------------------------
bool FileManager::saveTasks(const TaskManager& manager) const {
    std::ofstream file(filepath_);
    if (!file.is_open()) {
        std::cerr << "\n  [ERROR] Cannot open file for writing: " << filepath_ << "\n";
        return false;
    }

    for (const auto& t : manager.getTasks()) {
        file << t.id          << DELIM
             << t.title       << DELIM
             << t.description << DELIM
             << priorityToString(t.priority) << DELIM
             << (t.completed ? 1 : 0)
             << "\n";
    }

    file.close();
    return true;
}

// -------------------------------------------------------
// Load tasks from the data file into the manager
// Returns the number of tasks successfully loaded
// -------------------------------------------------------
int FileManager::loadTasks(TaskManager& manager) const {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        // File not found is a normal "first run" condition — not an error
        return 0;
    }

    int     count  = 0;
    int     maxId  = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> fields;

        while (std::getline(ss, token, DELIM)) {
            fields.push_back(token);
        }

        // Expect exactly 5 fields
        if (fields.size() != 5) continue;

        try {
            int         id          = std::stoi(fields[0]);
            std::string title       = fields[1];
            std::string description = fields[2];
            Priority    priority    = stringToPriority(fields[3]);
            bool        completed   = (fields[4] == "1");

            manager.getTasks().emplace_back(id, title, description, priority, completed);
            if (id > maxId) maxId = id;
            ++count;
        } catch (const std::exception& e) {
            std::cerr << "  [WARN] Skipping malformed line: " << line
                      << " (" << e.what() << ")\n";
        }
    }

    // Sync the ID counter so new tasks get unique IDs
    if (maxId > 0) {
        manager.setNextId(maxId + 1);
    }

    file.close();
    return count;
}
