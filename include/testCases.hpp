#pragma once
#include "testUtil.hpp"
//#include "testCasesBody.hpp"
class test_case_base : NonCopyable, public std::enable_shared_from_this<test_case_base>
{
  public:
    test_case_base(TEST_PREPARE_FUNCTION prepare_env, TEST_BODY_FUNCTION body, TEST_DESTROY_FUNCTION destroy_env)
    {
        _prepare_env = prepare_env;
        _body = body;
        _destroy_env = destroy_env;
        if (_prepare_env)
        {
            arg = prepare_env();
        }
    }
    ~test_case_base()
    {
        if (_destroy_env)
        {
            _destroy_env(arg);
        }
    }
    TEST_PREPARE_FUNCTION _prepare_env;
    TEST_BODY_FUNCTION _body;
    TEST_DESTROY_FUNCTION _destroy_env;
    void *arg;

    virtual void run()
    {
        if (_body)
        {
            _body(arg);
        }
    }
    std::shared_ptr<test_case_base> getSelf()
    {
        return shared_from_this();
    }
};