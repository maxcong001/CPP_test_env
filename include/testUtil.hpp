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

typedef std::function<void(void *)> TEST_BODY_FUNCTION;
typedef std::function<void *()> TEST_PREPARE_FUNCTION;
typedef std::function<void(void *)> TEST_DESTROY_FUNCTION;
