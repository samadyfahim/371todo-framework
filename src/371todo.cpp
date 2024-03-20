// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2035827>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include "371todo.h"
#include "lib_cxxopts.hpp"

// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a TodoList object
//  2. Parse the 'action' argument to decide what action should be taken
//     (json, create, delete, update).  'json' is the easiest to implement, and
//     'update' is the hardest. The details of how these arguments work is in
//     the coursework specification.
//  3. Save the updated TodoList object to the JSON file if there have been
//     changes (calling save() on the TodoList object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//  int main(int argc, char *argv[]) { return App::run(argc, argv); }

int App::run(int argc, char *argv[])
{
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);

  if (args.count("help"))
  {
    std::cout << options.help() << '\n';
    return 0;
  }

  // Open the database and construct the TodoList
  const std::string db = args["db"].as<std::string>();
  TodoList tlObj{};

  // Only uncomment this once you have implemented the load function!
  tlObj.load(db);

  const Action a = parseActionArgument(args);
  switch (a)
  {
  case Action::CREATE:
    handleCreateAction(args, tlObj);
    break;

  case Action::JSON:
    handleJsonAction(args, tlObj);
    break;

  case Action::UPDATE:
    handleUpdateAction(args, tlObj);
    break;

  case Action::DELETE:
    handleDeleteAction(args, tlObj);
    break;

  default:
    throw std::runtime_error("unknown action not implemented");
  }

  tlObj.save(db);

  return 0;
}

void App::handleJsonAction(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string json = getJSON(tlObj);
  std::cout << json << std::endl;
}

void App::handleCreateAction(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  try
  {
    if (args.count("project"))
    {
      createProject(args, tlObj);
    }
    if (args.count("task"))
    {
      createTask(args, tlObj);
    }
    if (args.count("tag"))
    {
      createTag(args, tlObj);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
void App::createProject(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  tlObj.newProject(projectIdent);
}

void App::createTask(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  project.newTask(taskIdent);
}

void App::createTag(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  std::string tagIdent = args["tag"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  Task &task = project.getTask(taskIdent);
  task.addTag(tagIdent);
}

void App::handleDeleteAction(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  try
  {
    if (args.count("tag"))
    {
      handleDeleteTag(args, tlObj);
    }
    else if (args.count("task"))
    {
      handleDeleteTask(args, tlObj);
    }
    else if (args.count("project"))
    {
      handleDeleteProject(args, tlObj);
    }
    else
    {
      throw std::invalid_argument("Missing required arguments for delete action");
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error!!!!: " << e.what() << std::endl;
  }
}

void App::handleDeleteProject(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  tlObj.deleteProject(projectIdent);
}

void App::handleDeleteTask(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  project.deleteTask(taskIdent);
}

void App::handleDeleteTag(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  std::string tagIdent = args["tag"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  Task &task = project.getTask(taskIdent);
  task.deleteTag(tagIdent);
}

void App::handleUpdateAction(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  try
  {
    if (args.count("project"))
    {
      updateProject(args, tlObj);
    }
    if (args.count("task"))
    {
      updateTask(args, tlObj);
    }
    if (args.count("tag"))
    {
      updateTag(args, tlObj);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void App::updateProject(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);

  if (args.count("project-name"))
  {
    std::string newName = args["project-name"].as<std::string>();
    project.setIdent(newName);
  }
}

void App::updateTask(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  Task &task = project.getTask(taskIdent);

  if (args.count("task-name"))
  {
    std::string newName = args["task-name"].as<std::string>();
    task.setIdent(newName);
  }
  if (args.count("completed"))
  {
    bool completed = args["completed"].as<bool>();
    task.setComplete(completed);
  }
}

void App::updateTag(const cxxopts::ParseResult &args, TodoList &tlObj)
{
  std::string projectIdent = args["project"].as<std::string>();
  std::string taskIdent = args["task"].as<std::string>();
  std::string tagIdent = args["tag"].as<std::string>();
  Project &project = tlObj.getProject(projectIdent);
  Task &task = project.getTask(taskIdent);

  if (task.containsTag(tagIdent))
  {

    task.deleteTag(tagIdent);
  }
}

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);

cxxopts::Options App::cxxoptsSetup()
{
  cxxopts::Options cxxopts("371todo", "Student ID: " + STUDENT_NUMBER + "\n");

  cxxopts.add_options()(
      "db", "Filename of the 371todo database",
      cxxopts::value<std::string>()->default_value("database.json"))(

      "action", "Action to take, can be: 'create', 'json', 'update', 'delete'.",
      cxxopts::value<std::string>())(

      "project",
      "Apply action (create, json, update, delete) to a project. If you want to "
      "add a project, set the action argument to 'create' and the project "
      "argument to your chosen project identifier.",
      cxxopts::value<std::string>())(

      "task",
      "Apply action (create, json, update, detele) to a task. If you want to add "
      "a task, set the action argument to 'create', the project argument to your "
      "chosen project identifier and the task argument to the task identifier).",

      cxxopts::value<std::string>())(

      "tag",
      "Apply action (create, json, delete) to a tag.  If you want to add an tag, "
      "set the action argument to 'create', the project argument to your chosen "
      "project identifier, the task argument to your chosen task identifier, and "
      "the tag argument to a single tag 'tag' or comma seperated list of tags: "
      "'tag1,tag2').  If you are simply retrieving a tag through the json action "
      "(and checking that it exists), set the tag argument to the tag name "
      "(e.g. 'example tag'). The action update is unsupported here.",
      cxxopts::value<std::string>())(

      "completed",
      "When creating or updating a task, set the completed flag to change the "
      "task's state to completed. This flag is not compatible in combination "
      "with the incomplete flag.",
      cxxopts::value<bool>())(

      "incomplete",
      "When creating or updating a task, set the incomplete flag to change the "
      "task's state to incomplete. This flag is not compatible in combination "
      "with the completed flag",
      cxxopts::value<bool>())(

      "due",
      "When creating or updating a task, set the due date flag to change the "
      "task's due date to the one specified as an argument (e.g. '2024-11-23')."
      "Ommitting the argument removes the due date from the task.",
      cxxopts::value<std::string>())(

      "h,help", "Print usage.");

  return cxxopts;
}

// TODO Rewrite this function so that it works. This function should
// case-insensitively check the action argument retrieved from cxxopts and
// convert this to a value from the ACTION enum. If an invalid value is given
// in a string, throw an std::invalid_argument exception.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args)
{
  //  enum Action { CREATE, JSON, DELETE, UPDATE };
  std::string input = args["action"].as<std::string>();

  std::transform(input.begin(), input.end(), input.begin(), ::tolower);

  if (input == "create")
  {
    return Action::CREATE;
  }
  else if (input == "json")
  {
    return Action::JSON;
  }
  else if (input == "delete")
  {
    return Action::DELETE;
  }
  else if (input == "update")
  {
    return Action::UPDATE;
  }
  throw std::invalid_argument("action");
}

// TODO Write a function, getJSON, that returns a std::string containing the
// JSON representation of a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::cout << getJSON(tlObj);
std::string App::getJSON(TodoList &tlObj)
{
  // return "{}";
  // Only uncomment this once you have implemented the functions used!
  return tlObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
// JSON representation of a specific Project in a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::cout << getJSON(tlObj, p);
std::string App::getJSON(TodoList &tlObj, const std::string &p)
{
  // return "{}";
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  return pObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Task in a TodoList object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::string t = "task argument value";
//  std::cout << getJSON(tlObj, p, t);
std::string App::getJSON(TodoList &tlObj, const std::string &p,
                         const std::string &t)
{
  // return "{}";
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(t);
  return tObj.str();
}

// DONE Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Tag in a TodoList object. If the tag
//  does not exist, return an empty string "".
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  TodoList tlObj{};
//  std::string p = "project argument value";
//  std::string task = "task argument value";
//  std::string tag = "tag argument value";
//  std::cout << getJSON(tlObj, p, task, tag);
std::string App::getJSON(TodoList &tlObj, const std::string &p,
                         const std::string &task, const std::string &tag)
{
  // return "{}";
  // Only uncomment this once you have implemented the functions used!
  auto pObj = tlObj.getProject(p);
  const auto tObj = pObj.getTask(task);
  if (tObj.containsTag(tag))
  {
    return tag;
  }
  else
  {
    return "";
  }
}
