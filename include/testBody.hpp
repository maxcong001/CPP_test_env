#pragma once
#include "testUtil.hpp"
class test_body_base
{
  public:
    bool _is_async;
    void set_is_async(bool is_async)
    {
        _is_async = is_async;
    }
    bool get_is_async()
    {
        return _is_async;
    }
    virtual case_result run(void *arg, unsigned long id)
    {
    }
    std::future<case_result> _run(void *arg, std::string sig, std::future<case_result> &fut)
    {
        // convert sig to case id

        unsigned long case_id;

        if (get_is_async())
        {
            result_promise.get_future();
        }
        else
        {
            // get promise
        }
    }
}

class case_result_map
{
  public:
    void insert(unsigned long id, std::future<case_result> &&fut)
    {
        _case_result_fut.emplace(id, fut);
    }
    static std::map<unsigned long, std::future<case_result>> _case_result_fut;
};

// we need ID->result map
// we need id->result future map
// we need id<->sig map