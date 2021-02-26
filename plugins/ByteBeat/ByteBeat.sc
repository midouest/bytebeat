ByteBeat : UGen {
    *ar { arg t;
        ^this.multiNew('audio', t);
    }

    checkInputs {
        ^this.checkValidInputs;
    }
}
