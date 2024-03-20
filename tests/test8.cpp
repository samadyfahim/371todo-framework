// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: Thomas Reitmaier
//         thomas.reitmaier@swansea.ac.uk
//         Martin Porcheron (adapted from)
//         m.a.w.porcheron@swansea.ac.uk
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// Catch2 — https://github.com/catchorg/Catch2
// Catch2 is licensed under the BOOST license
// -----------------------------------------------------
// This file tests 371todo output for the database
// argument and 'json' value of the action argument.
// This test works by capturing the stdout of your
// program.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/371todo.h"

// Redirect std::cout to a buffer
// by Björn Pollex
// via https://stackoverflow.com/a/5419388
// licensed under CC BY-SA 3.0.
class CoutRedirect
{
private:
  std::streambuf *old;

public:
  CoutRedirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer))
  { /* do nothing */
  }

  ~CoutRedirect() { std::cout.rdbuf(old); }
};

SCENARIO("The database and action program arguments can be parsed correctly "
         "such that a file can be opened, json output printed, parsed, and "
         "output to stdout",
         "[args]")
{

  const std::string filePath = "./tests/testdatabasealt.json";

  auto fileExists = [](const std::string &path)
  {
    return std::ifstream(path).is_open();
  };

  auto writeFileContents = [](const std::string &path,
                              const std::string &contents)
  {
    // Not a robust way to do this, but here it doesn't matter so much, if it
    // goes wrong we'll fail the test anyway…
    std::ofstream f{path};
    f << contents;
  };

  GIVEN("a valid path to a reset database JSON file")
  {

    // Reset the file...
    REQUIRE(fileExists(filePath));
    REQUIRE_NOTHROW(writeFileContents(
        filePath,

        "{ \"CSC307\": { \"Write Mobile App\": { \"completed\": true, "
        "\"dueDate\": \"2023-11-30\", \"tags\": [ \"uni\", \"programming\", "
        "\"android\" ] } }, \"CSC371\": { \"Lab Assignment 1\": { "
        "\"completed\": true, \"dueDate\": \"2024-11-13\", \"tags\": [ "
        "\"uni\", \"c\", \"programming\" ] }, \"Lab Assignment 6\": { "
        "\"completed\": false, \"dueDate\": \"2024-11-23\", \"tags\": [ "
        "\"uni\", \"c++\", \"programming\", \"standard library\" ] } } }"));

    WHEN("the db program argument is '" + filePath +
         "' and the action program argument is 'json'")
    {

      Argv argvObj({"test", "--db", filePath.c_str(), "--action", "json"});
      auto **argv = argvObj.argv();
      auto argc = argvObj.argc();

      std::stringstream buffer;
      CoutRedirect originalBuffer{buffer.rdbuf()};

      THEN("the expected JSON output is printed to stdout")
      {

        REQUIRE_NOTHROW(App::run(argc, argv));

        std::string output = buffer.str();
        REQUIRE(output.find(
            "{\"CSC307\":{\"Write Mobile "
            "App\":{\"completed\":true,\"dueDate\":\"2023-11-30\",\"tags\":["
            "\"uni\",\"programming\",\"android\"]}},\"CSC371\":{\"Lab "
            "Assignment "
            "1\":{\"completed\":true,\"dueDate\":\"2024-11-13\",\"tags\":["
            "\"uni\",\"c\",\"programming\"]},\"Lab Assignment "
            "6\":{\"completed\":false,\"dueDate\":\"2024-11-23\",\"tags\":["
            "\"uni\",\"c++\",\"programming\",\"standard library\"]}}}" == 0));

      } // THEN

    } // WHEN

  } // GIVEN

} // SCENARIO

TEST_CASE("Test JSON representation of a specific tag in TodoList object", "[getJSON]")
{
  // Initialize a TodoList object
  TodoList tlObj;

  // Add projects, tasks, and tags to the TodoList object
  // For example:
  tlObj.newProject("CSC307");
  tlObj.getProject("CSC307").newTask("Write Mobile App");
  tlObj.getProject("CSC307").getTask("Write Mobile App").addTag("uni");
  tlObj.getProject("CSC307").getTask("Write Mobile App").addTag("programming");
  tlObj.getProject("CSC307").getTask("Write Mobile App").addTag("android");

  // Define the tag you want to retrieve
  std::string projectIdent = "CSC307";
  std::string taskIdent = "Write Mobile App";
  std::string tagIdent = "programming";

  // Redirect stdout to a stringstream for capturing the output
  std::stringstream buffer;
  CoutRedirect redirect(buffer.rdbuf());

  // Call the function under test
  std::string tagJson = App::getJSON(tlObj, projectIdent, taskIdent, tagIdent);

  // Remove leading and trailing quotation marks from the output
  if (!tagJson.empty() && tagJson.front() == '"' && tagJson.back() == '"')
  {
    tagJson = tagJson.substr(1, tagJson.size() - 2);
  }

  // Validate the output
  REQUIRE(tagJson == "programming");

  // Optional: Validate the content of stdout
  std::string output = buffer.str();
  REQUIRE(output == "programming\n");
}

// Define the test case
TEST_CASE("Test JSON representation of a specific task in TodoList object", "[getJSON]")
{
  // Initialize a TodoList object
  TodoList tlObj;

  // Add projects, tasks, and tags to the TodoList object
  tlObj.newProject("CSC307");
  tlObj.getProject("CSC307").newTask("Write Mobile App");

  // Define the project and task identifiers
  std::string projectIdent = "CSC307";
  std::string taskIdent = "Write Mobile App";

  // Call the function under test
  std::string taskJson = App::getJSON(tlObj, projectIdent, taskIdent);

  // Validate the output
  REQUIRE(taskJson == "{\"completed\":false,\"dueDate\":\"\",\"tags\":[]}");
}
