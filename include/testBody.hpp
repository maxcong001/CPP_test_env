#pragma once
#include "testUtil.hpp"
class test_body_base : NonCopyable,
                       public std::enable_shared_from_this<test_body_base>
{
  public:
    test_body_base() = delete;
    test_body_base(std::function<case_result(void *arg, unsigned long id)> func, bool is_async)
    {
        _is_async = is_async;
        _func = func;
    }

    void set_is_async(bool is_async)
    {
        _is_async = is_async;
    }
    bool get_is_async()
    {
        return _is_async;
    }

    std::shared_future<case_result> run(void *arg, std::string sig)
    {
        unsigned long case_id = sigIDMapping::add(sig);
        std::promise<case_result> result_promise;
        std::shared_future<case_result> result_future(result_promise.get_future());
        result_container::set_case_future(sig, result_future);
        result_container::set_case_promise(sig, std::move(result_promise));
        if (get_is_async())
        {
            case_result result = _func(arg, case_id);
            if (result != CASE_SUCCESS)
            {
                result_container::record_result_with_sig(result, sig);
            }
        }
        else
        {
            result_container::record_result_with_sig(_func(arg, case_id), sig);
        }

        return result_future;
    }
    bool _is_async;
    std::function<case_result(void *arg, unsigned long id)> _func;
};
