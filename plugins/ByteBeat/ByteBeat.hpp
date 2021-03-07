#pragma once

#include <SC_PlugIn.hpp>

#include "interpreter.hpp"

namespace ByteBeat
{
/**
 * ByteBeat is able to parse simple mathematical expressions and evaluate
 * them to produce audio samples.
 *
 * ByteBeat will not produce any audio output initially. The /eval unit
 * command must be used to send expressions to the synth to be parsed. Once
 * an expression has been parsed, it will become the active expression and
 * begin producing audio samples.
 *
 * ByteBeat expects a single audio-rate input, "t", that is passed to the
 * expression.
 */
class ByteBeat : public SCUnit
{
public:
    ByteBeat();

    /**
     * Parse the incoming expression and replace the existing expression.
     * Does not replace the existing expression if the incoming expression
     * cannot be parsed.
     */
    void parse(const char *input);

private:
    /**
     * Evaluate the current bytebeat expression for the given number of
     * samples.
     */
    void next(int nSamples);

    bb::Interpreter mInterpreter;
};
} // namespace ByteBeat
