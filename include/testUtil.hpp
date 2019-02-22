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
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <future>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "NonCopyable.hpp"
#include "threadPool.hpp"
using namespace std;
/*
3/4 bit
https://en.wikipedia.org/wiki/ANSI_escape_code
*/
static const char black[] = {0x1b, '[', '1', ';', '3', '0', 'm', 0};
static const char red[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};
static const char green[] = {0x1b, '[', '1', ';', '3', '2', 'm', 0};
static const char yellow[] = {0x1b, '[', '1', ';', '3', '3', 'm', 0};
static const char blue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0};
static const char magenta[] = {0x1b, '[', '1', ';', '3', '5', 'm', 0};
static const char cyan[] = {0x1b, '[', '1', ';', '3', '6', 'm', 0};
static const char white[] = {0x1b, '[', '1', ';', '3', '7', 'm', 0};
static const char normal[] = {0x1b, '[', '0', ';', '3', '9', 'm', 0};

std::string _SUCCESS = std::string("").append(green).append("SUCCESS").append(normal);
std::string _FAIL = std::string("").append(red).append("FAIL").append(normal);

enum case_result
{
	CASE_SUCCESS = 0,
	CASE_RUNNING,
	CASE_TIMEOUT,
	CASE_STUB,
	CASE_FAIL

};

typedef void *(*TEST_PREPARE_FUNCTION)();
typedef void (*TEST_DESTROY_FUNCTION)(void *);
std::tuple<std::string, std::string, std::string> get_project_suit_case_name(std::string sig);
//  NOTE: before using this template, you should write operator ==.
//  this is for sync compare result
//  to do: maybe use case class instance as arg, then print the case name and
//  case info.
template <typename EXP_RESULT, typename REL_RESULT>
case_result EXCEPT_EQ(EXP_RESULT &&except_result, REL_RESULT &&real_result)
{
	return ((except_result == real_result) ? CASE_SUCCESS : CASE_FAIL);
}

class sigIDMapping
{
  public:
	static unsigned long add(std::string sig)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		++current_id;
		sig_id_map[current_id] = sig;
		return current_id;
	}
	static std::string get_sig(unsigned long id)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		return (sig_id_map.find(id) == sig_id_map.end()) ? "" : sig_id_map[id];
	}

  private:
	static std::map<unsigned long, std::string> sig_id_map;
	static unsigned long current_id;
	static std::mutex result_mutex;
};
class result_container
{
  public:
	static bool set_case_promise_with_id(unsigned long id, std::promise<case_result> &&case_promise)
	{
		std::string sig;
		sig = sigIDMapping::get_sig(id);
		return set_case_promise(sig, std::move(case_promise));
	}
	static bool set_case_promise(std::string sig, std::promise<case_result> &&case_promise)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		std::string project_name;
		std::string suit_name;
		std::string case_name;
		std::tie(project_name, suit_name, case_name) = get_project_suit_case_name(sig);
		if (_case_promise_container.find(project_name) != _case_promise_container.end() && _case_promise_container[project_name].find(suit_name) != _case_promise_container[project_name].end() && _case_promise_container[project_name][suit_name].find(case_name) != _case_promise_container[project_name][suit_name].end())
		{
			std::cout << "there is already promise for test case : " << sig << std::endl;
			return false;
		}
		else
		{
			std::cout << "set promise for case : " << sig << std::endl;
			_case_promise_container[project_name][suit_name][case_name] = std::move(case_promise);
		}
		return true;
	}
	static bool set_case_future_with_id(unsigned long id, std::future<case_result> &&case_future)
	{
		std::string sig;
		sig = sigIDMapping::get_sig(id);
		return set_case_future(sig, std::move(case_future));
	}
	static bool set_case_future(std::string sig, std::shared_future<case_result> case_future)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		std::string project_name;
		std::string suit_name;
		std::string case_name;
		std::tie(project_name, suit_name, case_name) = get_project_suit_case_name(sig);
		if (_case_reslut_container.find(project_name) != _case_reslut_container.end() && _case_reslut_container[project_name].find(suit_name) != _case_reslut_container[project_name].end() && _case_reslut_container[project_name][suit_name].find(case_name) != _case_reslut_container[project_name][suit_name].end())
		{
			std::cout << "there is already future for test case : " << sig << std::endl;
			return false;
		}
		else
		{
			_case_reslut_container[project_name][suit_name][case_name] = std::move(case_future);
		}
		return true;
	}

	static bool record_result_with_sig(case_result result, std::string sig)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		std::string project_name;
		std::string suit_name;
		std::string case_name;
		std::tie(project_name, suit_name, case_name) = get_project_suit_case_name(sig);

		if (_case_promise_container.find(project_name) != _case_promise_container.end() && _case_promise_container[project_name].find(suit_name) != _case_promise_container[project_name].end() && _case_promise_container[project_name][suit_name].find(case_name) != _case_promise_container[project_name][suit_name].end())
		{
			try
			{
				_case_promise_container[project_name][suit_name][case_name].set_value(result);
			}
			catch (std::future_error &e)
			{
				std::cout << "the promise is already set!!!!" << std::endl;
				return false;
			}
		}
		else
		{
			//_case_promise_container[project_name][suit_name].emplace(sig, std::move());
			std::cout << "there is no promise for test case : " << sig << std::endl;
			return false;
		}
		return true;
	}
	static bool record_result_with_id(case_result result, unsigned long id)
	{
		std::string sig;
		sig = sigIDMapping::get_sig(id);
		return record_result_with_sig(result, sig);
	}
	static void dump_result()
	{
		int pass = 0;
		int fail = 0;
		std::unique_lock<std::mutex> lock(result_mutex);
		for (auto project : _case_reslut_container)
		{
			cout << "now showing the result under project : " << magenta << project.first << ", total " << project.second.size() << " suit" << normal << endl;
			for (auto suit : project.second)
			{
				cout << "now showing the result under suit : " << magenta << suit.first << normal << endl;
				for (auto one_case : suit.second)
				{
					string result;

					std::chrono::seconds span(10);
					/*
					if (one_case.second.wait_for(span) == std::future_status::timeout)
					{
						cout << "timeout waiting for the result of case : " << cyan << one_case.first << normal << endl;
						result = _FAIL;
						fail++;
					}
					else
					{*/
					if (one_case.second.get() == CASE_SUCCESS)
					{
						result = _SUCCESS;
						pass++;
					}
					else
					{
						result = _FAIL;
						fail++;
					}
					//}
					cout << "now showing the result of case : " << cyan << one_case.first << ", result is : " << result << normal << endl;
				}
			}
		}

		cout << magenta << "total run [ " << (pass + fail) << " ] cases, " << normal << green << "[ " << pass << " ] cases pass" << normal
			 << ", " << red << "[ " << fail << " ] cases fail " << normal << endl;
	}

	static std::map<std::string, std::map<std::string, std::map<std::string, std::shared_future<case_result>>>> _case_reslut_container;
	static std::map<std::string, std::map<std::string, std::map<std::string, std::promise<case_result>>>> _case_promise_container;
	static std::mutex result_mutex;
};

// to do: async result calculate