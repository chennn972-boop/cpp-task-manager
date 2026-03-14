#pragma once
#include <string>

// Enum for task priority levels
enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

// Converts Priority enum to display string
std::string priorityToString(Priority p);

// Converts string to Priority enum (used when loading from file)
Priority stringToPriority(const std::string& s);

// ============================================================
// Task: Represents a single task with all its attributes
// ============================================================
class Task {
public:
    int         id;
    std::string title;
    std::string description;
    Priority    priority;
    bool        completed;

    // Constructor
    Task(int id, const std::string& title, const std::string& description,
         Priority priority, bool completed = false);

    // Returns "Completed" or "Pending"
    std::string statusString() const;
};
