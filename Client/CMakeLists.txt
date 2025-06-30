cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Client)

# Определение и настройка конфигурации C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакетов Qt
find_package(Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)
#find_package(Qt6 COMPONENTS Core Gui Network Widgets REQUIRED)

# Список исходных и заголовочных файлов проекта
set(SOURCES
    main.cpp
    mainwindow.cpp
    socketclient.cpp
)

set(HEADERS
    mainwindow.h
    socketclient.h
)

set(FORMS
    mainwindow.ui
)

# Обработка .ui файлов с помощью Qt UIC
set(CMAKE_AUTOUIC ON)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS})

# Добавление общих define
add_definitions(-DQT_DEPRECATED_WARNINGS)
# Uncomment the line below if you want to disable deprecated APIs before specific version
# add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# Установка правильных Qt библиотеки в зависимости от найденной версии Qt
if (Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
elseif (Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets)
endif()

# Настройка правил для установки
if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "/tmp/${PROJECT_NAME}/bin")
elseif (UNIX AND NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "/opt/${PROJECT_NAME}/bin")
endif()
