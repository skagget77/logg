# Logg
Logg is a tiny C++ logging framework built on top of std::basic_ostream. The frameworks main goals are:

  * Minimize the use of C++ preprocessor macros. Heavy use of macros can lead to hard to read code and difficult to solve build problems. Logg uses macros only when absolutely necessary.
  * Zero overhead for log requests with log levels higher than the set global log level, i.e. a DEBUG log request should have zero overhead if the global log level is set to INFO. Using Logg it's possible to add trace and debug calls to performance critical code in a near standard way and still ship release builds with no overhead.
  * Near zero overhead when logging compared to logging directly to a std::basic_ostream. Logg perform no heap allocations as part of a log request.

These goals are realized by using template partial specialization and function overloading instead of the more traditional way of relying on the C++ preprocessor.

All that is needed to use Logg in a module is a log stream derived from std::basic_ostream. Using Logg and the log stream client code can log at different predefined levels, e.g. ERROR, DEBUG, TRACE. The predefined log levels are open for extension and it's easy to create custom log levels allowing for finer grained control of what get's logged and when.

## Macros
In a perfect world Logg would not need to use any macros at all. Unfortunately there are some things in C++ that cannot be done expect through the use of macros. For Logg these things are:

  * Build time configuration.
  * Source code location, filename and line number.
  * Function name.

Because of this Logg uses a small set of well documented macros, all paremeterless and evaluates to at most a single line of source code.

### Macros Used for Configuration
The configuration macros can be used by client code to perform build time configuration of Logg. Note that for the configuration macros to have any effect they must be defined before including _<logg/logg.h>_. See the examples section further down for different ways to do this.

#### LOGG_LOG_LEVEL
Sets the global log level for the build. The value must be a constexpr and evaluate to an unsiged int. If not defined the default setting is logg::ALL for debug builds and logg::ERROR for release builds. NDEBUG is used to determine build type.

#### LOGG_DISABLE_ALIASES
Disables definition of shorter to type aliases for frequently used macros. By default Logg defines aliases for some frequently used macros, i.e. LOGG_SOURCE and LOGG_FUNCTION. However, there is a small chance that these shorter names will collide with names in other frameworks/libraries. 

### Macros Used for Logging
The logging macros are used when instatiating a logger to get additional information about the source code location where the logging takes place.

### LOGG_FUNCTION
Gets the name and signature of the function enclosing the current scope. If the scope is outside of any function _top-level_ is returned. Aliases defined, unless disabled:

  * LGFUNC
  * lgfunc

### LOGG_SOURCE
Gets the source filename and line number. Aliases defined, unless disabled:

  * LGSRC
  * lgsrc

## Examples
The examples below and additional examples are available in the example folder in the root of the repository. Binaries for the examples are built as part of the normal build process using CMake.

### Basic Usage
Using Logg's default configuration to log a TRACE message to the translation unit's global log stream.
```C++
#include <logg/logg.h>

namespace {
  std::ostream& log = std::cout;
}

int add_two_integers(int a, int b) {
  logg::trace(log) << "a=" << a << ", b=" << b;
  return a + b;
}

int main() {
  auto r = add_two_integers(2, 3);
  std::cout << "r=" << r << std::endl;
}
```

Running the example using a debug build will result in the following output.
```Bash
$ examples/basic
2018-04-16 12:58 [12489] TRACE - a=2, b=3
r=5
```

Running the same example but using a release build will output the following.
```Bash
$ examples/basic
r=5
```

Logg's default configuration set the log level to logg::ALL for debug builds and logg::ERROR for release builds. So for release builds only log requests with log level FATAL or ERROR will be logged.

### Basic Usage with Source Location Macros
Using LOGG_FUNCTION or LOGG_SOURCE, or one of their aliases, pin-points where in the source code the log request took place.
```C++
#include <logg/logg.h>

namespace {
  std::ostream& log = std::cout;
}

int add_two_integers(int a, int b) {
  logg::trace(log, lgfunc) << "a=" << a << ", b=" << b;
  auto r = a + b;
  logg::trace(log, lgsrc) << "r=" << r;
  return r;
}

int main() {
  auto r = add_two_integers(2, 3);
  std::cout << "r=" << r << std::endl;
}
```

Running the example using a debug build will result in the following output.
```Bash
$ examples/basic
2018-04-16 12:58 [12489] TRACE {int add_two_integers(int, int)} - a=2, b=3
2018-04-16 12:58 [12489] TRACE {location.cpp:10} - r=5
r=5
```

When using LOGG_FUNCTION at the global scope, i.e. outside of any function, it will be logged as _{top-level}_.

### Compound Log Messsge
Sometimes it makes sense to break down a log request into mutliple smaller log requests all providing part of the information for the log message. This can be achieved by holding a constant reference to the instantiated logger.
```C++
#include <logg/logg.h>

namespace {
  std::ostream& log = std::cout;
}

int add_two_integers(int a, int b) {
  const auto& trace = logg::trace(log, lgsrc);
  trace << "a=" << a << ", b=" << b;
  auto r = a + b;
  trace << ", r=" << r;
  return r;
}

int main() {
  auto r = add_two_integers(2, 3);
  std::cout << "r=" << r << std::endl;
}
```

Running the example using a debug build will result in the following output.
```Bash
$ examples/basic
2018-04-16 12:58 [12489] TRACE {composite.cpp:8} - a=2, b=3, r=5
r=5
```

Note that source location provided by LOGG_FUNCTION will be source line where logger was instantiated and no the line where log requests where created.

### Custom Log Levels
The default log levels provided by Logg can easily be extended if needed. Anything evaluating to an unsigned int can be used as a custom log level. Note that the custom log level needs to be passed as a template parameter.
```C++
#include <logg/logg.h>

enum my_custom_levels {
  FOO = 42,
  BAR = 314,
};

int add_two_integers(std::ostream& log, int a, int b) {
  logg::log<FOO>(log) << "a=" << a << ", b=" << b;
  auto r = a + b;
  logg::log<BAR>(log) << "r=" << r;
  return r;
}

int main() {
  auto r = add_two_integers(std::cout, 2, 3);
  std::cout << "r=" << r << std::endl;
}
```

Running the example using a debug build will result in the following output.
```Bash
$ examples/basic
2018-04-16 12:58 [12489] CUSTOM(42) - a=2, b=3
2018-04-16 12:58 [12489] CUSTOM(314) - r=5
r=5
```

Running the same example but using a release build will output the following. FOO (42) is lower that the global log level (log::ERROR = 100) and gets written to the log.
```Bash
$ examples/basic
2018-04-16 12:58 [12489] CUSTOM(42) - a=2, b=3
r=5
```

### Configuration
There are essentially three different ways to configure Logg:
  * Don't do it at all, i.e I'm happy with the default settings.
  * Do the configuration using the build system, e.g. CMake.
  * Do the configuration using the source code.

The first bullet is pretty straight forward, don't do any configuration at all, just use it. Of the two remaining bullets the second is more flexible, it allows multiple binaries with different log levels to be created without modifying the source code, e.g. for a continious integration system. The last one is a good option if there is no build system or if different build systems are used for different platforms.

#### Custom Configuration Using CMake
The following CMake snippet allow us to pass the global log level to the CMake command on the command line. CMake in turn will pass it to the C++ compiler during the build. If a log level is not passed on the CMake command line the default will be used.
```CMakeLists.txt
if(DEFINED LOGG_LOG_LEVEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOGG_LOG_LEVEL=${LOGG_LOG_LEVEL}")
endif()
```

Setting the global log level for the build to INFO.
```Bash
cmake -DLOGG_LOG_LEVEL=logg::INFO ..
```

It should be quite straight forward to port this example to other build systems.

#### Custom Configuration Using Source Code
To ensure that the same global log level is used for the entire code base it's recommended that a local header file is created which defines the configuration and includes _<logg/logg.h>_. The code base then include this local header file. The example local header file below sets the global log level to ALL for debug builds and INFO for release builds.
```C++
#if defined(NDEBUG)
#define LOGG_LOG_LEVEL logg::INFO
#else
#define LOGG_LOG_LEVEL logg::ALL

#include <logg/logg.h>
```

## Inspecting Binary Code with Objdump
The example called optimization has been used to compare code generated by the compiler for release builds. The function called _fac1_ does no logging. The function called _fac2_ does logging at the INFO level. Below we compare code when LOGG_LOG_LEVEL has been set to OFF, i.e. when the compiler should optmize away any calls to Logg. The following command line has been used to disassemble the binaries.
```Bash
$ objdump -M intel -C -d example/optimization
```

Here is _fac1_, note that vectorization has been turned off to make the disassembly a bit easier to read.
```Asm
00000000000009d0 <fac1(int)>:
 9d0:  83 ff 02               cmp    edi,0x2
 9d3:  7e 1b                  jle    9f0 <fac1(int)+0x20>
 9d5:  ba 02 00 00 00         mov    edx,0x2
 9da:  b8 01 00 00 00         mov    eax,0x1
 9df:  90                     nop
 9e0:  0f af c2               imul   eax,edx
 9e3:  83 c2 01               add    edx,0x1
 9e6:  39 d7                  cmp    edi,edx
 9e8:  75 f6                  jne    9e0 <fac1(int)+0x10>
 9ea:  f3 c3                  repz ret
 9ec:  0f 1f 40 00            nop    DWORD PTR [rax+0x0]
 9f0:  b8 01 00 00 00         mov    eax,0x1
 9f5:  c3                     ret
 9f6:  66 2e 0f 1f 84 00 00   nop    WORD PTR cs:[rax+rax*1+0x0]
 9fd:  00 00 00
```

Here is _fac2_.
```Asm
0000000000000a00 <fac2(int)>:
 a00:   83 ff 02               cmp    edi,0x2
 a03:   b8 01 00 00 00         mov    eax,0x1
 a08:   7e 16                  jle    a20 <fac2(int)+0x20>
 a0a:   ba 02 00 00 00         mov    edx,0x2
 a0f:   90                     nop
 a10:   0f af c2               imul   eax,edx
 a13:   83 c2 01               add    edx,0x1
 a16:   39 fa                  cmp    edx,edi
 a18:   75 f6                  jne    a10 <fac2(int)+0x10>
 a1a:   f3 c3                  repz ret
 a1c:   0f 1f 40 00            nop    DWORD PTR [rax+0x0]
 a20:   f3 c3                  repz ret
 a22:   66 2e 0f 1f 84 00 00   nop    WORD PTR cs:[rax+rax*1+0x0]
 a29:   00 00 00
```
As we can fac1 is almost identical to fac2. The only that differs is that the compiler has rearranged some instructions. This means that all calls to Logg has been optimized away.

Here is a snipped of _fac2_ when LOGG_LOG_LEVEL has been set to INFO. As we can see Logg has been almost entirely inlined and adds very little extra work on top of using standard streams directly.
```Asm
0000000000000e40 <fac2(int)>:
 ...
 e4e:   48 81 ec 88 00 00 00   sub    rsp,0x88
 e55:   48 89 e5               mov    rbp,rsp
 e58:   48 89 ef               mov    rdi,rbp
 e5b:   e8 90 02 00 00         call   10f0 <logg::detail::build_header(char*, unsigned int)>
 e60:   ba 80 00 00 00         mov    edx,0x80
 e65:   48 8d 35 98 03 00 00   lea    rsi,[rip+0x398]     # 1204 <_IO_stdin_used+0x4>
 e6c:   29 c2                  sub    edx,eax
 e6e:   89 c0                  mov    eax,eax
 e70:   48 8d 7c 05 00         lea    rdi,[rbp+rax*1+0x0]
 e75:   e8 f6 fc ff ff         call   b70 <strncpy@plt>
 e7a:   48 89 ea               mov    rdx,rbp
 e7d:   8b 0a                  mov    ecx,DWORD PTR [rdx]
 e7f:   48 83 c2 04            add    rdx,0x4
 e83:   8d 81 ff fe fe fe      lea    eax,[rcx-0x1010101]
 e89:   f7 d1                  not    ecx
 e8b:   21 c8                  and    eax,ecx
 e8d:   25 80 80 80 80         and    eax,0x80808080
 e92:   74 e9                  je     e7d <fac2(int)+0x3d>
 e94:   89 c1                  mov    ecx,eax
 e96:   48 8d 3d 23 12 20 00   lea    rdi,[rip+0x201223]  # 2020c0 <std::cout@@GLIBCXX_3.4>
 e9d:   48 89 ee               mov    rsi,rbp
 ea0:   c1 e9 10               shr    ecx,0x10
 ea3:   a9 80 80 00 00         test   eax,0x8080
 ea8:   0f 44 c1               cmove  eax,ecx
 eab:   48 8d 4a 02            lea    rcx,[rdx+0x2]
 eaf:   89 c3                  mov    ebx,eax
 eb1:   48 0f 44 d1            cmove  rdx,rcx
 eb5:   00 c3                  add    bl,al
 eb7:   48 83 da 03            sbb    rdx,0x3
 ebb:   48 29 ea               sub    rdx,rbp
 ebe:   e8 0d fd ff ff         call   bd0 <std::__ostream_insert<char, ...>(..., char const*, ...)@plt>
 ec3:   48 8d 35 43 03 00 00   lea    rsi,[rip+0x343]     # 120d <_IO_stdin_used+0xd>
 eca:   48 8d 3d ef 11 20 00   lea    rdi,[rip+0x2011ef]  # 2020c0 <std::cout@@GLIBCXX_3.4>
 ed1:   ba 02 00 00 00         mov    edx,0x2
 ed6:   e8 f5 fc ff ff         call   bd0 <std::__ostream_insert<char, ...>(..., char const*, ...)@plt>
 edb:   48 8d 3d de 11 20 00   lea    rdi,[rip+0x2011de]  # 2020c0 <std::cout@@GLIBCXX_3.4>
 ee2:   44 89 e6               mov    esi,r12d
 ee5:   e8 36 fd ff ff         call   c20 <std::ostream::operator<<(int)@plt>
 ...
 102b:c3                       ret
```

## Limitations
The current implementation is thread-safe but does not synchronize writes to the underlaying log stream. This will be addressed in a future release.

## License
Logg is licensed under the MIT license. Please see the LICENSE file in the root of the repository.
