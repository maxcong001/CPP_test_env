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
#include "suit_pool.hpp"
#include "case_pool.hpp"
#include "env_pool.hpp"
#include "body_pool.hpp"
#include <memory>

int main()
{
	std::shared_ptr<test_project_base> project_sptr = std::make_shared<test_project_base>("test_project");
	// prepare suit here
	suit_0001->addCase(case_0001);
	suit_0001->addCase(case_0002);
	suit_0001->addCase(case_0003);
	suit_0001->addCase(case_0004);
	suit_0001->addCase(case_0005);

	suit_0002->addCase(case_0006);
	suit_0002->addCase(case_0007);

	suit_0003->addCase(case_0008);
	suit_0003->addCase(case_0009);

	// add your suit here
	project_sptr->add_suit(suit_0001);
	project_sptr->add_suit(suit_0002);
	project_sptr->add_suit(suit_0003);
	// run!
	project_sptr->run();
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	// dump result
	DUMP_RESULT();
}
