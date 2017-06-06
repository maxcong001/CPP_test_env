## This is a test framework for CPP development.
detail see the example.
## how to new a case
```
shared_ptr<test_case_base> case_0001(new test_case_base(prepare_env_example, body_0001, destroy_env_example, "case name", "case info "));
```
## how to new a suit 
```
shared_ptr<test_suit_base> suit_0001(new test_suit_base("suit name"));
```
## how to use:
### frist you will have a project, which is a singleton. 
```
auto project_instance = Singleton<test_project_base>::Instance();
```
### then you will have some suits, then add them to the project.
```
project_instance->add_suit(suit_0001);
```
### then you must have some test cases, and add them to suit.
```
suit_0001->addCase(case_0001);
```
#### test case is made up of test body, prepare env and destory env
```
shared_ptr<test_case_base> case_0001(new test_case_base(prepare_env_example, body_0001, destroy_env_example, "case name", "case info "));
```

Have fun

Max
