#pragma once

#include <string>

#include "expression.hpp"

using namespace std;

namespace bb
{
    Expression *parse(string &input);
}
