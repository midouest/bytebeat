ByteBeat : UGen {
    *ar { arg t=0.0, mul=1.0, add=0.0;
        ^this.multiNew('audio', t, mul, add).madd(mul, add);
    }

    checkInputs {
        ^this.checkValidInputs;
    }
}
