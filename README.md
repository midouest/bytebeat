# bytebeat

A [bytebeat](https://arxiv.org/abs/1112.1368) interpreter

## Building

Requires:

- [Catch2](https://github.com/catchorg/Catch2) (on macOS, `brew install catch2`)

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./test_bytebeat
```

## Usage Example

Requires:

- [SoX](http://sox.sourceforge.net) (on macOS, `brew install sox`)

```
> ./bytebeat "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7" | head -c 8000000 > crowd.raw
> sox -r 8000 -c 1 -t u8 crowd.raw crowd.wav
```

## Expression Syntax

- One expression only
- Only one pre-defined variable: `t`
- Mathematic operators: ​`(), +, -, *, /, %`
- Bitwise operators: ​`&, |, ^, <<, >>`
- Relational operators: `<, >, <=, >=, ==, !=`
