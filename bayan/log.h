/**
 * @file log.h
 * @author  Sergey Simonov
 * @brief Cout logger, that can be setup in compile time
 */
#pragma once

#include <iostream>
#include "optlog.h"

// Default value, if it wasn't setup in build system
#ifndef VERBOSE_LEVEL
    #define VERBOSE_LEVEL 3
#endif

// Any case ouput
[[maybe_unused]] static optlog::ConstOptLog<&std::cout, VERBOSE_LEVEL, 0> log0;

// Output only when VERBOSE_LEVEL>0
[[maybe_unused]] static optlog::ConstOptLog<&std::cout, VERBOSE_LEVEL, 1> log1;
