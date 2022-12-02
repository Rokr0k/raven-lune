#include <fstream>
#include "file.hpp"
#include <string.h>
#include <algorithm>
#include <filesystem>
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
        std::vector<std::future<void>> tasks;
        while (std::getline(list, dir))
        {
            if (std::filesystem::exists(dir))
            {
                for (const std::filesystem::directory_entry &i : std::filesystem::recursive_directory_iterator(dir, std::filesystem::directory_options::follow_directory_symlink | std::filesystem::directory_options::skip_permission_denied))
                {
                    if (i.is_regular_file() && std::regex_match(i.path().extension().string(), std::regex(R"(\.bm[sel])")))
                    {
                        tasks.push_back(std::async(
                            std::launch::async, [](std::string file)
                            {
                                bms::Chart *chart = bms::parseBMS(file);
                                if (chart)
                                {
                                    file::charts.push_back(chart);
                                }
                            },
                            i.path().string()));
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
            return a->filename < b->filename;
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
    std::filesystem::path parent(path.parent_path());
    if (std::filesystem::exists(parent))
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(parent))
        {
            if (i.path().stem() == path.stem())
            {
                res.push_back(i.path().string());
            }
        }
    }
    return res;
}