cmake_minimum_required(VERSION 3.10)

# Имя проекта и версия
project(Server VERSION 1.0)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Настройка Qt
find_package(Qt5 COMPONENTS Core Network REQUIRED)

# Включаем автоматическую обработку moc-uic-rcc файлов
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Исходники проекта
set(SOURCES
    main.cpp
    socketserver.cpp
)

# Заголовочные файлы
set(HEADERS
    socketserver.h
)

# Добавление исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Линковка с Qt5
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)

# Пути к дополнительным библиотекам и заголовочным файлам
if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/OpenSSL/include")
    target_link_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/OpenSSL/lib")
    target_link_libraries(${PROJECT_NAME} ssl crypto)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE "/home/sysadm/qt_project/openssl/include/openssl")
    target_link_directories(${PROJECT_NAME} PRIVATE "/home/sysadm/qt_project/openssl/lib64")
    target_link_libraries(${PROJECT_NAME} ssl crypto dl pthread)
endif()

# Специфичные зависимости для разных платформ и конфигураций
# Интерфейс
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Interface/Interface)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../Interface/Interface)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} optimized Interface debug Interface_d)
else()
    target_link_libraries(${PROJECT_NAME} Interface)
endif()

# Контроль
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Control)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../Control)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} optimized Control debug Control_d)
else()
    target_link_libraries(${PROJECT_NAME} Control)
endif()
