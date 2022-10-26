[![build & test (clang, gcc, MSVC)](https://github.com/kamchatka-volcano/figcone_shoal/actions/workflows/build_and_test.yml/badge.svg?branch=master)](https://github.com/kamchatka-volcano/figcone_shoal/actions/workflows/build_and_test.yml)

**figcone_shoal** - is a [`shoal`](https://shoal.eelnet.org/) configuration parser for [`figcone`](https://github.com/kamchatka-volcano/figcone) library. 


## Installation
Download and link the library from your project's CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(figcone_shoal
    GIT_REPOSITORY "https://github.com/kamchatka-volcano/figcone_shoal.git"
    GIT_TAG "origin/master"
)
#uncomment if you need to install figcone_shoal with your target
#set(INSTALL_FIGCONE_SHOAL ON)
FetchContent_MakeAvailable(figcone_shoal)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE figcone::figcone_shoal)
```

For the system-wide installation use these commands:
```
git clone https://github.com/kamchatka-volcano/figcone_shoal.git
cd figcone_shoal
cmake -S . -B build
cmake --build build
cmake --install build
```

Afterwards, you can use find_package() command to make the installed library available inside your project:
```
find_package(figcone_shoal 0.1.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE figcone::figcone_shoal)
```

## Running tests
```
cd figcone_shoal
cmake -S . -B build -DENABLE_TESTS=ON
cmake --build build
cd build/tests && ctest
```

## License
**figcone_shoal** is licensed under the [MS-PL license](/LICENSE.md)  
