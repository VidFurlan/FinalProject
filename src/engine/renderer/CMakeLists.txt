set(RENDERER_SRC
    renderer.cpp
    renderer.hpp
)

add_library(renderer STATIC ${RENDERER_SRC})
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(renderer PUBLIC engine)