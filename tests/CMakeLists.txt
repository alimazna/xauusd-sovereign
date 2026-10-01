cmake_minimum_required(VERSION 3.20)
project(XAUUSD_Sovereign LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(XAUUSD_BUILD_UI "Build the Dear ImGui desktop UI" ON)

include(FetchContent)

if (XAUUSD_BUILD_UI)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b-docking
        GIT_SHALLOW TRUE
    )
    FetchContent_GetProperties(imgui)
    if (NOT imgui_POPULATED)
        FetchContent_Populate(imgui)
    endif()

    FetchContent_Declare(
        glad
        URL https://github.com/Dav1dde/glad/archive/refs/tags/v0.1.36.zip
    )
    FetchContent_GetProperties(glad)
    if (NOT glad_POPULATED)
        FetchContent_Populate(glad)
    endif()

    add_library(glad STATIC
        ${glad_SOURCE_DIR}/src/glad.c
    )
    target_include_directories(glad PUBLIC
        ${glad_SOURCE_DIR}/include
    )

    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC glfw)
    target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_link_libraries(imgui PUBLIC glad)
endif()

set(FOUNDATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/contracts/foundation)
if (NOT EXISTS ${FOUNDATION_DIR})
    message(FATAL_ERROR "Missing src/contracts/foundation.")
endif()

file(GLOB FOUNDATION_SOURCES CONFIGURE_DEPENDS "${FOUNDATION_DIR}/*.cpp")
list(FILTER FOUNDATION_SOURCES EXCLUDE REGEX ".*Tests\\.cpp$")
list(FILTER FOUNDATION_SOURCES EXCLUDE REGEX ".*IntegrationTests\\.cpp$")

add_library(xauusd_foundation STATIC ${FOUNDATION_SOURCES})
target_include_directories(xauusd_foundation PUBLIC ${FOUNDATION_DIR})
target_compile_features(xauusd_foundation PUBLIC cxx_std_20)

if (XAUUSD_BUILD_UI)
    file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp")
    find_package(OpenGL REQUIRED)

    add_executable(xauusd_ui ${UI_SOURCES})
    target_include_directories(xauusd_ui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${FOUNDATION_DIR}
    )
    target_compile_features(xauusd_ui PRIVATE cxx_std_20)
    target_link_libraries(xauusd_ui PRIVATE xauusd_foundation imgui glfw glad OpenGL::GL)

    if (WIN32)
        target_compile_definitions(xauusd_ui PRIVATE NOMINMAX)
    endif()

    if (MSVC)
        target_compile_options(xauusd_ui PRIVATE /W4)
    else()
        target_compile_options(xauusd_ui PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

include(CTest)
enable_testing()

set(PHASE_TESTS
    Phase1IntegrationTests
    Phase2IntegrationTests
    Phase3IntegrationTests
    Phase4IntegrationTests
    Phase5IntegrationTests
    Phase6IntegrationTests
    Phase7IntegrationTests
    Phase8IntegrationTests
    Phase10IntegrationTests
    Phase11IntegrationTests
)

foreach(test_name IN LISTS PHASE_TESTS)
    if(EXISTS "${FOUNDATION_DIR}/${test_name}.cpp")
        add_executable(${test_name} "${FOUNDATION_DIR}/${test_name}.cpp")
        target_include_directories(${test_name} PRIVATE ${FOUNDATION_DIR})
        target_compile_features(${test_name} PRIVATE cxx_std_20)
        target_link_libraries(${test_name} PRIVATE xauusd_foundation)
        if (MSVC)
            target_compile_options(${test_name} PRIVATE /W4)
        else()
            target_compile_options(${test_name} PRIVATE -Wall -Wextra -Wpedantic)
        endif()
        add_test(NAME ${test_name} COMMAND ${test_name})
    endif()
endforeach()

file(GLOB UNIT_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp")
file(GLOB INTEGRATION_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/*.cpp")
file(GLOB E2E_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/end_to_end/*.cpp")

set(ALL_NEW_TEST_SOURCES
    ${UNIT_TEST_SOURCES}
    ${INTEGRATION_TEST_SOURCES}
    ${E2E_TEST_SOURCES}
)

foreach(test_source IN LISTS ALL_NEW_TEST_SOURCES)
    get_filename_component(test_name "${test_source}" NAME_WE)
    add_executable("${test_name}" "${test_source}")
    target_include_directories("${test_name}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        "${FOUNDATION_DIR}"
    )
    target_compile_features("${test_name}" PRIVATE cxx_std_20)
    target_link_libraries("${test_name}" PRIVATE xauusd_foundation)
    if (MSVC)
        target_compile_options("${test_name}" PRIVATE /W4)
    else()
        target_compile_options("${test_name}" PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME "${test_name}" COMMAND "${test_name}")
endforeach()

# -----------------------------------------------------------------------------
# MT5 Integration (C++ side only; MQL5 files are deployed to MetaTrader 5)
# -----------------------------------------------------------------------------
set(MT5_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/mt5/cpp)

add_library(xauusd_mt5 STATIC
    ${MT5_CPP_DIR}/MT5Protocol.cpp
    ${MT5_CPP_DIR}/MT5Server.cpp
    ${MT5_CPP_DIR}/MT5MessageParser.cpp
    ${MT5_CPP_DIR}/MT5MessageBuilder.cpp
    ${MT5_CPP_DIR}/MT5AdapterManager.cpp
    ${MT5_CPP_DIR}/MT5Integration.cpp
)

target_include_directories(xauusd_mt5 PUBLIC
    ${MT5_CPP_DIR}
    ${FOUNDATION_DIR}
)
target_compile_features(xauusd_mt5 PUBLIC cxx_std_20)
target_link_libraries(xauusd_mt5 PUBLIC xauusd_foundation)

if(WIN32)
    target_link_libraries(xauusd_mt5 PUBLIC ws2_32)
endif()

if(MSVC)
    target_compile_options(xauusd_mt5 PRIVATE /W4)
else()
    target_compile_options(xauusd_mt5 PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(test_mt5_protocol tests/mt5/test_mt5_protocol.cpp)
target_link_libraries(test_mt5_protocol PRIVATE xauusd_mt5)
target_compile_features(test_mt5_protocol PRIVATE cxx_std_20)
add_test(NAME test_mt5_protocol COMMAND test_mt5_protocol)

add_executable(test_mt5_server tests/mt5/test_mt5_server.cpp)
target_link_libraries(test_mt5_server PRIVATE xauusd_mt5)
target_compile_features(test_mt5_server PRIVATE cxx_std_20)
add_test(NAME test_mt5_server COMMAND test_mt5_server)

add_executable(test_mt5_integration tests/mt5/test_mt5_integration.cpp)
target_link_libraries(test_mt5_integration PRIVATE xauusd_mt5)
target_compile_features(test_mt5_integration PRIVATE cxx_std_20)
add_test(NAME test_mt5_integration COMMAND test_mt5_integration)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if (WIN32)
    find_package(CURL REQUIRED)
    target_link_libraries(xauusd_foundation PUBLIC CURL::libcurl)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBCURL REQUIRED IMPORTED_TARGET libcurl)
    target_link_libraries(xauusd_foundation PUBLIC PkgConfig::LIBCURL)
endif()
add_test(NAME test_mt5_mql5_static
         COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/mt5/test_mql5_static.py)


set_tests_properties(test_telegram_config PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
