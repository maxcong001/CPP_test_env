#include "testUtil.hpp"
#include <sstream>
#include <set>

std::vector<std::string> sig_split(const std::string &s, char delim)
{
    std::vector<std::string> elems;

    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        if (!item.empty())
        {
            elems.push_back(item);
        }
    }

    return elems;
}

std::tuple<std::string, std::string, std::string> get_project_suit_case_name(std::string sig)
{
    std::vector<std::string> sig_result = sig_split(sig, ':');
    std::string project_name = sig_result[0];
    std::string suit_name = sig_result[1];
    std::string case_name = sig_result[2];
    //std::cout << "project is : " << project_name << ", suit is : " << suit_name << ", case is : " << case_name << std::endl;
    return std::make_tuple(project_name, suit_name, case_name);
}
