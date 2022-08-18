#include <fstream>
#include "file.hpp"
#include <regex>

using namespace rl;

std::vector<bms::Chart *> file::charts;
std::future<void> file::loading;

#if defined(__unix__)

#include <glob.h>

static void loopFiles(const std::string &path, std::vector<std::future<void>> &tasks)
{
    glob_t g;
    glob((path + "/**/*.bms").c_str(), GLOB_TILDE | GLOB_BRACE, NULL, &g);
    glob((path + "/**/*.bme").c_str(), GLOB_TILDE | GLOB_BRACE | GLOB_APPEND, NULL, &g);
    glob((path + "/**/*.bml").c_str(), GLOB_TILDE | GLOB_BRACE | GLOB_APPEND, NULL, &g);
    for (size_t j = 0; j < g.gl_pathc; j++)
    {
        tasks.push_back(std::async(
            std::launch::async, [](const std::string &file)
            {
                            bms::Chart *chart = bms::parseBMS(file);
                            if(chart)
                            {
                                file::charts.push_back(chart);
                            } },
            g.gl_pathv[j]));
    }
    globfree(&g);
}

#elif defined(__WIN32__)

#include <Windows.h>

static void loopFiles(const std::string &path, std::vector<std::future<void>> &tasks)
{
    //
}

#endif

void file::initialise()
{
    loading = std::async(std::launch::async, []()
                         {
        std::ifstream list("res/data/list.txt");
        std::string dir;
        std::vector<std::future<void>> tasks;
        while (std::getline(list, dir))
        {
            loopFiles(dir, tasks);
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