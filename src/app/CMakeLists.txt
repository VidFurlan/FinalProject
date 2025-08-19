set(APP_SRC
	main.cpp
	Application.cpp
	Application.hpp
)

add_executable(${PROJECT_NAME} ${APP_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE engine renderer)
