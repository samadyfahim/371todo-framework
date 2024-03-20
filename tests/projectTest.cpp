// This is a Catch2 test case for testing the 371todo output for the database
// argument and 'json' value of the action argument.

#include "../src/lib_catch.hpp"
#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"
#include "../src/371todo.h"

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
        std::ofstream f{path};
        f << contents;
    };

    GIVEN("a valid path to a reset database JSON file")
    {
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
            }
        }
    }
}
