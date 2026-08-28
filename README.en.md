# candyLover

A Discord bot with a bunch of stuff:
* [Not implemented] Spam trap
* [Partial] Number count
* [Not implemented] Word chain
* [Not implemented] Word blacklist
* Maybe more?

Most has a reset command.

## Installation

At the moment this is not implemented.
But as long as used libraries are still available (matching version and full path), copy `build/soc` to any directory visible in PATH environment variable and you're done.

## Compiling

Needs:
1. Git
2. [Concord](https://github.com/Cogmasters/concord/tree/dev)'s requirements
3. Lua 5.x (maybe 5.4+?)
4. Discord bot token and application ID
5. Compiler that supports [C23](https://cppreference.com/c/23)
6. GNU Make
7. [Bear](https://github.com/rizsotto/bear) to generate compile_commands.json for IDE auto-complete, errors and more

Clone this repo:

```
git clone --recursive https://github.com/lebao3105/candyLover.git
# If you have GitHub CLI:
gh repo clone lebao3105/candyLover -- --recursive
# After one of the commands above:
cd candyLover
```

Build:

```
make all
# Same as above
make soc
# Build and generate compile_commands.json (only need this once unless you modify Makefile / add new files)
make compile_commands
# Clean build/
make clean
# Clean build/ and dependencies
make deepclean
```

The binary is always `build/soc`.

## Setup

This bot uses Lua to setup stuff - no auto reloading is implemented.

Only some Lua modules are loaded (subject to change?):
* Base
* String
* Math
* Table

Modules can be found starting from [6.2sec](https://www.lua.org/manual/5.5/manual.html#6.2).

By default the bot loads ./env.lua. If you don't want to, use --env-path / -e flag.

### Global functions

* `include(path)`: Immediately load and runs `path`;
* Functions from loaded modules above;
* Section starters below;
* `end_section()` ends the current section. Not required.

### Sections

There are *[REDACTED]* sections:
* environment: Set tokens, channel IDs, and maybe more;
* count: Number counter;
* trapboy: Spam trap;
* wordchain: Word chain
