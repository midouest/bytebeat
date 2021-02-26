#pragma once

#include <string>

#include "expression.hpp"

using namespace std;

namespace bb
{
    ExpressionPtr parse(string &input);
}
