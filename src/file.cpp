#include "file.hpp"
#include <filesystem>
#include <fstream>
#include <regex>

using namespace rl;

std::vector<bms::Chart *> file::charts;
std::future<void> file::loading;

void file::initialise()
{
    loading = std::async(std::launch::async, []()
                         {
        std::ifstream list("res/list.txt");
        std::string file;
        std::regex fileRegex = std::regex(R"(^.*\.(bms|bme|bml)$)");
        while (std::getline(list, file))
        {
            if (std::filesystem::is_directory(file))
            {
                for (const std::filesystem::directory_entry &i : std::filesystem::recursive_directory_iterator(file))
                {
                    if (i.is_regular_file() && std::regex_match(i.path().string(), fileRegex))
                    {
                        file::charts.push_back(bms::parseBMS(i.path()));
                    }
                }
            }
        } });
}

void file::release()
{
    for (const bms::Chart *chart : file::charts)
    {
        delete chart;
    }
    file::charts.clear();
}