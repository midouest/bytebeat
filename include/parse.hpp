#pragma once

#include "ast.hpp"

#include <string>

using namespace std;

namespace bb
{

AstPtr parse(const string &tokens);

} // namespace bb
