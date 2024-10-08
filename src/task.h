// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2035827>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// A Task class contains multiple 'tags' (e.g., a tag might be 'home' and
// another tag could be 'uni'). A Task also has a identifier (e.g. 'Complete
// Coursework') a completed status (completed or not) and due date. An
// uninitialised due date (see date.cpp/date.h) means the task does not have a
// due date.
// -----------------------------------------------------

#ifndef TASK_H
#define TASK_H
#include <string>
#include "date.h"

using TagContainer = std::vector<std::string>;
class Task
{
private:
    std::string ident;
    Date dueDate;
    bool completed;
    TagContainer tags;

public:
    Task(std::string ident);
    ~Task();

    unsigned int numTags() const noexcept;

    bool addTag(const std::string &tag);
    bool deleteTag(const std::string &tag);
    bool containsTag(const std::string &tag) const;
    bool isComplete() const;
    bool operator==(const Task &other) const;

    void setIdent(const std::string &ident) noexcept;
    void setDueDate(const Date &dueDate) noexcept;
    void setComplete(bool complete) noexcept;
    void parseJsonTask(const nlohmann::json &jsonData);

    Date getDueDate() const noexcept;

    const TagContainer &getTags() const noexcept;

    std::string str() const;
    std::string getIdent() const noexcept;

    nlohmann::json json() const;

    inline TagContainer::iterator begin() { return tags.begin(); }
    inline TagContainer::const_iterator cbegin() const { return tags.cbegin(); }
    inline TagContainer::iterator end() { return tags.end(); }
    inline TagContainer::const_iterator cend() const { return tags.cend(); }
    inline TagContainer::reverse_iterator rbegin() { return tags.rbegin(); }
    inline TagContainer::const_reverse_iterator crbegin() const { return tags.crbegin(); }
    inline TagContainer::reverse_iterator rend() { return tags.rend(); }
    inline TagContainer::const_reverse_iterator crend() const { return tags.crend(); }
};

struct TagError : public std::out_of_range
{
    explicit TagError(const std::string &message) : std::out_of_range(message) {}
    ~TagError() override = default;
};

#endif //
