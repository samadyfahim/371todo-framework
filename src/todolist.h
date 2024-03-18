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
    void insertNewProject(const Project &project);

public:
    TodoList();
    ~TodoList();
    unsigned int size() const;
    Project &newProject(const std::string &projectIdent);
    bool addProject(const Project &project);
    Project &getProject(const std::string &projectIdent);
    bool deleteProject(const std::string &projectIdent);
    void load(const std::string &filename);
    void save(const std::string &filename);
    bool operator==(const TodoList &other) const;
    std::string str() const;
};

#endif // TODOLIST_H
