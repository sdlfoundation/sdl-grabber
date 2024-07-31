# ================================
# ============ Setup =============

cmake_minimum_required(VERSION 3.29.6)
set(CMAKE_CXX_STANDARD 23)
project(sdl-grabber C CXX)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(sdl-grabber ${SOURCES})

# ================================

# ================================
# ============= WUT ==============

wut_create_rpx(sdl-grabber)

# ================================

# ================================
# ========= Dependencies =========

if (NOT TARGET wups)
    add_subdirectory(Dependencies/wups)
endif()

if (NOT TARGET macchiato)
    add_subdirectory(Dependencies/macchiato)
endif()

# ================================

# ================================
# =========== Linkage ============

target_include_directories(sdl-grabber PRIVATE Source Dependencies/macchiato/Include)
target_link_libraries(sdl-grabber wups wut macchiato)

# Link with the recreated symbol table of the game.
# set_target_properties(sdl-grabber PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/Linkerscripts/Silverlight.ld)
# target_link_options(sdl-grabber PRIVATE -T${CMAKE_SOURCE_DIR}/Linkerscripts/Silverlight.ld)
#
# set_property(
#         DIRECTORY
#         APPEND
#         PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Linkerscripts/Silverlight.ld
# )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/sdl-grabber.rpx"
    DESTINATION "${CMAKE_INSTALL_PREFIX}")

# ================================
