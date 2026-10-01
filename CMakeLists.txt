cmake_minimum_required(VERSION 3.20)
project(XAUUSD_Sovereign LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(XAUUSD_BUILD_UI "Build the Dear ImGui desktop UI" OFF)
# NOTE: the UI (ImGui+GLFW+glad) requires OpenGL/GLFW dev packages and Python 3.
# Enable it explicitly with -DXAUUSD_BUILD_UI=ON after installing them.
# The core engine, MT5 bridge, tests and CI build headless with the default OFF.

include(FetchContent)

if (XAUUSD_BUILD_UI)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)

    # Virtual glad population directories (generation happens below).
    set(glad_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/glad-src)
    set(glad_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/glad-build)
    file(MAKE_DIRECTORY ${glad_BINARY_DIR}/include/glad)
    file(MAKE_DIRECTORY ${glad_BINARY_DIR}/src)

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

    # The vendored glad package is the *generator tool* source tree; it does
    # not contain prebuilt src/glad.c + include/glad/glad.h headers, so we
    # generate the OpenGL 3.3 core loader at configure time using the
    # packaged Khronos XML spec (fully offline / reproducible).
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    set(GLAD_TOOL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad-0.1.36)
    set(GLAD_SPEC_XML ${GLAD_TOOL_DIR}/glad/files/gl.xml)
    if (NOT EXISTS ${GLAD_SPEC_XML})
        message(FATAL_ERROR "Missing vendored glad spec: ${GLAD_SPEC_XML}")
    endif()

    # Prefer a locally installed glad module; otherwise add the vendored
    # generator source tree to PYTHONPATH so `python -m glad` works anywhere.
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import glad"
        RESULT_VARIABLE GLAD_MODULE_PRESENT
        ERROR_QUIET OUTPUT_QUIET
    )
    if (GLAD_MODULE_PRESENT EQUAL 0)
        set(GLAD_PYTHONPATH "")
    else()
        set(GLAD_PYTHONPATH "PYTHONPATH=${GLAD_TOOL_DIR}")
    endif()

    if (WIN32)
        set(GLAD_RUNNER ${Python3_EXECUTABLE})
        set(GLAD_RUNNER_ARGS -m glad)
    else()
        # Pass the optional PYTHONPATH override through `sh -c`.
        set(GLAD_RUNNER sh)
        string(JOIN " " GLAD_ENV_STR ${GLAD_PYTHONPATH})
        set(GLAD_RUNNER_ARGS -c "${GLAD_ENV_STR} ${Python3_EXECUTABLE} -m glad")
    endif()

    execute_process(
        COMMAND ${GLAD_RUNNER} ${GLAD_RUNNER_ARGS}
                --generator c --profile core --api gl=3.3
                --out-path ${glad_BINARY_DIR} --reproducible --quiet
        WORKING_DIRECTORY ${GLAD_TOOL_DIR}
        RESULT_VARIABLE GLAD_GEN_RESULT
        OUTPUT_VARIABLE GLAD_GEN_OUTPUT
        ERROR_VARIABLE GLAD_GEN_ERROR
    )
    if (NOT GLAD_GEN_RESULT EQUAL 0 OR NOT EXISTS ${glad_BINARY_DIR}/src/glad.c
        OR NOT EXISTS ${glad_BINARY_DIR}/include/glad/glad.h)
        message(FATAL_ERROR
            "glad loader generation failed (result=${GLAD_GEN_RESULT}).\n"
            "stdout: ${GLAD_GEN_OUTPUT}\nstderr: ${GLAD_GEN_ERROR}")
    endif()

    add_library(glad STATIC
        ${glad_BINARY_DIR}/src/glad.c
    )
    target_include_directories(glad PUBLIC
        ${glad_BINARY_DIR}/include
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

# libcurl is required by TelegramGateway. Resolved here (before any target
# that links xauusd_foundation) so the dependency propagates to all tests,
# the MT5 bridge and the UI executable on every platform.
if (WIN32)
    # Prefer a vcpkg-style CURL package; fall back to the WinHTTP-backed
    # stub below if no curl development files are available.
    find_package(CURL QUIET)
endif()
if (NOT WIN32)
    # Resolve libcurl via pkg-config when available, otherwise fall back to the
    # standard FindCURL module (works on machines without pkg-config installed).
    find_package(PkgConfig QUIET)
    if (PkgConfig_FOUND)
        pkg_check_modules(LIBCURL IMPORTED_TARGET libcurl)
    endif()
    if (LIBCURL_FOUND)
        target_link_libraries(xauusd_foundation PUBLIC PkgConfig::LIBCURL)
    else()
        find_package(CURL QUIET)
        if (CURL_FOUND)
            target_link_libraries(xauusd_foundation PUBLIC CURL::libcurl)
        else()
            message(FATAL_ERROR
                "libcurl development files are required on Linux/macOS. "
                "Install them (e.g. apt-get install libcurl4-openssl-dev) or pass -DXAUUSD_TELEGRAM_STUB=ON.")
        endif()
    endif()
elseif (CURL_FOUND)
    target_link_libraries(xauusd_foundation PUBLIC CURL::libcurl)
else()
    message(STATUS "libcurl not found on Windows - building the WinHTTP-backed telegram transport stub")
    target_sources(xauusd_foundation PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/contracts/foundation/winhttp/winhttp_compat.cpp)
    target_include_directories(xauusd_foundation PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/contracts/foundation/winhttp)
    target_link_libraries(xauusd_foundation PUBLIC winhttp ws2_32 bcrypt advapi32)
endif()

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
        # Produce a single self-contained .exe on Windows: statically link the
        # GCC runtime so users do not need libgcc/libstdc++/winpthread DLLs.
        if (MINGW)
            target_link_options(xauusd_ui PRIVATE -static -static-libgcc -static-libstdc++)
        endif()
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
add_test(NAME test_mt5_mql5_static
         COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/mt5/test_mql5_static.py)


set_tests_properties(test_telegram_config PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
