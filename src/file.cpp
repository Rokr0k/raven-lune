#include <fstream>
#include "file.hpp"
#include <string.h>
#include <algorithm>
#include <filesystem>
#include <regex>

using namespace rl;

std::vector<bms::Chart *> file::charts;
std::future<void> file::loading;

static void findBMS(const std::string &path, std::vector<std::future<void>> &tasks)
{
    for (const std::filesystem::directory_entry &i : std::filesystem::recursive_directory_iterator(path))
    {
        if (i.is_regular_file() && std::regex_match(i.path().extension().string(), std::regex(R"(\.bm[sel])")))
        {
            tasks.push_back(std::async(
                std::launch::async, [](std::string file)
                {
                bms::Chart *chart = bms::parseBMS(file);
                if(chart)
                {
                    file::charts.push_back(chart);
                } },
                i.path().string()));
        }
    }
}

void file::initialise()
{
    loading = std::async(std::launch::async, []()
                         {
        std::ifstream list("res/data/list.txt");
        std::string dir;
        std::vector<std::future<void>> tasks;
        while (std::getline(list, dir))
        {
            findBMS(dir, tasks);
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

std::vector<std::string> file::getAltFiles(const std::string &file)
{
    std::vector<std::string> res;
    std::filesystem::path path(file);
    if (std::filesystem::exists(path))
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(path.parent_path()))
        {
            if (i.path().stem() == path.stem())
            {
                res.push_back(i.path().string());
            }
        }
    }
    return res;
}