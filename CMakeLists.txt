cmake_minimum_required(VERSION 3.20)

project(figcone_shoal VERSION 0.2.0 DESCRIPTION "shoal parser for figcone library")
include(GNUInstallDirs)
include(external/seal_lake)

set(INSTALL_FIGCONE_TREE ON)
SealLake_Import(
        figcone_tree 0.12.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        v0.12.0
)

SealLake_Bundle(
        NAME figcone_gsl
        GIT_REPOSITORY https://github.com/microsoft/GSL
        GIT_TAG        v4.0.0
        TEXT_REPLACEMENTS
            "namespace gsl" "namespace figcone::shoal::gsl"
            "gsl::details" "figcone::shoal::gsl::details"
            "#include <gsl/assert>" "#include \"assert\""
            GSL figcone_gsl
            figcone_gsl.natvis GSL.natvis
)

SealLake_ObjectLibrary(
        NAMESPACE figcone
        COMPILE_FEATURES cxx_std_11
        SOURCES
            src/parser.cpp
            src/nodeparser.cpp
            src/paramparser.cpp
            src/stream.cpp
            src/utils.cpp
        LIBRARIES Microsoft.figcone_gsl::figcone_gsl
        INTERFACE_LIBRARIES figcone::figcone_tree
        DEPENDENCIES
            figcone_tree 0.12.0
)

SealLake_OptionalBuildSteps(tests)