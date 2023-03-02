### Installation

Currently the repository must be cloned or downloaded via the releases.

```git
git clone git+https://github.com/Dan-Tan/riot-cpp.git
```
Option 1: CMake

Linking to executable or library

```cmake
add_subdirectory(<path/to/riot-cpp/src>)
target_link_libraries(<your library or executable> <PRIVATE/PUBLIC> riot-cpp)
```

Option 2: Manually with libriot-cpp.

The shared object file is located as $riot-cpp/build/src/libriot-cpp.so$ and can be manually linked with your compiler of choice. You must include the following in your includes

```cpp
#include "path/to/riot-cpp/src/client/client.h"
```

See options for linking with gcc, [Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)

### Dependencies

The following libaries are reqired for usage:

 - C++ 20
 - [Jsoncpp](https://curl.se/libcurl/)
 - [libcurl](https://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html)

*To do: add version requirement information*
