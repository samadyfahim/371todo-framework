// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2035827>
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

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of projects the TodoList contains.
//
// Example:
//  TodoList tObj{};
//  auto size = tObj.size();

unsigned int TodoList::size() const noexcept
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
        return getProject(projectIdent);
    }
    catch (const NoProjectError &)
    {
        Project newProject(projectIdent);
        projects.push_back(std::move(newProject));
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
        throw NoProjectError(projectIdent);
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
    try
    {
        Project &existingProject = findExistingProject(project.getIdent());
        mergeProjectTasks(existingProject, project);
        return false;
    }
    catch (const NoProjectError &)
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

void TodoList::insertNewProject(const Project &project) noexcept
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
    for (Project &project : projects)
    {
        if (project.getIdent() == projectIdent)
        {
            return project;
        }
    }
    throw NoProjectError(projectIdent);
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
    auto it = std::find_if(projects.begin(), projects.end(), [&projectIdent](const Project &proj)
                           { return proj.getIdent() == projectIdent; });

    if (it != projects.end())
    {
        projects.erase(it);
        return true;
    }

    throw NoProjectError(projectIdent);
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
    try
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        nlohmann::json jsonData;
        file >> jsonData;

        parseToDoList(jsonData);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error loading file: " + std::string(e.what()));
    }
}

// TODO Write a function, save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the TodoList object
//  as JSON.
//
// Example:
//  TodoList tObj{};
//  tObj.load("database.json");
//  ...
//  tObj.save("database.json");

void TodoList::save(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FailedOpenFile(filename);
    }

    std::string jsonDataStr = str();
    file << jsonDataStr;
    file.close();
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
    return json().dump();
}

nlohmann::json TodoList::json() const
{
    nlohmann::json jsonData;
    for (const auto &project : projects)
    {
        nlohmann::json projectData;
        for (const auto &task : project.getTasks())
        {
            nlohmann::json taskData;
            taskData["completed"] = task.isComplete();
            taskData["dueDate"] = task.getDueDate().str();
            for (const std::string &tag : task.getTags())
            {
                taskData["tags"].push_back(tag);
            }
            projectData[task.getIdent()] = taskData;
        }
        jsonData[project.getIdent()] = projectData;
    }

    return jsonData;
}

void TodoList::parseToDoList(const nlohmann::json &jsonData)
{
    try
    {
        for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            const std::string &projectName = it.key();
            const nlohmann::json &projectData = it.value();
            Project &project = newProject(projectName);
            project.parseJsonProject(projectData);
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error parsing TodoList data: " + std::string(e.what()));
    }
}
