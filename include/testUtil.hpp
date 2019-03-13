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
#include "threadPool.hpp"
#include <sstream>

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

enum case_result : unsigned char
{
	CASE_SUCCESS = 0,
	CASE_RUNNING,
	CASE_TIMEOUT,
	CASE_STUB,
	CASE_FAIL

};

class test_util
{
  public:
	static std::tuple<std::string, std::string, std::string> get_project_suit_case_name(std::string sig)
	{
		std::vector<std::string> sig_result = sig_split(sig, ':');
		std::string project_name = sig_result[0];
		std::string suit_name = sig_result[1];
		std::string case_name = sig_result[2];
		//std::cout << "project is : " << project_name << ", suit is : " << suit_name << ", case is : " << case_name << std::endl;
		return std::make_tuple(project_name, suit_name, case_name);
	}
	static std::vector<std::string> sig_split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;

		std::stringstream ss(s);
		std::string item;

		while (std::getline(ss, item, delim))
		{
			if (!item.empty())
			{
				elems.push_back(item);
			}
		}
		return elems;
	}
};

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
	static inline sigIDMapping *instance()
	{
		static auto ins = new sigIDMapping();
		return ins;
	}
	unsigned long add(std::string sig)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		++current_id;
		sig_id_map[current_id] = sig;
		return current_id;
	}
	std::string get_sig(unsigned long id)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		return (sig_id_map.find(id) == sig_id_map.end()) ? "" : sig_id_map[id];
	}

  private:
	std::map<unsigned long, std::string> sig_id_map;
	unsigned long current_id;
	std::mutex result_mutex;
};
class result_container
{
  public:
	static inline result_container *instance()
	{
		static auto ins = new result_container();
		return ins;
	}
	bool set_case_promise_with_id(unsigned long id, std::promise<case_result> &&case_promise)
	{
		std::string sig;
		sig = sigIDMapping::instance()->get_sig(id);
		return set_case_promise(sig, std::move(case_promise));
	}
	bool set_case_promise(std::string sig, std::promise<case_result> &&case_promise)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		if (_case_promise_container.find(sig) != _case_promise_container.end())
		{
			std::cout << "there is already promise for test case : " << sig << std::endl;
			return false;
		}
		else
		{
			_case_promise_container[sig] = std::move(case_promise);
		}
		return true;
	}

	bool record_result_with_sig(case_result result, std::string sig)
	{
		std::unique_lock<std::mutex> lock(result_mutex);
		std::string project_name;
		std::string suit_name;
		std::string case_name;
		std::tie(project_name, suit_name, case_name) = test_util::get_project_suit_case_name(sig);

		if (_case_promise_container.find(sig) != _case_promise_container.end())
		{
			_case_promise_container[sig].set_value(result);
		}
		else
		{
			_case_reslut_container[project_name][suit_name][case_name] = result;
		}

		return true;
	}
	bool record_result_with_id(case_result result, unsigned long id)
	{
		std::string sig;
		sig = sigIDMapping::instance()->get_sig(id);
		return record_result_with_sig(result, sig);
	}
	void dump_result()
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
					if (one_case.second == CASE_SUCCESS)
					{
						result = _SUCCESS;
						pass++;
					}
					else
					{
						result = _FAIL;
						fail++;
					}
					cout << "now showing the result of case : " << cyan << one_case.first << ", result is : " << result << normal << endl;
				}
			}
		}

		cout << magenta << "total run [ " << (pass + fail) << " ] cases, " << normal << green << "[ " << pass << " ] cases pass" << normal
			 << ", " << red << "[ " << fail << " ] cases fail " << normal << endl;
	}
	std::map<std::string, std::map<std::string, std::map<std::string, case_result>>> _case_reslut_container;
	std::map<std::string, std::promise<case_result>> _case_promise_container;
	std::mutex result_mutex;
};
std::function<bool(unsigned long)> REC_CASE_FAIL = std::bind(&result_container::record_result_with_id, result_container::instance(), CASE_FAIL, std::placeholders::_1);
std::function<bool(unsigned long)> REC_CASE_SUCCESS = std::bind(&result_container::record_result_with_id, result_container::instance(), CASE_SUCCESS, std::placeholders::_1);
