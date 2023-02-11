# SQLite3 C++

A very straight forward C++ SQLite3 wrapper.

## Why use it?

In short: it makes your life easier.

Although the C SQLite API is fairly easy to use, it can get somewhat verbose.
This API allows you to interact with a SQLite database in an abstract and efficient way,
with error codes handling built in. 

## How to use it?

### Build it

Configure the project with cmake:

`> cmake -S . -B build`

Then simply build the thing:

`> cmake --build ./build`

Finally, just link `sqlite3` into whatever you need.

### The interface

An example of how one would use this interface is shown in [ex.cpp](examples/ex.cpp). This code's SOLE purpose is
to showcase the capabilities of the library and how to use it, it is NOT meant to show you 
how to build an application with it. For obvious security reasons, DO NOT USE the example as is 
in any sort of production environment!

If you need more detailed documentation, the headers have built-in doxygen style documentation
 and should provide you with all you need! 