#pragma once
#include "testUtil.hpp"
class test_suit_base : NonCopyable, public std::enable_shared_from_this<test_suit_base>
{
  public:
    void addCase(std::shared_ptr<test_case_base> test_case)
    {

        _cases[caseID] = test_case;
        caseID++;
    }
    std::shared_ptr<test_suit_base> getSelf()
    {
        return shared_from_this();
    }
    void run()
    {
        for (auto i : _cases)
        {
            (i.second)->run();
        }
    }
    int caseID;
    std::map<int, std::shared_ptr<test_case_base> > _cases;
};