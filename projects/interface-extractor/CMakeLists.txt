cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR          ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_SETUP_SRC_DIR    ${CMAKE_CURRENT_LIST_DIR}/../setup/src)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/Palette.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/setup-utils.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/interface-extractor.cpp
)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pch.cpp
)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/BinaryFileReader.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/Image.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/Palette.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/setup-utils.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/interface-extractor.h
)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_INTERFACE_EXTRACTOR_PRECOMPILED_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/pch.h
)

# Creates the target
add_executable(interface-extractor
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_SOURCES}
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_INCLUDE_HEADERS}
)

target_include_directories(interface-extractor PUBLIC
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_INCLUDE_DIRS}
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_EXTERNAL_DIRS}
)

target_link_libraries(interface-extractor PRIVATE
    project_options
    project_warnings
)

target_precompile_headers(interface-extractor PRIVATE
    ${OPEN_RIVAL_INTERFACE_EXTRACTOR_PRECOMPILED_HEADERS}
)
