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
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "NonCopyable.hpp"

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

enum case_result
{
	CASE_SUCCESS = 0,
	CASE_RUNNING,
	CASE_STUB,
	CASE_FAIL

};
/*
typedef std::function<void(void *)> TEST_BODY_FUNCTION;
typedef std::function<void *()> TEST_PREPARE_FUNCTION;
typedef std::function<void(void *)> TEST_DESTROY_FUNCTION;
*/

typedef case_result (*TEST_BODY_FUNCTION)(void *, unsigned long id);
typedef void *(*TEST_PREPARE_FUNCTION)();
typedef void (*TEST_DESTROY_FUNCTION)(void *);

typedef std::tuple<std::string, case_result> RESULT_TUPLE;
// case name and case result list, this will be read after all the case done.
// when after a case run, should put the case name and case pass, waiting , fail
// info to the list.
typedef std::list<RESULT_TUPLE> RESULT_LIST;

//  NOTE: before using this template, you should write operator ==.
//  this is for sync compare result
//  to do: maybe use case class instance as arg, then print the case name and
//  case info.
template <typename EXP_RESULT, typename REL_RESULT>
case_result EXCEPT_EQ(EXP_RESULT &&except_result, REL_RESULT &&real_result)
{
	return ((except_result == real_result) ? CASE_SUCCESS : CASE_FAIL);
}

void REC_RESULT(case_result result, unsigned long id);
void REC_RESULT(case_result result, std::string sig);
void REC_RESULT_FINAL(case_result result, unsigned long id);
void REC_RESULT_FINAL(case_result result, std::string sig);

void DUMP_RESULT();

class sigIDMapping
{
  public:
	static unsigned long add(std::string sig)
	{
		++current_id;
		sig_id_map[current_id] = sig;
		return current_id;
	}
	static std::string get_sig(unsigned long id)
	{
		return (sig_id_map.find(id) == sig_id_map.end()) ? "" : sig_id_map[id];
	}

  private:
	static std::map<unsigned long, std::string> sig_id_map;

	static unsigned long current_id;
};

// to do: async result calculate