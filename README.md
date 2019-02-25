## This is a test framework for CPP development.
detail see the example.
## dependancy   
C++11
## how to new a case
```
shared_ptr<test_case_base> case_0001(
	new test_case_base(prepare_env_example, body_0001, destroy_env_example,
					   "case_0001"));
```
## how to new a suit 
```
shared_ptr<test_suit_base> suit_0001(new test_suit_base("suit001"));
```
## how to use:
### frist you will have a project, which is a singleton. 
```
std::shared_ptr<test_project_base> project_sptr = std::make_shared<test_project_base>("test_project");
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
now showing the result under project : test_project, total 3 suit
now showing the result under suit : suit001
now showing the result of case : case_0001, result is : SUCCESS
now showing the result of case : case_0002, result is : FAIL
now showing the result of case : case_0003, result is : FAIL
now showing the result of case : case_0004, result is : SUCCESS
now showing the result of case : case_0005, result is : FAIL
now showing the result under suit : suit002
now showing the result of case : case_0006, result is : FAIL
now showing the result of case : case_0007, result is : SUCCESS
now showing the result under suit : suit003
now showing the result of case : case_0008, result is : FAIL
now showing the result of case : case_0009, result is : SUCCESS
total run [ 9 ] cases, [ 4 ] cases pass, [ 5 ] cases fail

```

Have fun

Max
