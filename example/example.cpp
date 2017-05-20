#include "testInclude.hpp"
int main()
{
    shared_ptr<test_case_base> case_0001(new test_case_base(prepare_env_example, body_0001, destory_env_example));

    shared_ptr<test_suit_base> suit_0001(new test_suit_base());
    suit_0001->addCase(case_0001);

    suit_0001->run();
}
