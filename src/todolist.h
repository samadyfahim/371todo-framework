// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// The root object that holds all data for 371todo. This
// class contains Projects, which in turn hold Tasks,
// which have a name, due date, completed status, and tags.
// -----------------------------------------------------

#ifndef TODOLIST_H
#define TODOLIST_H

#include "project.h"
#include "task.h"
#include <vector>
#include <fstream>
#include <iomanip>
using ProjectContainer = std::vector<Project>;

class TodoList
{
private:
    ProjectContainer projects;

    Project &findExistingProject(const std::string &projectIdent);
    void mergeProjectTasks(Project &existingProject, const Project &newProject);
    void insertNewProject(const Project &project) noexcept;

public:
    TodoList();
    ~TodoList();
    unsigned int size() const noexcept;
    Project &newProject(const std::string &projectIdent);
    bool addProject(const Project &project);
    Project &getProject(const std::string &projectIdent);
    bool deleteProject(const std::string &projectIdent);
    void load(const std::string &filename);
    void save(const std::string &filename);
    bool operator==(const TodoList &other) const;
    std::string str() const;
    void parseToDoList(const nlohmann::json &jsonData);

    nlohmann::json json() const;

    inline ProjectContainer::iterator begin() { return projects.begin(); }
    inline ProjectContainer::const_iterator cbegin() const { return projects.cbegin(); }

    inline ProjectContainer::iterator end() { return projects.end(); }
    inline ProjectContainer::const_iterator cend() const { return projects.cend(); }

    inline ProjectContainer::reverse_iterator rbegin() { return projects.rbegin(); }
    inline ProjectContainer::const_reverse_iterator crbegin() const
    {
        return projects.crbegin();
    }

    inline ProjectContainer::reverse_iterator rend() { return projects.rend(); }
    inline ProjectContainer::const_reverse_iterator crend() const
    {
        return projects.crend();
    }
};

struct AddProjectError : public std::runtime_error
{
    explicit AddProjectError(const std::string &projectIdent)
        : std::runtime_error("could not add project with identifier '" + projectIdent + "'")
    {
        /* do nothing */
    }

    ~AddProjectError() override = default;
};

struct NoProjectError : public std::out_of_range
{
    explicit NoProjectError(const std::string &projectIdent)
        : std::out_of_range("unknown project with identifier '" + projectIdent + "'")
    {
        /* do nothing */
    }

    ~NoProjectError() override = default;
};

struct FailedOpenFile : public std::runtime_error
{
    explicit FailedOpenFile(const std::string &filename)
        : std::runtime_error("Failed to open file" + filename)
    {
        /* do nothing */
    }
    ~FailedOpenFile() override = default;
};

struct JsonParsingError : public std::runtime_error
{
    explicit JsonParsingError(const std::string &message)
        : std::runtime_error("Error parsing JSON file: " + message)
    {
    }
};

#endif // TODOLIST_H
