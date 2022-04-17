cmake_minimum_required(VERSION 3.13)
project(sdrpp_dsp)

# Main code
file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

# Add code to dyn lib
add_executable(sdrpp_dsp ${SRC} ${BACKEND_SRC})

# Set compiler options
if (MSVC)
    target_compile_options(sdrpp_dsp PRIVATE /std:c++17 /EHsc) # /O2 /Ob2
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(sdrpp_dsp PRIVATE -O3 -std=c++17)
else ()
    target_compile_options(sdrpp_dsp PRIVATE -O3 -std=c++17)
endif ()


# Include core headers
target_include_directories(sdrpp_dsp PUBLIC "src/")

if (MSVC)
    # Lib path
    target_link_directories(sdrpp_dsp PUBLIC "C:/Program Files/PothosSDR/lib/")

    # Misc headers
    target_include_directories(sdrpp_dsp PUBLIC "C:/Program Files/PothosSDR/include/")

    # Volk
    target_link_libraries(sdrpp_dsp PUBLIC volk)

    # FFTW3
    find_package(FFTW3f CONFIG REQUIRED)
    target_link_libraries(sdrpp_dsp PUBLIC FFTW3::fftw3f)

else()
    find_package(PkgConfig)

    pkg_check_modules(FFTW3 REQUIRED fftw3f)
    pkg_check_modules(VOLK REQUIRED volk)

    target_include_directories(sdrpp_dsp PUBLIC
        ${FFTW3_INCLUDE_DIRS}
        ${VOLK_INCLUDE_DIRS}
    )
    
    target_link_directories(sdrpp_dsp PUBLIC
        ${FFTW3_LIBRARY_DIRS}
        ${VOLK_LIBRARY_DIRS}
    )

    target_link_libraries(sdrpp_dsp PUBLIC
        ${FFTW3_LIBRARIES}
        ${VOLK_LIBRARIES}
    )

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(sdrpp_dsp PUBLIC stdc++fs pthread)
    endif ()

endif ()

# cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"