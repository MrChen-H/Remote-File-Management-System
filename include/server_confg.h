#pragma once
//该文件包含所有外部库头和需要的标准库头
//以及服务器各项配置
#include <unistd.h>
#include <fcgiapp.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include<fcntl.h>
#include<fastdfs/fdfs_client.h>
#include<errno.h>
#include<sys/types.h>
#include<wait.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
#include "mysql/mysql.h"
#include "json.hpp"
#include <fcgi_stdio.h>
// Log 级别
enum LEVEL
{
    TRACE_LOG,
    DEBUG_LOG,
    INFO_LOG,
    WARN_LOG,
    ERROR_LOG,
    CRITICAL_LOG
};

namespace Server_Conf
{
    const std::string DB_NAME("Your DataBase Name");//数据库名
    const std::string DB_PASSWORD("Your MySQL User Password");//数据库密码
    const std::string DB_USER("Your MySQL User Name");//用户名

    const std::string SERVER_IP("Your Server IP");//服务器IP
    const std::string FDFS_CLIENT_CONF("/etc/fdfs/client.conf");                       // Fast_DFS 的客户端配置文件路径
    const std::string FDFS_STORAGE_PATH("/home/fast_dfs/storage/Storage_Node_1/data"); // Fast_DFS 的储存节点路径

    // 取消共享的CGI日志路径
    const std::string PATH_CANCEL_SHARED_LOG("/usr/local/nginx/logs/CGI_Log/Cancel_Shared_File/cancel_shared_file.log");

    // 删除文件的CGI日志路径
    const std::string PATH_DELETE_FILE_LOG("/usr/local/nginx/logs/CGI_Log/Delete_File/delete_file.log");

    // 下载文件的CGI日志路径
    const std::string PATH_DOWNLOAD_FILE_LOG("/usr/local/nginx/logs/CGI_Log/DownLoad_File/download_file.log");

    // 登录校验的CGI日志路径
    const std::string PATH_LOGIN_CHECK_LOG("/usr/local/nginx/logs/CGI_Log/Login_Check/login_check.log");

    // MD5校验的CGI日志路径
    const std::string PATH_MD5_CHECK_LOG("/usr/local/nginx/logs/CGI_Log/Md5_Check/md5_check.log");  

    //文件共享的CGI日志路径                    
    const std::string PATH_SHARED_FILE_LOG("/usr/local/nginx/logs/CGI_Log/Shared_File/shared_file.log");

    //文件上传的CGI日志路径
    const std::string PATH_UPLOAD_FILE_LOG("/usr/local/nginx/logs/CGI_Log/DFS_Upload/dfs.log");

    //用户注册的CGI日志路径
    const std::string PATH_USER_REGISTER_LOG("/usr/local/nginx/logs/CGI_Log/Register/Register_Log.log");

    //数据库操作的日志路径
    const std::string PATH_DB_LOG("/usr/local/nginx/logs/Data_Base_Log/db.log");
};