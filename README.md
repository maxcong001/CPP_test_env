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

## here is some description about the log output:
```
prepare env is called
case info
this is a test case body
case info
this is a test case body
destroy env is called
prepare 001 env is called
case info
this is a test case body 0002
destroy env is called
prepare env is called
case info
this is a test case body
case info
this is a test case body
case info
this is a test case body
destroy env is called
prepare 001 env is called
case info
this is a test case body 0002
destroy env is called
prepare env is called   <- this means prepare environment is called
case info					<- this shows the test info
this is a test case body <- this means the test body is called
case info
this is a test case body
case info
this is a test case body
case info
this is a test case body
destroy env is called
now showing the result under suit : suit003 <- this shows which suit.
case name : case_0001 result is :SUCCESS     <- this shows case result
case name : case_0002 result is :SUCCESS
now showing the result under suit : suit002
case name : case_0003 result is :FAIL
case name : case_0001 result is :SUCCESS
case name : case_0001 result is :SUCCESS
case name : case_0001 result is :SUCCESS
now showing the result under suit : suit001
case name : case_0003 result is :FAIL
case name : case_0001 result is :SUCCESS
case name : case_0001 result is :SUCCESS
case name : case_0002 result is :SUCCESS
case name : case_0001 result is :SUCCESS
total run 11 cases, 9 cases pass, 2 cases fail <- this shows the final result
```

Have fun

Max
