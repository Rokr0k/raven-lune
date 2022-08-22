#include <fstream>
#include "file.hpp"
#include <string.h>
#include <algorithm>

using namespace rl;

std::vector<bms::Chart *> file::charts;
std::future<void> file::loading;

#if defined(__unix__)

#include <glob.h>

static void findBMS(const std::string &path, std::vector<std::future<void>> &tasks)
{
    glob_t g;
    glob((path + "/**/*.bm{s,e,l}").c_str(), GLOB_TILDE | GLOB_BRACE, NULL, &g);
    for (size_t j = 0; j < g.gl_pathc; j++)
    {
        tasks.push_back(std::async(
            std::launch::async, [](std::string file)
            {
                bms::Chart *chart = bms::parseBMS(file);
                if(chart)
                {
                    file::charts.push_back(chart);
                } },
            std::string(g.gl_pathv[j])));
    }
    globfree(&g);
}

#elif defined(_WIN32)

#include <regex>
#include <Windows.h>
#include <fileapi.h>

static void findBMS(const std::string &path, std::vector<std::future<void>> &tasks)
{
    printf("%s\n", path.c_str());
    std::string a(path);
    if (a.back() != '/' && a.back() != '\\')
    {
        a += '\\';
    }
    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(path.c_str(), &data);
    do
    {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            findBMS(a + data.cFileName, tasks);
        }
        else
        {
            std::string p(a + data.cFileName);
            if (std::regex_match(p, std::regex(R"(.*\.bm[sel])")))
            {
                tasks.push_back(std::async(
                    std::launch::async, [](std::string file)
                    {
                bms::Chart *chart = bms::parseBMS(file);
                if(chart)
                {
                    file::charts.push_back(chart);
                } },
                    p));
            }
        }
    } while (FindNextFileA(h, &data));
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