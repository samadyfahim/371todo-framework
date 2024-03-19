// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "project.h"

// TODO Write a constructor that takes one parameter, a string identifier and
// initialises the object and member data.
//
// Example:
//  Project p{"projectIdent"};

Project::Project(std::string ident) : ident(std::move(ident))
{
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
// int of the number of Tasks in the Project contains.
//
// Example:
//  Project p{"projectIdent"};
//  auto size = p.size();

unsigned int Project::size() const noexcept
{
    return tasks.size();
}

// TODO Write a function, getIdent, that returns the identifier for the Project.
//
// Example:
//  Project pObj{"projectIdent"};
//  auto ident = pObj.getIdent();

const std::string &Project::getIdent() const noexcept
{
    return ident;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
// Project identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.setIdent("projectIdent2");

void Project::setIdent(std::string pIdent) noexcept
{
    ident = std::move(pIdent);
}

// TODO Write a function, newTask, that takes one parameter, a Task identifier,
// (a string) and returns the Task object as a reference. If an object with the
// same identifier already exists, then the existing object should be returned.
// Throw a std::runtime_error if the Task object cannot be inserted into the
// container for whatever reason.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");

Task &Project::newTask(const std::string &tIdent)
{
    auto it = findTask(tIdent);
    if (it != tasks.end())
    {
        return *it;
    }
    else
    {
        tasks.emplace_back(tIdent);
        return tasks.back();
    }
}

// TODO Write a function, addTask, that takes one parameter, a Task object, and
// returns true if the object was successfully inserted. If an object with the
// same identifier already exists, then:
//  - the tags should be merged
//  - completed status overwritten by the task being added
//  - dueDate overwritten by the task being added
//  - false should be returned.
//
// Example:
//  Project pObj{"projectIdent"};
//  Task tObj{"taskIdent"};
//  pObj.addItem(tObj);

bool Project::addTask(Task task)
{
    auto it = findTask(task.getIdent());
    if (it != tasks.end())
    {
        for (const auto &tag : task.getTags())
        {
            it->addTag(tag);
        }

        it->setComplete(task.isComplete());
        it->setDueDate(task.getDueDate());
        return false;
    }
    else
    {
        tasks.push_back(std::move(task));
        return true;
    }
}

// TODO Write a function, getTask, that takes one parameter, a Task identifier
// (a string) and returns the Task as a reference. If no Task exists, throw an
// appropriate exception.
//
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");
//  auto tObj = pObj.getTask("newTaskName");

Task &Project::getTask(const std::string &tIdent)
{
    auto it = findTask(tIdent);
    if (it != tasks.end())
    {
        return *it;
    }
    else
    {
        throw NoTaskError(tIdent);
    }
}

// TODO Write a function, deleteTask, that takes one parameter, a Task
// identifier (a string), deletes it from the container, and returns true if the
// Task was deleted. If no Task exists, throw an appropriate exception.
//
// Example:
//  Project pObj{"projectIdent"};
//  pObj.newTask("newTaskName");
//  bool result = pObj.deleteTask("newTaskName");

bool Project::deleteTask(const std::string &tIdent)
{
    auto it = findTask(tIdent);
    if (it != tasks.end())
    {
        tasks.erase(it);
        return true;
    }
    else
    {
        throw std::out_of_range("Task not found");
        return false;
    }
}

// TODO Write an == operator overload for the Project class, such that two
// Project objects are equal only if they have the same identifier and same
// Tasks.
//
// Example:
//  Project pObj{"projectIdent1"};
//  pObj.newTask("newTaskName");
//  Project pObj1{"projectIdent2"};
//  if(pObj1 == pObj2) {
//    ...
//  }

bool operator==(const Project &c1, const Project &c2)
{
    return c1.ident == c2.ident && c1.tasks == c2.tasks;
}

nlohmann::json Project::json() const
{
    nlohmann::json projectJson;
    projectJson["identifier"] = ident;

    for (const auto &task : tasks)
    {
        projectJson["tasks"].emplace_back(task.json());
    }

    return projectJson;
}

// TODO Write a function, str, that takes no parameters and returns a
// std::string of the JSON representation of the data in the Project.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Project pObj{"projectIdent"};
//  std::string s = pObj.str();

std::string Project::str() const
{
    return json().dump();
}

TaskContainer::iterator Project::findTask(const std::string &tIdent)
{
    return std::find_if(tasks.begin(), tasks.end(),
                        [&tIdent](const Task &task)
                        { return task.getIdent() == tIdent; });
}

// bool Project::containsTask(const std::string &tIdent) const noexcept
// {
//     return std::any_of(tasks.begin(), tasks.end(),
//                        [&tIdent](const Task &task)
//                        { return task.getIdent() == tIdent; });
// }

const TaskContainer &Project::getTasks() const noexcept
{
    return tasks;
}

void Project::parseJsonProject(const nlohmann::json &jsonData)
{
    try
    {
        for (auto it = jsonData.begin(); it != jsonData.end(); ++it)

        {
            const std::string &taskName = it.key();
            const nlohmann::json &taskData = it.value();
            Task task(taskName);
            task.parseJsonTask(taskData);
            tasks.emplace_back(std::move(task));
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error parsing project data: " + std::string(e.what()));
    }
}