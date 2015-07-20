# importauthplain

Utility to import Dokuwiki authplain text files into an authsqlite database

I'm writing this in C, which makes me ridiculously uncool.  Only troglodytes
and fossils write in C.  But I've been writing code for 20 years, and I've
gotten quite comfortable with C.  For something straight forward like this,
C is also the fastest and easiest solution.

## Tools

First, you need a C development environment on the system where you intend
to build and run this program.  If you're on a UNIX derived system like
Linux or BSD you should already have it installed.

The easiest way to tell is to type:

    gcc --version

If you get version information back, you're ready to rock and roll.  If you
don't, try installing gcc using your system's package management tools.

If you're on OSX you'll need to install your development tools via the app
store.  This should also get you git and CMake.

If you are on Windows you'll need to install a development toolkit.
Microsoft distributes their own C/C++ development tools for free.  If CMake
detects the Microsoft development tools it will generate appropriate
solution and project files to make a build.

## Building

The program uses cmake for build configuration.  Build order is something
like:

    mkdir build
    cd build
    cmake ..
    make
    make test

This should create an executable suitable for your system.  I have tested
this on Linux, Windows and OSX.  It should work on most UNIX like systems.

## Importing Data

Version 1.1 of importauthplain expects a database with a schema like the one
described in auth.sql from the authsqlite plugin distribution.  If you have
a different schema you will need to adjust the program as appropriate.

From your build folder:

    importauthplain -s /farm/cow/conf/users.auth.php
	-d /var/allusers/users.db -a cow.myfarm.org

In the event of a user or group name collision, importauthplain will attempt
to overwrite existing data, assuming that newer data is more accurate.

Please note that changing the animal you are importing for will not cause
group membership overwrites.  New group memberships for the new animal will
be created.
