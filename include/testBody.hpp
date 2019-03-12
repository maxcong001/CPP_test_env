#pragma once
#include "testUtil.hpp"

template <typename = void *>
class test_body_base()
{
  public:
    test_body_base() = delete;
    test_body_base(std::shared_ptr<env_type> env)
    {
        _env env;
    }
    void set_is_async(bool is_async)
    {
        _is_async = is_async;
    }
    bool get_is_async()
    {
        return _is_async;
    }
    virtual case_result run(unsigned long id, env_type arg)
    {
        unsigned long case_id = sigIDMapping::add(sig);
        std::promise<case_result> result_promise;
        std::shared_future<case_result> result_future(result_promise.get_future());
 
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

        return (result_future.valid())?result_future.get():CASE_FAIL;
    }
    std::shared_ptr<env_type> _env;
    bool _is_async;
}
template <typename env_type>
class test_body_sync_base : public test_body_base<test_body_base>
{
  public:
    test_body_base()
    {
        _is_async = false;
    }

    case_result run(void *arg, std::string sig)
    {
        unsigned long case_id = sigIDMapping::add(sig);
        std::promise<case_result> result_promise;
        std::shared_future<case_result> result_future(result_promise.get_future());
        result_container::set_case_future(sig, result_future);
        result_container::set_case_promise(sig, std::move(result_promise));

        result_container::record_result_with_sig(_func(arg, case_id), sig);
    }
};
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

#include <iostream>
#include <typeinfo>
using namespace std;
template <typename... A>
class BMW
{
};                                         // 变长模板的声明
template <typename Head, typename... Tail> // 递归的偏特化定义
class BMW<Head, Tail...> : public BMW<Tail...>
{ //当实例化对象时，则会引起基类的递归构造
  public:
    BMW() { printf("type: %s\n", typeid(Head).name()); }
    Head head;
};
template <>
class BMW<>
{
}; // 边界条件
int main()
{
    BMW<int, char, float> car;
    return 0;
}
