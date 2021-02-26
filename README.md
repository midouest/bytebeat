# bytebeat

A [bytebeat](https://arxiv.org/abs/1112.1368) interpreter and [SuperCollider](https://supercollider.github.io) plugin.

## Building

Requires:

- [CMake 3.7+](https://cmake.org)
- [SuperCollider Source](https://github.com/supercollider/supercollider)
- [Catch2](https://github.com/catchorg/Catch2) (on macOS, `brew install catch2`)

```
$ cmake -Bbuild -H. \
  -DSC_PATH=/path/to/supercollider \
  -DCMAKE_INSTALL_PREFIX=/path/to/extensions \
  -DCMAKE_BUILD_TYPE=RELEASE
$ cmake --build build/ --target test_bytebeat
$ ./build/test_bytebeat
$ cmake --build build/ --target install
```

`/path/to/supercollider` is the path to the root of your SuperCollider source installation.

`/path/to/extensions` is the path to your SuperCollider user extensions directory.

- On macOS, this is `~/Library/ApplicationSupport/SuperCollider/Extensions`.

## Expression Syntax

- One expression only
- Only one pre-defined variable: `t`
- Mathematic operators: ​`(), +, -, *, /, %`
- Bitwise operators: ​`&, |, ^, <<, >>`
- Relational operators: `<, >, <=, >=, ==, !=`

## SuperCollider Usage

Initially, the UGen will not produce any audio. A `ByteBeatController` instance
should be used to wrap the synth containing the `ByteBeat` UGen. The `eval`
instance method can then be used to send a bytebeat expression string to the
UGen to be parsed and evaluated.

```
(
SynthDef.new(\bytebeat, {
    var t = PulseCount.ar(Impulse.ar(8000));
    Out.ar(0, ByteBeat.ar(t).dup)
}).add;
)

b = ByteBeatController(Synth.new(\bytebeat), 2);
b.eval("((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7");
```

## Command Line Usage

Requires:

- [SoX](http://sox.sourceforge.net) (on macOS, `brew install sox`)

```
$ ./bytebeat "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7" | head -c 8000000 > crowd.raw
$ sox -r 8000 -c 1 -t u8 crowd.raw crowd.wav
```

## Benchmarks

Host: 13-inch M1 MacBook Pro (2020)

```
...............................................................................

benchmark name                       samples       iterations    estimated
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
parse crowd                                    100             5     1.7205 ms
                                        4.06055 us    3.81938 us    4.37245 us
                                        1.38736 us    1.14322 us    1.68179 us

eval crowd                                     100           615     1.6605 ms
                                        27.3658 ns    27.3508 ns     27.382 ns
                                      0.0791974 ns  0.0684235 ns  0.0937675 ns
```

Host: Raspberry Pi 3 Model B+

```
...............................................................................

benchmark name                       samples       iterations    estimated
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
parse crowd                                    100             3    20.427 ms
                                          68.03 us     67.885 us    68.377 us
                                          1.098 us        485 ns     1.907 us

eval crowd                                     100           557   15.1504 ms
                                            281 ns        279 ns       284 ns
                                             13 ns          9 ns        18 ns
```
