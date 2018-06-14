#include "testUtil.hpp"

RESULT_LIST case_reslut_list;

void DUMP_RESULT();
{
    int pass = 0;
    int fail = 0;
    for (auto i : case_reslut_list)
    {
        string result;
        if (std::get<1>(i) == CASE_SUCCESS)
        {
            result = "SUCCESS";
            pass++;
        }
        else if (std::get<1>(i) == CASE_STUB)
        {
            cout << "now showing the result under suit : " << std::get<0>(i) << endl;
            continue;
        }
        else
        {
            fail++;
            result = "FAIL";
        }
        cout << "case name : " << std::get<0>(i) << " result is :" << result
             << endl;
    }
    cout << "total run " << (pass + fail) << " cases, " << pass << " cases pass"
         << ", " << fail << " cases fail " << endl;
}
void REC_RESULT(case_result result, string case_name)
{
    case_reslut_list.emplace_back(std::make_tuple(case_name, result))
}
void ADD_SUIT_INFO(string suit_name);
{
    case_reslut_list.emplace_back(std::make_tuple(suit_name, CASE_STUB));
}