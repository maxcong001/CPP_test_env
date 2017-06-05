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
#include <iostream>
#include <vector>
#include <chrono>
#include <map>
#include <thread>
#include <string>
#include <tuple>
#include <condition_variable>
#include <mutex>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "NonCopyable.hpp"
#include "singleton.hpp"

using namespace std;
/*
typedef std::function<void(void *)> TEST_BODY_FUNCTION;
typedef std::function<void *()> TEST_PREPARE_FUNCTION;
typedef std::function<void(void *)> TEST_DESTROY_FUNCTION;
*/
typedef void (*TEST_BODY_FUNCTION)(void *);
typedef void *(*TEST_PREPARE_FUNCTION)();
typedef void (*TEST_DESTROY_FUNCTION)(void *);

enum case_result
{
    CASE_SUCCESS = 0,
    CASE_RUNNING,
    CASE_FAIL

};
// case name and case result map, this will be read after all the case done.
// when after a case run, should put the case name and case pass, waiting , fail info to the map.
typedef std::map<std::string, case_result> RESULT_MAP;
//  NOTE: before using this template, you should write operator ==.
//  this is for sync compare result
//  to do: maybe use case class instance as arg, then print the case name and case info.
template <typename EXP_RESULT, typename REL_RESULT>
void EXCEPT_EQ(EXP_RESULT &&except_result, REL_RESULT &&real_result, string case_name)
{
    RESULT_MAP.emplace_back(case_name, ((except_result == real_result) ? CASE_SUCCESS : CASE_FAIL));
}
// to do: async result calculate