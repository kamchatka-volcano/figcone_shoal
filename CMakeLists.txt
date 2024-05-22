cmake_minimum_required(VERSION 3.18)

project(figcone_shoal VERSION 1.1.0 DESCRIPTION "shoal parser for figcone library")
include(external/seal_lake)

SealLake_IsInstallEnabled(INSTALL_FIGCONE_TREE ON)
SealLake_Import(
        figcone_tree 2.1.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        v2.1.0
)

SealLake_Bundle(
        NAME figcone_shoal_sfun
        GIT_REPOSITORY https://github.com/kamchatka-volcano/sfun.git
        GIT_TAG        v5.1.0
        TEXT_REPLACEMENTS
            "namespace sfun" "namespace figcone::shoal::sfun"
            SFUN_ FIGCONE_SHOAL_SFUN
)

SealLake_Bundle(
        NAME figcone_shoal_gsl
        GIT_REPOSITORY https://github.com/microsoft/GSL
        GIT_TAG        v4.0.0
        TEXT_REPLACEMENTS
            "namespace gsl" "namespace figcone::shoal::gsl"
            "gsl::details" "figcone::shoal::gsl::details"
            "#include <gsl/assert>" "#include \"assert\""
            GSL figcone_shoal_gsl
            figcone_shoal_gsl.natvis GSL.natvis
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
        LIBRARIES Microsoft.figcone_shoal_gsl::figcone_shoal_gsl figcone_shoal_sfun::figcone_shoal_sfun
        INTERFACE_LIBRARIES figcone::figcone_tree
        DEPENDENCIES
            figcone_tree 2.1.0
)

SealLake_OptionalSubProjects(tests)