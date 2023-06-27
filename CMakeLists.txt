CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(Server_CGI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")


if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CGI_bin)

add_executable(Cancel_Shared_File "${CMAKE_CURRENT_SOURCE_DIR}/src/cancel_shared_file.cpp")
add_executable(Delete_File "${CMAKE_CURRENT_SOURCE_DIR}/src/delete_file.cpp")
add_executable(DownLoad_File "${CMAKE_CURRENT_SOURCE_DIR}/src/DownLoad_File.cpp")
add_executable(Login_Check "${CMAKE_CURRENT_SOURCE_DIR}/src/Login_Cheak.cpp")
add_executable(Md5_Check "${CMAKE_CURRENT_SOURCE_DIR}/src/Md5_Cheack.cpp")
add_executable(Shared_File "${CMAKE_CURRENT_SOURCE_DIR}/src/shared_file.cpp")
add_executable(Up_Load_File "${CMAKE_CURRENT_SOURCE_DIR}/src/Up_Load_File.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/DFS_Upload.cpp")
add_executable(User_Register "${CMAKE_CURRENT_SOURCE_DIR}/src/User_Register.cpp")

target_link_libraries(Cancel_Shared_File PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(Delete_File PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(DownLoad_File PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(Login_Check PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(Md5_Check PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(Shared_File PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(Up_Load_File PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)
target_link_libraries(User_Register PRIVATE spdlog::spdlog$<$<BOOL:${MINGW}>:ws2_32>  fcgi fdfsclient mysqlclient)