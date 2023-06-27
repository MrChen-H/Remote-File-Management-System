#pragma once
#include "server_confg.h"
#include"mysql/mysql.h"
#include"make_log.h"

/**
 * @brief 数据插入
 *
 * @param data_base_command 数据库操作命令
 * @return 成功返回0 失败返回-1
 */
int DB_Insert(const char *data_base_command)
{

    if (spdlog::get("Data_base") == nullptr) // 如果不存名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Data_base",Server_Conf::PATH_DB_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }
    auto db_log = spdlog::get("Data_base");

    MYSQL *connect = NULL;
    connect = mysql_init(NULL); // 初始化链接

    connect = mysql_real_connect(connect, Server_Conf::SERVER_IP.c_str(), Server_Conf::DB_USER.c_str(), Server_Conf::DB_PASSWORD.c_str(), Server_Conf::DB_NAME.c_str(), 3306, NULL, 0); // 链接数据库
    if (connect == NULL)
    {
        db_log->error("MySQL connect faild:{}", mysql_error(connect));
        return false;
    }

    mysql_query(connect, "set names utf8"); // 设置utf8

    int ret = mysql_query(connect, data_base_command); // 进行数据库操作
    if(ret != 0)
    {
        db_log->error("data insert failed: {}", mysql_error(connect));
        return -1;
    }
    mysql_close(connect);
    return 0;
}

/**
 * @brief 该函数用于数据库查询
 * 
 * @param command 查询命令
 * @param res 返回的结果,二维数组
 * 
 * @returns
 * 成功 true 失败false,如果要判断是否有查询结果,判断一下res是否为NULL即可
*/
bool Select_Data(const char* command,std::vector<std::vector<std::string>> &res)
{
    if (spdlog::get("Data_base") == nullptr) // 如果不存名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Data_base", Server_Conf::PATH_DB_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }
    auto db_log = spdlog::get("Data_base");

    MYSQL *connect = NULL;
    connect = NULL;
    connect = mysql_init(NULL); // 初始化链接
    connect = mysql_real_connect(connect, Server_Conf::SERVER_IP.c_str(), Server_Conf::DB_USER.c_str(), Server_Conf::DB_PASSWORD.c_str(), Server_Conf::DB_NAME.c_str(), 3306, NULL, 0); // 链接数据库
    if (connect == NULL)
    {
        db_log->info( "MySQL connect faild:{}", mysql_error(connect));
        return false;
    }
    mysql_query(connect, "set names utf8"); // 设置utf8
    int ret = mysql_query(connect, command); // 进行数据库操作
    if (ret != 0)
    {
        db_log->info("MySQL Select faild:{}", mysql_error(connect));
        return false;
    }
    MYSQL_RES *result = mysql_store_result(connect);//获取结果集
    
    MYSQL_ROW res_row;

    if (result == NULL)
    {
        db_log->info("Select Data Failed:{}",mysql_error(connect));
        return true; // 查询结果集为空
    }
    while (res_row = mysql_fetch_row(result))
    {
        std::vector<std::string> temp_vec;
        for (int i = 0; i < result->field_count; i++)
        {
            temp_vec.push_back(res_row[i]);
        }
        res.push_back(temp_vec);
    } // 返回结果

    mysql_free_result(result);
    mysql_close(connect);
    
    return true;
}

/**
 * @brief 数据库数据插入
 * @param db_command 插入命令
 * @param reply_info 回复的提示信息
 * 
 * @returns 成功 true 失败false
*/
bool Insert_Data(char * db_command,std::string &reply_info)
{
    if (spdlog::get("Data_base") == nullptr) // 如果不存名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Data_base", Server_Conf::PATH_DB_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }
    auto db_log = spdlog::get("Data_base");
    MYSQL *connect = NULL;
    connect = NULL;
    connect = mysql_init(NULL);                                                                          // 初始化链接
    connect = mysql_real_connect(connect, Server_Conf::SERVER_IP.c_str(), Server_Conf::DB_USER.c_str(), Server_Conf::DB_PASSWORD.c_str(), Server_Conf::DB_NAME.c_str(), 3306, NULL, 0); // 链接数据库
    if (connect == NULL)
    {
        reply_info = mysql_error(connect);
        db_log->error("MySQL connect faild: {}", mysql_error(connect));
        return false;
    }
    int sta = mysql_query(connect, db_command);
    if (sta != 0)
    {
        reply_info = mysql_error(connect);
        db_log->error("MySQL insert faild: {}", mysql_error(connect));
        return false;
    }
    mysql_close(connect);
    return true;
}

/**
 * @brief 数据库数据删除
 * @param db_command 插入命令
 *
 * @returns 成功 true 失败false
 */
bool Delete_DB_Data(const char *db_command)
{
    if (spdlog::get("Data_base") == nullptr) // 如果不存名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Data_base", Server_Conf::PATH_DB_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }
    auto db_log = spdlog::get("Data_base");
    MYSQL *connect = NULL;
    connect = NULL;
    connect = mysql_init(NULL);                                                                          // 初始化链接
    connect = mysql_real_connect(connect, Server_Conf::SERVER_IP.c_str(), Server_Conf::DB_USER.c_str(), Server_Conf::DB_PASSWORD.c_str(), Server_Conf::DB_NAME.c_str(), 3306, NULL, 0); // 链接数据库
    if (connect == NULL)
    {
        db_log->error("MySQL connect faild: {}", mysql_error(connect));
        return false;
    }
    int sta = mysql_query(connect, db_command);
    if (sta != 0)
    {
        db_log->error("MySQL Delete faild: {}", mysql_error(connect));
        return false;
    }
    mysql_close(connect);
    return true;
}

/**
 * @brief 对 MySQL 执行事务操作的函数(需要多条语句原子操作时使用)
 * @param command {const char*}首项提交的事务命令
 * @param commit_to_transaction_commands 可变参数,需要提交的各项命令,函数将会按照传参顺序依次提交至MySQL,至少两条命令参数
 *
 * @return 当函数执行成功返回 true,执行失败会回滚事务状态并返回 false
 */
template <class... Arg>
bool DB_transaction(const char *command,Arg... commit_to_transaction_commands)
{
    const char *get_arg[sizeof...(commit_to_transaction_commands) + 1] = {command, commit_to_transaction_commands...};//获取所有参数

    if (spdlog::get("Data_base") == nullptr) // 如果不存名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Data_base",Server_Conf::PATH_DB_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }
    auto db_log = spdlog::get("Data_base");

    //当形参包没有参数传入时断言失败并抛出信息
    static_assert(sizeof...(commit_to_transaction_commands) >= 1, "Arguments commit_to_transaction_commands must be have at least 1 argument!");

    MYSQL *connect = NULL;
    connect = NULL;
    connect = mysql_init(NULL);                                                                          // 初始化链接
    connect = mysql_real_connect(connect, Server_Conf::SERVER_IP.c_str(), Server_Conf::DB_USER.c_str(), Server_Conf::DB_PASSWORD.c_str(), Server_Conf::DB_NAME.c_str(), 3306, NULL, 0); // 链接数据库
    if (connect == NULL)
    {
        db_log->error("MySQL connect faild: {}", mysql_error(connect));
        return false;
    }

    mysql_query(connect, "BEGIN");//开始事务

    for (int i = 0; i < sizeof...(commit_to_transaction_commands) + 1; i++) // 依次提交事务
    {
        int sta = mysql_query(connect, get_arg[i]);
        if (sta != 0)//有某条语句执行失败则回滚
        {
            mysql_rollback(connect);
            db_log->error("MySQL execute faild transaction: {}", mysql_error(connect));
            mysql_close(connect);
            return false;
        }
        
    }

    int sta = mysql_commit(connect);//提交事务
    if (sta != 0)//提交失败回滚
    {
        db_log->error("MySQL commit faild: {}", mysql_error(connect));
        mysql_rollback(connect);
        mysql_close(connect);
        return false;
    }
    try
    {
        /* code */
        mysql_close(connect);
    }
    catch(const std::exception& e)
    {
        db_log->error("MySQL connect already closed.Don't close it again");
    }
       
    return true;
}