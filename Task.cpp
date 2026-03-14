#include "Task.h"
#include <stdexcept>

// -------------------------------------------------------
// Priority helpers
// -------------------------------------------------------
std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::LOW:    return "Low";
        case Priority::MEDIUM: return "Medium";
        case Priority::HIGH:   return "High";
        default:               return "Unknown";
    }
}

Priority stringToPriority(const std::string& s) {
    if (s == "Low")    return Priority::LOW;
    if (s == "Medium") return Priority::MEDIUM;
    if (s == "High")   return Priority::HIGH;
    throw std::invalid_argument("Invalid priority string: " + s);
}

// -------------------------------------------------------
// Task implementation
// -------------------------------------------------------
Task::Task(int id, const std::string& title, const std::string& description,
           Priority priority, bool completed)
    : id(id), title(title), description(description),
      priority(priority), completed(completed) {}

std::string Task::statusString() const {
    return completed ? "Completed" : "Pending";
}
