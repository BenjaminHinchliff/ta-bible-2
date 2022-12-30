# Totally Accurate Bible Quote Generator v2.0

A small (not simple) program to generate arbitrary quotes from the bible (King James version) by abusing ellipses.

```bash
❯ ./build/ta-bible-2 fish were created as the best living creatures
Indexing bible...
Building fragment tree...
Scoring fragment tree...
"fish...were created...as the...best...living creatures" (John21:13, Rev4:11, Rev20:8, 1Cor12:31, Eze3:13)
```

###### If you're curious, the much simpler v1.0 is [here](https://github.com/BenjaminHinchliff/totally-accurate-bible-quotes)

## Building

Single source file, only requires a c++17 compliant compiler (and stl). A CMake build file is included, for convenience,
but since it's just one source file it doesn't really need to be used.

### CMake example

```bash
# Setup CMake build dir in release mode
❯ cmake -DCMAKE_BUILD_TYPE=Release -Bbuild
# Make
❯ make -C build
# Run executable
❯ ./build/ta-bible-2 fish were created as the best living creatures
```

## Usage

Simply pass the words to be constructed from bible quotes on the command line, as single or multiple arguments.

Separate arguments will always be interpreted as having a single space between them.

The quote itself is printed to `stdout` and any status or error messages go to `stderr`.

```bash
❯ ./ta-bible-2 [TARGET WORDS]
```

## The Algorithm Behind It
TODO: write this part
