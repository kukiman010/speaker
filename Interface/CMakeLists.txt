cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Interface)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка версии Qt
find_package(Qt5 COMPONENTS Core REQUIRED)
#find_package(Qt6 COMPONENTS Core REQUIRED)

# Определение целей
set(SOURCES
    generator.cpp
    abstractcommand.cpp
    scenario.cpp
    abstractobject.cpp
    ifactory.cpp
)

set(HEADERS
    singleton.h
    generator.h
    abstractcommand.h
    scenario.h
    scenarioid.h
    abiglobal.h
    abstractobject.h
    ifactory.h
    source.h
)

# Создание статической библиотеки
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# Список компиляторских предупреждений
target_compile_definitions(${PROJECT_NAME} PRIVATE QT_DEPRECATED_WARNINGS)

# Использование стандартов C++17
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

# Удаление GUI части из собранной qt
if (Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)
elseif (Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
endif()

# Дополнительные конфигурации
if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()
