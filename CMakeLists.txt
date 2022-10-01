cmake_minimum_required(VERSION 3.20)

project(figcone_shoal VERSION 0.2.0 DESCRIPTION "shoal parser for figcone library")
include(GNUInstallDirs)
include(external/seal_lake)

SealLake_Download(
        GIT_REPOSITORY https://github.com/microsoft/GSL
        GIT_TAG        v4.0.0
        DESTINATION    include/figcone_shoal/detail/external
        DIRECTORIES    include/gsl
        TEXT_REPLACEMENTS
            "namespace gsl" "namespace figcone::shoal::gsl"
            "gsl::details" "figcone::shoal::gsl::details"
            "#include <gsl/assert>" "#include \"assert\""
            "GSL_" "FIGCONE_SHOAL_GSL_"
)

set(INSTALL_FIGCONE_TREE ON)
SealLake_Import(
        figcone_tree 0.11.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        cmake_maintenance
)

SealLake_HeaderOnlyLibrary(
        NAMESPACE figcone
        COMPILE_FEATURES cxx_std_17
        LIBRARIES figcone::figcone_tree
        DEPENDENCIES
            figcone_tree 0.11.0
)
SealLake_OptionalBuildSteps(tests)
