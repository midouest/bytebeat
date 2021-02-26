ByteBeat : UGen {
    *ar { arg t=0.0, mul=1.0, add=0.0;
        ^this.multiNew('audio', t).madd(mul, add);
    }

    checkInputs {
        if (inputs.at(0).rate != 'audio') {
            ^("t input is not audio rate: " + inputs.at(0) + inputs.at(0).rate);
        }
        ^this.checkValidInputs;
    }
}
