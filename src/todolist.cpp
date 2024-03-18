// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
#include "todolist.h"
#include "project.h"
#include "date.h"

// TODO Write a TodoList constructor that takes no parameters and constructs an
//  empty todolist.
//
// Example:
//  TodoList tObj{};

TodoList::TodoList()
{
}

TodoList::~TodoList()
{
}

TagContainer Task::getTags() const
{
    return tags;
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of projects the TodoList contains.
//
// Example:
//  TodoList tObj{};
//  auto size = tObj.size();

unsigned int TodoList::size() const
{
    return projects.size();
}

// TODO Write a function, newProject, that takes one parameter, a project
//  identifier, and returns the Project object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Project object cannot be
//  inserted into the container.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");

Project &TodoList::newProject(const std::string &projectIdent)
{
    try
    {
        return findExistingProject(projectIdent);
    }
    catch (const std::runtime_error &)
    {
        projects.emplace_back(projectIdent);
        return projects.back();
    }
}

Project &TodoList::findExistingProject(const std::string &projectIdent)
{
    auto projectIterator = std::find_if(projects.begin(), projects.end(), [&projectIdent](const Project &p)
                                        { return p.getIdent() == projectIdent; });

    if (projectIterator != projects.end())
    {
        return *projectIterator;
    }
    else
    {
        throw std::runtime_error("the project with identifier '" + projectIdent + "' was not found");
    }
}

// emplace construct element in place
// try_emplce insert inplace if the key dosenot exist dose nothing otherwise

// TODO Write a function, addProject, that takes one parameter, a Project
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged (see also Project::addTask) and then return false. Throw a
//  std::runtime_error if the Project object cannot be inserted into the
//  container for whatever reason.
//
// Example:
//  TodoList tObj{};
//  Project cObj{"projectIdent"};
//  tObj.addProject(cObj);

bool TodoList::addProject(const Project &project)
{
    auto projectIterator = std::find_if(projects.begin(), projects.end(), [&project](const Project &p)
                                        { return p.getIdent() == project.getIdent(); });

    if (projectIterator != projects.end())
    {
        mergeProjectTasks(*projectIterator, project);
        return false;
    }
    else
    {
        insertNewProject(project);
        return true;
    }
}

void TodoList::mergeProjectTasks(Project &existingProject, const Project &newProject)
{
    for (const auto &task : newProject.getTasks())
    {
        existingProject.addTask(task);
    }
}

void TodoList::insertNewProject(const Project &project)
{
    projects.push_back(project);
}

// TODO Write a function, getProject, that takes one parameter, a Project
//  identifier and returns the Project. If no Project exists, throw an
//  appropriate exception.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");
//  auto cObj = tObj.getProject("projectIdent");

Project &TodoList::getProject(const std::string &projectIdent)
{
    auto projectIterator = std::find_if(projects.begin(), projects.end(), [&projectIdent](const Project &p)
                                        { return p.getIdent() == projectIdent; });

    if (projectIterator != projects.end())
    {
        return *projectIterator;
    }
    else
    {
        throw std::runtime_error("project not found");
    }
}

// TODO Write a function, deleteProject, that takes one parameter, a Project
//  identifier, and deletes it from the container, and returns true if the
//  Project was deleted. If no Project exists, throw an appropriate exception.
//
// Example:
//  TodoList tObj{};
//  tObj.newProject("projectIdent");
//  tObj.deleteProject("projectIdent");

bool TodoList::deleteProject(const std::string &projectIdent)
{
    auto projectIterator = std::find_if(projects.begin(), projects.end(), [&projectIdent](const Project &p)
                                        { return p.getIdent() == projectIdent; });

    if (projectIterator != projects.end())
    {
        projects.erase(projectIterator);
        return true;
    }
    else
    {
        throw std::runtime_error("project not found");
        return false;
    }
}

// TODO Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this TodoList. If the file does open throw
//  an appropriate exception (either std::runtime_error or a derived class).
//
// A note on clashes:
//  If you encounter two projects with the same key, or two tasks with the same
//  key within the same project, you may choose whichever project/task is parsed
//  by the nlohmann json library. The json spec on duplicate keys is undefined, so
//  you can default to the behaviour of the nlohmann json library.
//
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
// {
//     "Project 1" : {
//         "Task 1":  {
//             "completed": true,
//             "due": "2024-11-23",
//             "tags" : ["uni",
//                       "c++",
//                       "programming",
//                       "standard library"
//                       ...
//                      ]
//             ...
//         },
//         "Task 2":  {
//             "completed": false,
//             "tags" : ["uni",
//                       "c++",
//                       "programming",
//                       "inheritance"
//                       ...
//                      ]
//         },
//         ...
//     },
//     "Project 2": {
//         "Task 1": {
//             "completed": true
//         }
//         ...
//     }
// }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Project and Task objects according
//  to the JSON data in the file.
//
// Example:
//  TodoList tObj{};
//  tObj.load("database.json");

void TodoList::load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
    {
        std::string projectName = it.key();
        Project &project = newProject(projectName);

        for (auto &taskData : it.value().items())
        {
            std::string taskName = taskData.key();
            bool completed = taskData.value()["completed"];
            std::string dueDateString = taskData.value()["due"];

            Task task(taskName);
            task.setComplete(completed);

            if (!dueDateString.empty())
            {
                Date dueDate;
                dueDate.setDateFromString(dueDateString);
                task.setDueDate(dueDate);
            }

            for (const auto &tag : taskData.value()["tags"])
            {
                task.addTag(tag);
            }

            project.addTask(task);
        }
    }
}

// TODO Write an == operator overload for the TodoList class, such that two
//  TodoList objects are equal only if they have the exact same data.
//
// Example:
//  TodoList tObj1{};
//  TodoList tObj2{};
//  if(tObj1 == tObj2) {
//    ...
//  }
bool TodoList::operator==(const TodoList &other) const
{
    return projects == other.projects;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the TodoList.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  TodoList tObj{};
//  std::string s = tObj.str();

std::string TodoList::str() const
{
    nlohmann::json jsonData;

    for (const auto &project : projects)
    {
        nlohmann::json projectData;

        for (const auto &task : project.getTasks())
        {
            nlohmann::json taskData;
            taskData["completed"] = task.isComplete();
            taskData["due"] = task.getDueDate().str();
            taskData["tags"] = task.getTags();

            projectData[task.getIdent()] = taskData;
        }

        jsonData[project.getIdent()] = projectData;
    }

    return jsonData.dump(4);
}
