#pragma once

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
#include "NonCopyable.hpp"

using namespace std;

typedef std::function<void()> TEST_FUNCTION;
typedef std::function<void(void *)> TEST_BODY_FUNCTION;
typedef std::function<void*()> TEST_PREPARE_FUNCTION;
typedef std::function<void*()> TEST_DESTROY_FUNCTION;
vector<TEST_FUNCTION> TEST_VECTOR;
typedef std::lock_guard<std::recursive_mutex> TEST_GUARD;
typedef std::unique_lock<std::recursive_mutex> TEST_UNIQUE;
std::recursive_mutex func_mutex;




void init_cb(RetStatus::ptr_t status)
{
    if (status->code != RET_OK)
    {
        cout << " -- Init callback, RetCode: " << RetCodeString(status->code) << ", info: " << status->info << endl;
    }
}