cmake_minimum_required(VERSION 3.18)

project(figcone_shoal VERSION 0.1.1 DESCRIPTION "shoal parser for figcone library")
include(GNUInstallDirs)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FIGCONE_SHOAL_SUBPROJECT OFF)
else()
    set(FIGCONE_SHOAL_SUBPROJECT ON)
endif()

include(external/gsl)
find_package(figcone_tree 0.10.1 QUIET)
if (NOT figcone_tree_FOUND)
    message("Configuration info: figcone_tree library wasn't found on your system, downloading it from the GitHub repository")
    include (external/figcone_tree)
endif()

add_library(figcone_shoal INTERFACE)
add_library(figcone::figcone_shoal ALIAS figcone_shoal)

target_compile_features(figcone_shoal INTERFACE cxx_std_17)
target_include_directories(
        figcone_shoal
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_include_directories(
    figcone_shoal
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/figcone_shoal/detail/external>
)

target_include_directories(
    figcone_shoal
    INTERFACE
    $<BUILD_INTERFACE:${gsl_SOURCE_DIR}/include/>
)
target_link_libraries(figcone_shoal INTERFACE figcone::figcone_tree)

option(ENABLE_TESTS "Enable tests" OFF)
if (ENABLE_TESTS AND NOT FIGCONE_SHOAL_SUBPROJECT)
    enable_testing()
    add_subdirectory(tests)
endif()

option(INSTALL_FIGCONE_SHOAL "Install figcone_shoal library unconditionally" OFF)
if (NOT FIGCONE_SHOAL_SUBPROJECT OR INSTALL_FIGCONE_SHOAL)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/figcone_shoal DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(DIRECTORY ${gsl_SOURCE_DIR}/include/gsl         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/figcone_shoal/detail/external)

    set(FIGCONE_SHOAL_PACK_PATH ${CMAKE_INSTALL_LIBDIR}/cmake/figcone_shoal)
    install(TARGETS figcone_shoal
            EXPORT figcone_shoal-targets
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/figcone_shoal)
    install(EXPORT figcone_shoal-targets
            FILE figcone_shoalTargets.cmake
            NAMESPACE figcone::
            DESTINATION ${FIGCONE_SHOAL_PACK_PATH})

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_shoalConfigVersion.cmake
            COMPATIBILITY SameMajorVersion
            ARCH_INDEPENDENT
    )

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/figcone_shoalConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_shoalConfig.cmake
            INSTALL_DESTINATION ${FIGCONE_SHOAL_PACK_PATH}
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_shoalConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_shoalConfigVersion.cmake
            DESTINATION ${FIGCONE_SHOAL_PACK_PATH}
    )
endif()
