SpelunkBots
===========

A C++ API that allows for bots to be written in the original 2009 version of Spelunky.
For more information, check out the SpelunkBots homepage at http://t2thompson.com/projects/spelunkbots/

# About this Fork

This fork allows us to run SpelunkBots on Linux.

## Dependencies

Since SpelunkBots is built using GameMaker 8, we are not able to run it on Linux
nativelly, therefore, [wine](https://www.winehq.org/) is our hero.

Having wine installed we will need to cross compile SpelunkBots Code.

A good option to do the job is [MXE](http://mxe.cc), since it uses an updated
version MinGW toolchain.

## Cross Compiling Code

First of all, clone [MXE](http://mxe.cc/#download) repo:

```bash
git clone https://github.com/mxe/mxe.git
```

Then, enter in MXE folder and install gcc cross compiler:

```bash
MXE_TARGETS='x86_64-w64-mingw32.static i686-w64-mingw32.static' make gcc
```

It will download some dependencies and install a gcc under `<mxe-path>/usr/bin/`
folder.

> It's a little bit slow to install all the dependencies, so, grab a coffee and
> wait.

### Compile DLL's

First of all, go to folder where you've installed MXE, then, export PATH to
access MXE's binaries:

```bash
cd <mxe-folder>
export PATH=$PATH:$(pwd)/usr/bin
```

Now, we have access to all MXE binaries. Remember that it's a temporary setup,
if you want to have it under your `$PATH` forever, add `export PATH` command to
your `.bashrc` (or `.zshrc` if using zsh), replacing `$(pwd)` with absolute path
to MXE installation.

Then, just go to `DLL Solution` folder and run `make` for each folder:

```bash
cd Bot ; make ; cd ../Spelunkbots ; make ; cd ..
```

Now, copy all DLL's to source folder:

```bash
cp ./**/*.dll ../Source/spelunky_1_1/
```

### Running Game Maker

To run Game Maker, go to Game Maker folder and type:

```bash
wine Game_Maker.exe
```
