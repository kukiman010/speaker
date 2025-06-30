cmake_minimum_required(VERSION 3.10)  # или выше, в зависимости от потребностей

# Имя проекта и язык программирования
project(Control LANGUAGES CXX)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Список исходных файлов
set(SOURCES
#    abstractcommand.cpp
#    abstractobject.cpp
    command.cpp
    socket.cpp
    sslworker.cpp
)

# Список заголовочных файлов
set(HEADERS
#    abstractcommand.h
#    abstractobject.h
    command.h
    saved_container.h
    socket.h
    sslworker.h
)


# Найти Qt5 или Qt6 пакеты (используем предпочтительный, либо альтернативный)
find_package(Qt5 COMPONENTS Core Network QUIET)
if (Qt5_FOUND)
    set(QT_PACKAGE Qt5)
else()
    find_package(Qt6 COMPONENTS Core Network REQUIRED)
    set(QT_PACKAGE Qt6)
endif()


# Создаем статическую библиотеку
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})


# Ссылки на Qt библиотеки
target_link_libraries(${PROJECT_NAME} ${QT_PACKAGE}::Core ${QT_PACKAGE}::Network)


# Добавляем пути для заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Interface/Interface)


# Платформоспецифические настройки
if(WIN32)
    include_directories("C:/Program Files/OpenSSL/include")
    target_link_libraries(${PROJECT_NAME} "C:/Program Files/OpenSSL/lib/libssl.lib" "C:/Program Files/OpenSSL/lib/libcrypto.lib")
else()
    include_directories("/home/sysadm/qt_project/openssl/include/openssl")
    target_link_libraries(${PROJECT_NAME} "/home/sysadm/qt_project/openssl/lib64/libssl.so" "/home/sysadm/qt_project/openssl/lib64/libcrypto.so" dl pthread)
endif()


# Ссылка на библиотеку Interface
if(WIN32)
    if(MSVC)
        target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface/$<CONFIG>/Interface.lib)
    else()
        target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface/$<CONFIG>/libInterface.a)
    endif()
#else()
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface/libInterface.a)
endif()


# Обеспечиваем корректный порядок сборки
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface
)
if(WIN32 AND MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface/$<CONFIG>/Interface.lib
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface/libInterface.a
    )
endif()
