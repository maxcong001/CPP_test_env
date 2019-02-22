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
#include "testInclude.hpp"
#include "env_pool.hpp"
#include "body_pool.hpp"

//-------------------------sync body ---------------------------
shared_ptr<test_body_base> body_0001(new test_body_base([](void *arg, unsigned long id) -> case_result {
	return CASE_SUCCESS;
},
													   false));
shared_ptr<test_body_base> body_0002(new test_body_base([](void *arg, unsigned long id) -> case_result {
	return CASE_FAIL;
},
													   false));

//-------------------------async body ---------------------------

shared_ptr<test_body_base> body_0003(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_FAIL, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));
shared_ptr<test_body_base> body_0004(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_SUCCESS, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));

shared_ptr<test_body_base> body_0005(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_TIMEOUT, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));
// return fail
shared_ptr<test_body_base> body_0006(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_SUCCESS, id);
	});
	test_thread.detach();

	return CASE_FAIL;
},
													   true));
// success
shared_ptr<test_body_base> body_0007(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_SUCCESS, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));
shared_ptr<test_body_base> body_0008(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_SUCCESS, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));
shared_ptr<test_body_base> body_0009(new test_body_base([](void *arg, unsigned long id) -> case_result {
	std::thread test_thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		result_container::record_result_with_id(CASE_SUCCESS, id);
	});
	test_thread.detach();

	return CASE_SUCCESS;
},
													   true));

shared_ptr<test_case_base> case_0001(
	new test_case_base(prepare_env_example, body_0001, destroy_env_example,
					   "case_0001"));
shared_ptr<test_case_base> case_0002(
	new test_case_base(prepare_env_example, body_0002, destroy_env_example,
					   "case_0002"));
shared_ptr<test_case_base> case_0003(
	new test_case_base(prepare_env_example_001, body_0003, destroy_env_example,
					   "case_0003"));
shared_ptr<test_case_base> case_0004(
	new test_case_base(prepare_env_example_001, body_0004, destroy_env_example,
					   "case_0004"));
shared_ptr<test_case_base> case_0005(
	new test_case_base(prepare_env_example_001, body_0005, destroy_env_example,
					   "case_0005"));
shared_ptr<test_case_base> case_0006(
	new test_case_base(prepare_env_example_001, body_0006, destroy_env_example,
					   "case_0006"));
shared_ptr<test_case_base> case_0007(
	new test_case_base(prepare_env_example_001, body_0007, destroy_env_example,
					   "case_0007"));
shared_ptr<test_case_base> case_0008(
	new test_case_base(prepare_env_example_001, body_0002, destroy_env_example,
					   "case_0008"));
shared_ptr<test_case_base> case_0009(
	new test_case_base(prepare_env_example_001, body_0004, destroy_env_example,
					   "case_0009"));
