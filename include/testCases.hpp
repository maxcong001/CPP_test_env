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
#include "testBody.hpp"
#include "threadPool.hpp"
class test_case_base
{
  public:
	test_case_base(string case_name, string info)
	{
		_case_name = case_name;
		_info = info;
	}
	virtual ~test_case_base()
	{
	}
	void set_case_name(std::string name)
	{
		_case_name = name;
	}
	std::string get_case_name()
	{
		return _case_name;
	}
	void set_suit_name(std::string name)
	{
		_suit_name = name;
	}
	std::string get_suit_name()
	{
		return _suit_name;
	}
	void set_project_name(std::string name)
	{
		_project_name = name;
	}
	std::string get_project_name()
	{
		return _project_name;
	}
	std::string get_signature()
	{
		return _project_name + ":" + _suit_name + ":" + _case_name;
	}
	virtual void run() = 0;
	std::string _case_name;
	std::string _suit_name;
	std::string _project_name;
	std::string _info;
};
template <typename env_arg>
class test_case : public test_case_base
{
  public:
	using test_case_base::test_case_base;
	virtual bool prepare_env() = 0;
	virtual bool destroy_env() = 0;
	virtual void run()
	{
		result_container::instance()->record_result_with_sig(CASE_FAIL, get_signature());
		std::cout << "now running case : " << _case_name << ", case info is : " << _info << std::endl;
		if (!prepare_env())
		{
			return;
		}
		unsigned long case_id = sigIDMapping::instance()->add(get_signature());
		if (_body)
		{
			if (_body->get_is_async())
			{
			
				if (_body->run(_arg, case_id) != CASE_SUCCESS)
				{
					
					result_container::instance()->record_result_with_sig(CASE_FAIL, get_signature());
				}
			}
			else
			{
				result_container::instance()->record_result_with_sig(_body->run(_arg, case_id), get_signature());
			}
		}
		else
		{
			std::cout << "no test body in the case : " << get_signature() << std::endl;
			result_container::instance()->record_result_with_sig(CASE_FAIL, get_signature());
		}
		destroy_env();
	}
	void set_body(std::shared_ptr<test_body_base<env_arg>> body)
	{
		_body = body;
	}
	std::shared_ptr<test_body_base<env_arg>> _body;
	std::shared_ptr<env_arg> _arg;
};
class case_pool
{
  public:
	case_pool()
	{
		int thread_num = std::thread::hardware_concurrency();
		_thread_pool_sptr.reset(new ThreadPool(thread_num));
	}
	static inline case_pool *instance()
	{
		static auto ins = new case_pool();
		return ins;
	}
	void add_case(std::shared_ptr<test_case_base> _case)
	{
		_case_pool[_case->get_case_name()] = _case;
	}
	void run()
	{
		std::vector<std::future<void>> _results;
		for (auto i : _case_pool)
		{
			std::function<void(void)> task = std::bind(&test_case_base::run, i.second);
			_results.push_back(_thread_pool_sptr->enqueue(task));
		}
		// wait until all the cases done
		for (auto &&result : _results)
		{
			if (result.valid())
			{
				result.wait();
			}
		}
	}
	std::shared_ptr<ThreadPool> _thread_pool_sptr;
	std::map<std::string, std::shared_ptr<test_case_base>> _case_pool;
};
