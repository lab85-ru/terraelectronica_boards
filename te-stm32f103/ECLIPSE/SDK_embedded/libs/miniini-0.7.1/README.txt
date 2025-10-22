MiniINI version 0.7.1


== Introduction ==

MiniINI is a a free/open source, minimalistic, simple to use C++ library for 
reading ini (aka cfg) files. It has no dependencies (that might change in the 
future, but it is unlikely) and should compile on any platform with a standards
compliant compiler. (only a makefile for GCC is included at the moment, though)

Main priority of MiniINI is, as its name suggests, minimalism and speed. Its 
goal is to create fastest ini library possible supporting the most common ini 
features without sacrificing ease of use. MiniINI should be especially useful 
for game development, for instance game settings, properties of units in 
strategy games, etc. MiniINI is already very fast. It takes under 70 ms to load,
parse and unload an INI file about 1MB large with 512 sections according to 
benchmarks, even with debug build. This is on a ~1.7 Ghz dual-core machine 
(although MiniINI can only use one core).

Debug build of MiniINI can also check ini files it reads, and issue warnings for 
the most common mistakes in ini code. This should be useful mainly for users of 
programs based on MiniINI, for instance game modders. 

At the moment, MiniINI can only read ini files, not write to them. This should 
change in future versions.

You can get newest versions of MiniINI at https://launchpad.net/miniini .

=== INI file format ===

INI files are text files containing simple values formatted as tag=value pairs,
for example:
```
answer=42
```
These are grouped in ini sections which start by headers containing names in 
square brackets, for example:
```
[section]
```
Optionally, ini files can contain comments, which are ignored. Comments usually
start by a semicolon (default in MiniINI) or hash and continue to the end of 
line. Example:
```
tag=value ;comment
```
A simple complete ini file example:
```
[General]
OS=Linux
Version=0.4
Renderers=OpenGLRenderer, SWRenderer
CurrentRenderer=OpenGLRenderer

[OpenGLRenderer]
ScreenWidth=1024
ScreenHeight=768
UseShaders=true
```

== Features ==

- Reads most common ini/cfg files, or from user provided buffer containing 
  contents of an ini/cfg file. This allows the user e.g. to load data from a 
  compressed file and pass it to MiniINI.
- Well documented both with API documenation and tutorials.
- Can read headerless ini/cfg files. Stores all tags before the first (if any)
  header in a default section.
- Reads only plain ASCII files. There is no support for Unicode, and there 
  probably never will be.
- Extremely fast. Even multi-MB ini files with hundreds or thousands of sections
  can be processed in fractions of a second.
- Case-sensitive. That means that [CASE], [case] and [Case] are not the same 
  and there is a difference between Tag= and TAG=
- Provides methods to read strings, ints, floats and bools from ini files and 
  checks the ini data for errors, allowing the programmer to use their own
  default values. 
- Can read arrays of data from tags with multiple, comma separated values.
- In debug build, can issue warnings to the user using a callback function 
  specified by the programmer. For example when a tag from which the program 
  tries to load an int contains something else.
- Can also read arrays of data from numbered sequences of tags, for example:
                  +
                  +
  a1=1            +
  a2=2            +
  a3=3            +
                  +
- Supports both plain strings and arrays and STL strings and vectors. STL
  functionality can be disabled for more minimalism.
- Cannot write to ini files at this time. This should be implemented in future.
- Ignores *all* spaces, i.e. no spaces/tabs in tags or values. For example:    +
                  +
  tag=125685      +
  and             +
  t a g = 125 685 +
                  +
  both have the same meaning. Spaces _might_ be supported in the future for values, 
  if there will be a need, but are not planned at the moment.
- Supports single line comments with a *configurable* comment character. So if 
  you want to use *#* instead of *;* , you can. There is no support for multiple 
  comment characters at the same time for performance reasons.
- Name=value separator is configurable as well, '=' by default.
- Does _not_ support multiline comments, like C /* */ comments. This is planned
  to be implemented in future.



== New in this release ==

MiniINI 0.7 finally comes with multiple value tag support, and some other 
changes.

Tags with multiple, comma separated values can now be read into arrays.
A new family of INISection methods, ReadMultiXXX was added to read multi value
tags. If ReadXXX methods are called on a multi-value tag, the first value is
read.

A bug that could cause a segfault when allocator allocated a new block was 
fixed.

Some refactoring of internal code.

Improvements in API documentation, warnings and regression tester.

Using txt2tags for documentation now.

For more details, see CHANGES.txt .

MiniINI 0.7.1, a bugfix release, removed the unusable INISection operator [] .
                                                  

== Compiling/Installing/Using ==

=== Directory structure ===
MiniINI files are split into multiple directories:
```
 ./         (top-level directory) MiniINI include file and this readme.

 ./miniini  MiniINI source code        

 ./bin      Compiled binary files, e.g. the library itself.

 ./doc      Documentation for MiniINI.

 ./example  Example MiniINI programs.

 ./test     Files for MiniINI regression tester and benchmark scripts.
```  
      

=== Requirements ===
MiniINI should compile on any standards compliant C++ compiler, and requires no
third-party libraries. MiniINI also requires C99 support. If you have gcc, there 
is a GNU makefile tested with gcc 4.x , which probably also works with older gcc 
versions.

MiniINI also includes some benchmarking scripts, which require Python and 
Valgrind to run. However, these are not necessary to compile or use MiniINI
and are mainly useful for MiniINI development.

=== Compiling/Installing with gcc ===
Type *make* in terminal.
Then type *make install* as a root user 
(e.g. *sudo make install* if you have sudo)
This will compile and copy optimized MiniINI build to
*/usr/lib/libminiini.a* , debug build to */usr/lib/libminiini-dbg.a* ,
and copy miniini headers to */usr/include/.

Debug and optimized versions can also be compiled separately using
*make debug* and *make optimized*

You can also uninstall MiniINI by typing *make uninstall* as a root user.

=== Using MiniINI ===
First, you will need to include miniini.h header file and link with
either debug or optimized miniini build.
To include MiniINI, add this line to your code:
```
#include<miniini.h>
```
To link with MiniINI using GCC, add this to your compiler options:
```
-lminiini 
```
for optimized or
```
-lminiini-dbg 
```
for debug build.

If you didn't install MiniINI, you'll also need to add path with miniini.h to 
header paths of your compiler and path with libminiini.a or libminiini-dbg.a to 
linker paths.

For more info you can check reference manual and tutorials in *./doc* directory 
and examples in *./example* directory.

== License ==

MiniINI is free/open source software distributed under the MIT/X11 license.
This license allows you to use MiniINI in both open and closed source software,
free of charge, and modify it if you need to.

Full text of the license:

```
Copyright (c) 2009-2010 Ferdinand Majerech

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
```


== Contact/Credits ==
MiniINI homepage at Launchpad: https://launchpad.net/miniini

MiniINI was created by Ferdinand Majerech aka Kiith-Sa kiithsacmp[AT]gmail.com

MiniINI was created using Vim and GCC on Ubuntu Linux, as an ini reading 
library for Catom https://launchpad.net/catom .

