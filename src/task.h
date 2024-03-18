// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
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
#include <vector>
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

    std::string getIdent() const;
    void setIdent(const std::string &ident);
    bool addTag(const std::string &tag);
    bool deleteTag(const std::string &tag);
    unsigned int numTags() const;
    bool containsTag(const std::string &tag) const;
    Date getDueDate() const;
    void setDueDate(const Date &dueDate);
    bool isComplete() const;
    void setComplete(bool complete);
    TagContainer getTags() const;
    bool operator==(const Task &other) const;
    std::string str() const;

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

struct AddTagError : public std::runtime_error
{
    explicit AddTagError(const std::string &tagIdent)
        : std::runtime_error("could not add tag with identifier '" + tagIdent + "'")
    {
        /* do nothing */
    }

    ~AddTagError() override = default;
};

struct NoTagError : public std::out_of_range
{
    explicit NoTagError(const std::string &tagIdent)
        : std::out_of_range("unknown tag with identifier '" + tagIdent + "'")
    {
        /* do nothing */
    }

    ~NoTagError() override = default;
};

#endif //
