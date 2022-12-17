project(test_figcone_shoal)

set(SRC
        test_paramparser.cpp
        test_paramlistparser.cpp
        test_nodeparser.cpp
        test_nodelistparser.cpp
)

SealLake_GoogleTest(
        SOURCES ${SRC}
        COMPILE_FEATURES cxx_std_17
        INCLUDES ../src
        PROPERTIES
            CXX_EXTENSIONS OFF
        LIBRARIES figcone::figcone_shoal
)
