/*
 * Copyright (c) 2016-20017 Max Cong <savagecm@qq.com>
 * this code can be found at https://github.com/maxcong001/CPP_test_env
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "testInclude.hpp"
#include <memory>

class test_body_example_async : public test_body_base<sigIDMapping>
{
  public:
	using test_body_base::test_body_base;
	test_body_example_async()
	{
		set_async();
	}
	case_result body(unsigned long id, std::shared_ptr<sigIDMapping> arg) override
	{
		std::cout << "[case body] in the async case body" << std::endl;
		std::thread result_thread([id]() {
			std::cout << "[case result] start a new thread and wait" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cout << "[case result] after sleep, record result" << std::endl;
			REC_CASE_FAIL(id);
		});
		result_thread.detach();
		return CASE_SUCCESS;
	}
};
class test_body_example : public test_body_base<sigIDMapping>
{
  public:
	using test_body_base::test_body_base;
	case_result body(unsigned long id, std::shared_ptr<sigIDMapping> arg) override
	{
		return CASE_SUCCESS;
	}
};
class test_case_example_async : public test_case<sigIDMapping>
{
  public:
	using test_case::test_case;
	bool prepare_env() override
	{
		_body = std::make_shared<test_body_example_async>();
		return true;
	}
	bool destroy_env() override
	{
		return true;
	}
};

class test_case_example : public test_case<sigIDMapping>
{
  public:
	using test_case::test_case;
	bool prepare_env() override
	{
		_body = std::make_shared<test_body_example>();
		return true;
	}
	bool destroy_env() override
	{
		return true;
	}
};

class test_suit_example : public test_suit_base
{
  public:
	using test_suit_base::test_suit_base;
	void init() override
	{
		addCase(std::make_shared<test_case_example>("case name", "case info"));
		addCase(std::make_shared<test_case_example_async>("case name1", "case info"));
		addCase(std::make_shared<test_case_example>("case name2", "case info"));
		addCase(std::make_shared<test_case_example_async>("case name3", "case info"));
		addCase(std::make_shared<test_case_example>("case name4", "case info"));
		addCase(std::make_shared<test_case_example_async>("case name5", "case info"));
	}
};

class test_project_example : public test_project_base
{
  public:
	using test_project_base::test_project_base;
	void init() override
	{
		add_suit(std::make_shared<test_suit_example>("suit name"));
	}
};

int main()
{
	auto _project = std::make_shared<test_project_example>("test");
	_project->init();
	_project->run();
}
