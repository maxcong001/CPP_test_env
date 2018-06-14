#include "testUtil.hpp"
/*
static const char black[] = {0x1b, '[', '1', ';', '3', '0', 'm', 0};
static const char red[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};
static const char green[] = {0x1b, '[', '1', ';', '3', '2', 'm', 0};
static const char yellow[] = {0x1b, '[', '1', ';', '3', '3', 'm', 0};
static const char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0};
static const char magenta[] = {0x1b, '[', '1', ';', '3', '5', 'm', 0};
static const char cyan[] = {0x1b, '[', '1', ';', '3', '6', 'm', 0};
static const char white[] = {0x1b, '[', '1', ';', '3', '7', 'm', 0};


*/
std::string _SUCCESS = std::string("").append(green).append("SUCCESS").append(normal);
std::string _FAIL = std::string("").append(red).append("FAIL").append(normal);
RESULT_LIST case_reslut_list;

void DUMP_RESULT()
{
    int pass = 0;
    int fail = 0;
    for (auto i : case_reslut_list)
    {
        string result;
        if (std::get<1>(i) == CASE_SUCCESS)
        {
            result = _SUCCESS;
            pass++;
        }
        else if (std::get<1>(i) == CASE_STUB)
        {
            cout << "now showing the result under suit : " << magenta << std::get<0>(i) << normal << endl;
            continue;
        }
        else
        {
            fail++;
            result = _FAIL;
        }
        cout << cyan << "case name : " << normal << std::get<0>(i) << " result is :" << result
             << endl;
    }
    cout << magenta << "total run [ " << (pass + fail) << " ] cases, " << normal << green << "[ " << pass << " ] cases pass" << normal
         << ", " << red << "[ " << fail << " ] cases fail " << normal << endl;
}

void REC_RESULT(case_result result, string case_name)
{
    case_reslut_list.emplace_back(std::make_tuple(case_name, result));
}

void ADD_SUIT_INFO(string suit_name)
{
    case_reslut_list.emplace_back(std::make_tuple(suit_name, CASE_STUB));
}