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
        std::ifstream list("res/data/list.txt");
        std::string dir;
        std::regex fileRegex = std::regex(R"(^\.(bms|bme|bml)$)", std::regex_constants::icase);
        std::vector<std::future<void>> tasks;
        while (std::getline(list, dir))
        {
            std::filesystem::path dirPath = std::filesystem::canonical(std::regex_replace(dir, std::regex(R"(~)"), getenv("HOME")));
            if (std::filesystem::is_directory(dirPath))
            {
                for (const std::filesystem::directory_entry &i : std::filesystem::recursive_directory_iterator(dirPath))
                {
                    if (i.is_regular_file() && std::regex_match(i.path().extension().string(), fileRegex))
                    {
                        tasks.push_back(std::async(std::launch::async, [](const std::filesystem::directory_entry &i){
                            bms::Chart *chart = bms::parseBMS(i.path());
                            if(chart)
                            {
                                file::charts.push_back(chart);
                            }
                        }, i));
                    }
                }
            }
        }
        list.close();
        for(const std::future<void> &task : tasks)
        {
            task.wait();
        }
        std::stable_sort(file::charts.begin(), file::charts.end(), [](const bms::Chart *a, const bms::Chart *b){
            if(a->title == b->title)
            {
                if(a->artist == b->artist)
                {
                    if(a->difficulty == b->difficulty)
                    {
                        return a->playLevel < b->playLevel;
                    }
                    return a->difficulty < b->difficulty;
                }
                return a->artist < b->artist;
            }
            return a->title < b->title;
        }); });
}

void file::release()
{
    file::loading.wait();
    for (const bms::Chart *chart : file::charts)
    {
        delete chart;
    }
    file::charts.clear();
}