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
- Integers (+/-), strings
- Mathematic operators: ​`(), +, -, *, /, %`
- Bitwise operators: ​`&, |, ^, <<, >>, ~`
- Relational operators: `<, >, <=, >=, ==, !=, !`
- Array subscript operator: `[]`
- Ternary if operator: `?:`

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
parse crowd                                    100             7     1.8942 ms
                                        2.82587 us    2.79468 us    2.97907 us
                                        305.722 ns    7.57781 ns    729.356 ns

eval crowd                                     100           456     1.6872 ms
                                        36.8993 ns    36.8727 ns    36.9713 ns
                                       0.208576 ns  0.0980819 ns   0.445346 ns
```

Host: Raspberry Pi 3 Model B+

```
...............................................................................

benchmark name                       samples       iterations    estimated
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
parse crowd                                    100            6   15.3318 ms
                                         25.808 us    25.268 us    26.657 us
                                          3.385 us     2.349 us     4.555 us

eval crowd                                     100          347   15.3027 ms
                                            440 ns       432 ns       453 ns
                                             49 ns        32 ns        68 ns
```
