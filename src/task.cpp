// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "task.h"

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Task tObj{"Task Name"};
// Task::Task(std::string ident) : ident(ident), completed(false) {}

Task::Task(std::string ident) : ident(ident) {}

Task::~Task() {}

// TODO Write a function, getIdent, that returns the identifier for the Task.
//
// Example:
//  Task tObj{"Task Name"};
//  auto ident = tObj.getIdent();

std::string Task::getIdent() const noexcept
{
    return ident;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Task identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  auto ident = tObj.setIdent("New Task Name");

void Task::setIdent(const std::string &ident) noexcept
{
    this->ident = ident;
}

// TODO Write a function, addTag, that takes one parameters, a tag
//  string and returns true if the entry was inserted into the
//  container or false if the tag already existed.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");

bool Task::addTag(const std::string &tag)
{
    if (!containsTag(tag))
    {
        tags.push_back(tag);
        return true;
    }
    return false;
}

// TODO Write a function, deleteTag, that takes one parameter, a tag
// string, deletes it from the container, and returns true if the tag
// string was deleted. If no tag exists with that name, throw an appropriate
// exception.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");
//  tObj.deleteTag("tag");

bool Task::deleteTag(const std::string &tag)
{
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end())
    {
        tags.erase(it);
        return true;
    }
    else
    {
        throw TagError("Tag not found");
        return false;
    }
}

// TODO Write a function, numTags, that takes no parameters and returns an
// unsigned int of the number of tags in the Task contains.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.numTags();

unsigned int Task::numTags() const noexcept
{
    return tags.size();
}
// TODO Write a function, containsTag, that takes one parameter, a tag string.
// If the tag exists, return true. Otherwise return false.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.addTag("tag");
//  tObj.findTag("tag");

bool Task::containsTag(const std::string &tag) const
{
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

// TODO Write a function, getDueDate, that returns the due date for the Task.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.getDueDate();
Date Task::getDueDate() const
{
    return dueDate;
}
// TODO Write a function, setDueDate, that takes one parameter, a Date for
// a new due date, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  Date d = Date();
//  tObj.setDueDate(d);
void Task::setDueDate(const Date &dueDate)
{
    this->dueDate = dueDate;
}
// TODO Write a function, setComplete, that takes one parameter, a bool for
// a new completed state, and updates the member variable. It returns nothing.
//
// Example:
//  Task tObj{"Task Name"};
//  tObj.setComplete(true);
void Task::setComplete(bool complete)
{
    completed = complete;
}
// TODO Write a function, isComplete, that returns the completed state for the
// Task.
//
// Example:
//  Task tObj{"Task Name"};
//  if(tObj.isComplete()) {
//   ...
//  }

bool Task::isComplete() const
{
    return completed;
}

// TODO Write an == operator overload for the Task class, such that two
// Tasks objects are equal only if they have the same identifier, dueDate,
// completed state, and tags.
//
// Example:
//  Task tObj1{"Task Name"};
//  Task tObj2{"Task Name"};
//  if(tObj1 == tObj2) {
//   ...
//  }
bool Task::operator==(const Task &other) const
{
    return ident == other.ident && dueDate == other.dueDate && completed == other.completed && tags == other.tags;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();
std::string Task::str() const
{
    nlohmann::json taskJson;

    taskJson["identifier"] = ident;
    taskJson["dueDate"] = dueDate.str();
    taskJson["completed"] = completed;

    nlohmann::json tagsJson;
    for (const std::string &tag : tags)
    {
        tagsJson.push_back(tag);
    }
    taskJson["tags"] = tagsJson;

    std::string jsonString = taskJson.dump();

    return jsonString;
}

nlohmann::json Task::json() const
{
    nlohmann::json taskJson;
    taskJson["identifier"] = ident;
    taskJson["dueDate"] = dueDate.str();
    taskJson["completed"] = completed;
    taskJson["tags"] = tags;
    return taskJson;
}