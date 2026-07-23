cmake_minimum_required(VERSION 3.19)
project(Xefexcel LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.9.3/msvc2022_64")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Xefexcel
    #WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    fileExplorer.cpp
    fileExplorer.h

    XMLParser.cpp
    XMLParser.h

    XMLData.h
)

target_link_libraries(Xefexcel
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Xefexcel
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Xefexcel
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
