set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 寻找Qt5库
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

# 设置UI文件搜索路径
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/res/ui)

# 查找源文件
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

# 添加资源文件
file(GLOB RESOURCES ${CMAKE_SOURCE_DIR}/res/rc/*)


# 添加可执行文件
# add_executable(${PROJECT_NAME}  ${SOURCES} ${RESOURCES}) #debug
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${RESOURCES}  ${MY_VERSIONINFO_RC}) # release

include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME}
PUBLIC
# 	_DEBUG
# 	WIN32_LEAN_AND_MEAN
# 	_WINSOCK_DEPRECATED_NO_WARNINGS
    NOLFS
    _CRT_SECURE_NO_WARNINGS
    _WINSOCK_DEPRECATED_NO_WARNINGS
)


# 为target添加头文件
target_include_directories(${PROJECT_NAME}
PUBLIC
)

# 为target添加库文件目录
target_link_directories(${PROJECT_NAME}
PUBLIC
)

# 为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    "Winscard.lib"
    "libcrypto32MD.lib"
    "libssl32MD.lib"
    "Crypt32.lib"   
    "ws2_32.lib"
    "zel.lib"
    "xhlanguage.lib"
    "xlnt.lib"
)
