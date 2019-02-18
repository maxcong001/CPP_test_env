#include "testUtil.hpp"
#include <sstream>
std::string _SUCCESS = std::string("").append(green).append("SUCCESS").append(normal);
std::string _FAIL = std::string("").append(red).append("FAIL").append(normal);
std::map<std::string, std::map<std::string, std::map<std::string, case_result>>> case_reslut_container;

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
    std::cout << "project is : " << project_name << ", suit is : " << suit_name << ", case is : " << case_name << std::endl;
    return std::make_tuple(project_name, suit_name, case_name);
}

void DUMP_RESULT()
{
    int pass = 0;
    int fail = 0;

    for (auto project : case_reslut_container)
    {
        cout << "now showing the result under project : " << magenta << project.first<<", total "<<project.second.size()<<" suit" << normal << endl;
        for (auto suit : project.second)
        {
            cout << "now showing the result under suit : " << magenta << suit.first << normal << endl;
            for (auto one_case : suit.second)
            {
                string result;
                if (one_case.second == CASE_SUCCESS)
                {

                    result = _SUCCESS;
                    pass++;
                }
                else
                {
                    result = _FAIL;
                    fail++;
                }
                cout << "now showing the result of case : " << cyan << one_case.first << ", result is : " << result << normal << endl;
            }
        }
    }

    cout << magenta << "total run [ " << (pass + fail) << " ] cases, " << normal << green << "[ " << pass << " ] cases pass" << normal
         << ", " << red << "[ " << fail << " ] cases fail " << normal << endl;
}

void REC_RESULT(case_result result, std::string sig)
{
    std::string project_name;
    std::string suit_name;
    std::string case_name;
    std::tie(project_name, suit_name, case_name) = get_project_suit_case_name(sig);
    if (case_reslut_container.find(project_name) != case_reslut_container.end() && case_reslut_container[project_name].find(suit_name) != case_reslut_container[project_name].end() && case_reslut_container[project_name][suit_name].find(case_name) != case_reslut_container[project_name][suit_name].end())
    {
        std::cout << "there is already result for test case : " << sig << std::endl;
    }
    else
    {
        case_reslut_container[project_name][suit_name][case_name] = result;
    }
}
void REC_RESULT_FINAL(case_result result, string sig)
{
    std::string project_name;
    std::string suit_name;
    std::string case_name;
    std::tie(project_name, suit_name, case_name) = get_project_suit_case_name(sig);
    case_reslut_container[project_name][suit_name][case_name] = result;
}
