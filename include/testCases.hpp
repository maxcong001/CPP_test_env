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
//#include "testCasesBody.hpp"
template <typename env_arg>
class test_case_base
{
  public:
	test_case_base(std::shared_ptr<test_body_base> body, string case_name, string info)
	{
		_body = body;
		_case_name = case_name;
		_info = info;
	}
	~test_case_base()
	{
	}

	virtual bool prepare_env() = 0;
	virtual bool destroy_env() = 0;

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
	virtual bool run()
	{
		std::cout << "now running case : " << _case_name << ", case info is : " << _info << std::endl;
		prepare_env();
		if (_body)
		{
			result_container::record_result_with_sig(_body->run(_arg, get_signature()), get_signature());
		}
		else
		{
			std::cout << "no test body in the case : " << get_signature() << std::endl;
			result_container::record_result_with_sig(CASE_FAIL, get_signature());
			return false;
		}
		destroy_env();
		return true;
	}



	std::unique_ptr<test_body_base> _body;
	std::unique_ptr<env_arg> _arg;
	std::string _case_name;
	std::string _suit_name;
	std::string _project_name;
	std::string _info;
};