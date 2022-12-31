# Totally Accurate Bible Quote Generator v2.0

A small (not simple) program to generate arbitrary quotes from the bible (King James version) by abusing ellipses.

##### Check out the online deployment [here](https://benjaminhinchliff.github.io/ta-bible-2/)

```bash
❯ ./ta-bible-2 fish were created as the best living creatures
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
❯ cd build && make
# Run executable
❯ ./ta-bible-2 fish were created as the best living creatures
```

## Usage

Simply pass the words to be constructed from bible quotes on the command line, as single or multiple arguments.

Separate arguments will always be interpreted as having a single space between them.

The quote itself is printed to `stdout` and any status or error messages go to `stderr`.

```bash
❯ ./ta-bible-2 [TARGET WORDS]
```

## How It Works

As I've designed it, the algorithm has 3 main parts:
1. Generate an index of each verse and word in the bible
2. Build a tree of the fragments of the target phrase, where each fragment is a quote from the bible
3. Score each combination of fragments, maximizing for longer subsections of quotes

The highest score after these parts is the final quote that is outputted.

### The Indexes

There are two indexs created inside `BibleIndex` - the word index and the verse index.
The word index is a multiple-key hashmap allowing the lookup of every verse that a particular word occurs in.
The verse index is simply a hashmap that allows the lookup of the content of a given verse. [^1]

### Fragment Generation

The target phrase is composed of quotes or "fragments" of individual verses. A fragment itself contains
only it's length in words of the target phrase and the verse it comes from, and the fragments that could continue it.

During the generation phrase, every fragment of every quote is built and turned into a tree.

### Scoring

This phrase finds the "best" quote by summing the squares of the length of the fragments in the quote.
This means something like "I am...a fish" would have a score of `2*2 + 2*2 = 8`, while "I am a...fish"
would have a score of `3*3 + 1*1 = 10`. As can be seen, this means that longer quotes are heavily 
preferred.

The current algorithm visits every combination to find the definitive "best" (highest-scoring), or maximum, however,
this is a place where an approximation (e.g. a greedy algorithm) could be used to massively improve performance for
longer targets, at the cost of perhaps not always producing the absolute "best" quote.

Since most quotes are relatively short though, and computers are quite fast, I decided this approach was sufficient.

After all this work is done, we have found the "best" quote.

[^1]: some nerd is definetly going to suggest using a trie to save memory here and it's true using one would improve that, however
  I chose a hashmap due to it already being in the C++ stl and having O(1) lookup performance (on a good day at least). In addition,
  at least on the King James Bible the memory usage isn't actually all that much by modern standards.
