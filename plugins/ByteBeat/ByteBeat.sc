ByteBeat : UGen {
    *ar {
        ^this.multiNew('audio');
    }

    checkInputs {
        ^this.checkValidInputs;
    }
}
