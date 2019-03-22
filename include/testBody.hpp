#pragma once
#include "testUtil.hpp"

template <typename env_type = void *>
class test_body_base
{
  public:
    test_body_base() = default;
    void set_async()
    {
        _is_async = true;
    }
    void set_sync()
    {
        _is_async = false;
    }

    bool get_is_async()
    {
        return _is_async;
    }
    virtual case_result body(unsigned long id, std::shared_ptr<env_type> env) = 0;
    virtual case_result run(std::shared_ptr<env_type> env, unsigned long id)
    {
        if (get_is_async())
        {

            std::promise<case_result> result_promise;
            std::shared_future<case_result> result_future(result_promise.get_future());
            result_container::instance()->set_case_promise_with_id(id, std::move(result_promise));
            body(id, std::move(env));
            return result_future.get(); //(result_future.valid()) ? result_future.get() : CASE_FAIL;
        }
        else
        {
            return body(id, std::move(env));
        }
    }
    bool _is_async;
};
