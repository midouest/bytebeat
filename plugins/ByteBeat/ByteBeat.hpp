#pragma once

#include <SC_PlugIn.hpp>

#include "expression.hpp"

namespace ByteBeat
{
    /**
     * ByteBeat is able to parse simple mathematical expressions and evaluate
     * them to produce audio samples.
     *
     * ByteBeat will not produce any audio output initially. The /setexpr unit
     * command must be used to send expressions to the synth to be parsed. Once
     * an expression has been parsed, it will become the active expression and
     * begin producing audio samples.
     *
     * Expressions produce samples using a monotonically increasing time
     * counter. The counter can be reset to t0 using the /restart unit command.
     */
    class ByteBeat : public SCUnit
    {
    public:
        ByteBeat();
        ~ByteBeat();

        /**
         * Parse the incoming expression and replace the existing expression.
         * Does not replace the existing expression if the incoming expression
         * cannot be parsed.
         */
        void setExpression(const char *input);

        /**
         * Reset the bytebeat time counter to zero.
         */
        void restart();

    private:
        /**
         * Evaluate the current bytebeat expression for the given number of
         * samples.
         */
        void next(int nSamples);

        /** Time counter passed to the bytebeat expression */
        int mTime = 0;

        /** bytebeat expression used to generate audio samples */
        bb::Expression *mExpression;
    };
}
