
# download Openblas
if (FORCE_BUILD_32BITS)
    message("downloading OpenBLAS")
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/OpenBLAS.zip)
        file(DOWNLOAD "https://downloads.sourceforge.net/project/openblas/v0.2.19/OpenBLAS-v0.2.19-Win32.zip" ${CMAKE_SOURCE_DIR}/OpenBLAS.zip SHOW_PROGRESS EXPECTED_MD5 "cc29b41bc1fe41e8ef5ecb452e1cd70c")
        # file(DOWNLOAD "https://downloads.sourceforge.net/project/openblas/v0.2.14/OpenBLAS-v0.2.14-Win32.zip" ${CMAKE_SOURCE_DIR}/OpenBLAS.zip SHOW_PROGRESS EXPECTED_MD5 "eefdf170439620d78fabb3139b7aeb2f")
    endif()
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/mingw.zip)
        file(DOWNLOAD "https://sourceforge.net/projects/openblas/files/v0.2.14/mingw32_dll.zip" ${CMAKE_SOURCE_DIR}/mingw.zip SHOW_PROGRESS EXPECTED_MD5 "47f8b18b7b99ea0ca452bbfc4f6ef579")
    endif()
else()
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/OpenBLAS.zip)
        file(DOWNLOAD "https://downloads.sourceforge.net/project/openblas/v0.2.19/OpenBLAS-v0.2.19-Win64-int32.zip" ${CMAKE_SOURCE_DIR}/OpenBLAS.zip SHOW_PROGRESS EXPECTED_MD5 "7ff6092397a93494c137e23670dd72ec")
        # file(DOWNLOAD "https://downloads.sourceforge.net/project/openblas/v0.2.14/OpenBLAS-v0.2.19-Win64-int32.zip" ${CMAKE_SOURCE_DIR}/OpenBLAS.zip SHOW_PROGRESS EXPECTED_MD5 "bb59507959975d8d55f3e7eb0ecd5ea3")
    endif()
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/mingw.zip)
        file(DOWNLOAD "https://sourceforge.net/projects/openblas/files/v0.2.14/mingw64_dll.zip" ${CMAKE_SOURCE_DIR}/mingw.zip SHOW_PROGRESS EXPECTED_MD5 "e619f1f936638240472397e0f7970e66")
    endif()
endif()

# extract the openblas/mingw archives
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${CMAKE_SOURCE_DIR}/OpenBLAS.zip)
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${CMAKE_SOURCE_DIR}/mingw.zip)

file(GLOB OpenBLAS_DIR "${CMAKE_BINARY_DIR}/OpenBLAS*")
set(OpenBLAS_CMAKE_FLAGS -DOpenBLAS_DIR:PATH=${OpenBLAS_DIR})
