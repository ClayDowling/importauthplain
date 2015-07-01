# importauthplain

Utility to import Dokuwiki authplain text files into an authsqlite database

I'm writing this in C, which makes me ridiculously uncool.  Only
troglodytes and fossils write in C.  But I've been writing code
for 20 years, and I've gotten quite comfortable with C.  For 
something straight forward like this, C is also the fastest and
easiest solution.

## Tools

First, you need a C development environment on the system where 
you intend to build and run this program.  If you're on a UNIX
derived system like Linux or BSD you should already have it 
installed.

The easiest way to tell is to type:

  gcc --version

If you get version information back, you're ready to rock and roll.
If you don't, try installing gcc using your system's package
management tools.

If you're on OSX you'll need to install your development tools
via the app store.  Best of luck.

If you're on windows, best of luck.  I like the MinGW-w64 
package.  Microsoft also distributes their compiler for free,
but they use their own build system and I haven't included 
the necessary build scripts for it.

## Building

The program is simple to build:

  make

That will build a binary for your system.  If you don't wind up 
with a file called "importauthplain" or "importauthplain.exe"
something went wrong.  Fill an issue on github and include the
output from your compiler.


