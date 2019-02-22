#pragma once
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
#include "testUtil.hpp"
class test_suit_base : NonCopyable,
					   public std::enable_shared_from_this<test_suit_base>
{
  public:
	test_suit_base() = delete;
	test_suit_base(string name) { _suit_name = name; }
	void addCase(std::shared_ptr<test_case_base> test_case)
	{
		_cases[test_case->get_case_name()] = test_case;
		test_case->set_suit_name(get_suit_name());
		std::cout << "-----------------now add case to suit : " << get_suit_name() << ", case name is :" << test_case->get_case_name() << std::endl;
		//test_case->set_project_name(get_project_name());
	}
	std::shared_ptr<test_suit_base> getSelf() { return shared_from_this(); }
	void run()
	{
		for (auto i : _cases)
		{
			std::function<bool(void)> task = std::bind(&test_case_base::run, i.second);
			_results.emplace_back(ThreadPool::get_instance()->enqueue(task));
		}

		// wait until all the cases done
		for (auto &&result : _results)
		{
			result.get();
		}
	}
	string get_suit_name() { return _suit_name; }
	void set_suit_name(std::string name)
	{
		_suit_name = name;
		for (auto i : _cases)
		{
			(i.second)->set_suit_name(get_suit_name());
		}
	}

	std::string get_project_name()
	{
		return _project_name;
	}
	void set_project_name(std::string name)
	{
		_project_name = name;
		for (auto i : _cases)
		{
			(i.second)->set_project_name(get_project_name());
		}
	}
	std::string _suit_name;
	std::string _project_name;

	std::unordered_map<std::string, std::shared_ptr<test_case_base>> _cases;

	std::vector<std::future<bool>> _results;
};