#pragma once
#include "server_confg.h"
#include"File_Class.hpp"
#include"make_log.h"
#include "db_operation.h"
#include "time.h"
using json = nlohmann::json;

/**
 * @brief 检查数据完整性
 *
 * @param Get_Json_Data 获取到的Json格式数据
 * @param key_1 键名
 * @param keys  更多键名(如果有)
 * 
 * @return 当所有键都有对应的值时返回true,否则返回false 并写警告日志
 */
template <class... Args>
bool Check_Data_wholeness(json &Get_Json_Data, const char *key_1, Args... keys)
{
    if (spdlog::get("CGI_Log") == nullptr) // 如果不存在名为 Delete_File 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("CGI_Log", "/usr/local/nginx/logs/CGI_Log/CGI/cgi.log", 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto cgi_log = spdlog::get("CGI_Log"); // 获取名为 Delete_File 的日志管理器
    const char *Args_Arr[sizeof...(keys) + 1] = {key_1, keys...};//将所有参数加入数组

    bool ststue = true;//标志
    std::string error_key;//不存在的key
    std::string empty_key;//存在但没有值的key

    const char *Ok_Key[sizeof...(keys) + 1];//存在的key
    int ok_number = 0;//json 中 key 的数量

    for (int i = 0; i < sizeof...(keys) + 1; i++)//遍历所有key
    {
        if (std::string(Get_Json_Data.find(Args_Arr[i]).key()).empty())//key不存在
        {
            ststue = false;
            error_key += Args_Arr[i];//统计不存在的key方便写日志
            error_key += ' ';
            continue;
        }
        Ok_Key[ok_number++] = Args_Arr[i];//存在的key
    }
    for (int i = 0; i < ok_number; i++)
    {
        if (Get_Json_Data.find(Ok_Key[i]).value() == nullptr)//key 没有值
        {
            ststue = false;
            empty_key += Args_Arr[i];//统计没有值的key
            empty_key += ' ';
        }
    }
    if (ststue == false)//写日志并返回false
    {
        cgi_log->warn("Please check data:\n Key-> {} <-don't exits\n", error_key);
        cgi_log->warn(" Key-> {} <-is empty",empty_key);
        return false;
    }
    return true;
}

/**
 * URL 解码函数
 * @param str {const char*} 经URL编码后的字符串
 * @return 解码后的字符串，返回值不可能为空
 */
std::string acl_url_decode(const char *str)
{
    static unsigned char dec_tab[256] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        10,
        11,
        12,
        13,
        14,
        15,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        10,
        11,
        12,
        13,
        14,
        15,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    int len = (int)strlen(str);
    char *tmp = (char *)malloc(len + 1);

    int i = 0, pos = 0;
    for (i = 0; i < len; i++)
    {
        if (str[i] != '%')
            tmp[pos] = str[i];
        else if (i + 2 >= len)
        {                     /* check boundary */
            tmp[pos++] = '%'; /* keep it */
            if (++i >= len)
                break;
            tmp[pos] = str[i];
            break;
        }
        else if (isalnum(str[i + 1]) && isalnum(str[i + 2]))
        {
            tmp[pos] = (dec_tab[(unsigned char)str[i + 1]] << 4) + dec_tab[(unsigned char)str[i + 2]];
            i += 2;
        }
        else
            tmp[pos] = str[i];

        pos++;
    }

    tmp[pos] = '\0';
    std::string res(tmp);
    free(tmp);
    return res;
}

/**
    @brief 解析上传的数据,同时得到文件大小

    @param File           需要写入数据的文件对象
    @param data_len       数据长度
    @param data_stream    数据读取位置

    @returns
    0 succ, -1 fail
**/
int POST_Data_Get(File_Struct &File, size_t data_len, FCGX_Stream *data_stream,std::string &client_request_header)
{
    //参数处理 -----后面再补

    std::vector<std::string> data_arr;//存放数据的数组
    std::string one_line_data;//单行数据

    int index = 0;//数据总行数
    for (int i = 0; i < data_len;i++)
    {
        int bytes = FCGX_GetChar(data_stream); // 读取所有数据
        one_line_data += (char)bytes;
        if((char)bytes == '\n')//遇到换行
        {
            data_arr.push_back(one_line_data);//加入到数组中
            index++;
            one_line_data.clear();//清除单行数据
        }
        
    }
    
    //测试代码
    /*std::fstream file;
    file.open("Get_Client_Data.txt",std::ios::out|std::ios::app);

    for(auto it : data_arr)
    {
        file << it;
    }*/

    for(auto it : data_arr)
    {
        if(it =="\r\n")
        {
            break;
        }
        client_request_header += it;
    }

    //file << client_request_header;

    std::string Boundary; // 边界字符
    for(auto it : data_arr[0])
    {
        if(it == '\r')
        {
            break;
        }
        Boundary += it;
    }

    for (int i = data_arr[1].find("filename")+10; i < data_len;i++)//获取文件名
    {
        if(data_arr[1][i] == '"')
        {
            break;
        }
        File.File_Name += data_arr[1][i];
    }


    for (int i = 3; i < index; i++)//提取文件内容
    {
        if (data_arr[i].find(Boundary)!=std::string::npos)
        {
            continue;
        }
        for(auto it : data_arr[i])
        {
            File.File_Content += it;
        }
    }

    return 0;
}
/**
 * @brief 获取系统时间
 *
 * @param get_time 存放结果的字符串
 */
void Get_Now_Time(std::string &get_time)
{
    time_t this_time;
    char YMD_HMS[128];
    tm *now;

    time(&this_time);
    now = localtime(&this_time);
    sprintf(YMD_HMS, "%d.%d.%d %02d:%02d:%02d\n", now->tm_year + 1900, 1 + now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    get_time = YMD_HMS;
}

/**
 * @brief 登录检查函数
 *
 * @param lengh 数据长度
 * @param get_stream 数据读取位置
 * @param information 返回的信息
 * @returns
 * 成功返回true 失败返回false
 */
bool Login_Cheak(int lengh,FCGX_Stream *get_stream,std::string &information)
{
    
    std::string old_data_str;//获取原始数据字符串

    for (int i = 0; i < lengh; i++)
    {
        old_data_str.push_back(FCGX_GetChar(get_stream));//读取全部数据
    }
    json Be_Cheak_Data;//定义json对象
    Be_Cheak_Data = json::parse(old_data_str);//将原始字符串存入json对象

    std::vector<std::vector<std::string>> rel_user_name;//结果用户名
    std::vector<std::vector<std::string>> rel_password; // 结果密码

    char Command_Name[256];//查询指令
    std::string temp = Be_Cheak_Data["user"]["user_name"];
    sprintf(Command_Name, "SELECT password FROM user WHERE name='%s'", temp.c_str());

    Select_Data(Command_Name, rel_user_name); // 查询用户名是否存在

    char Command_Password[256];
    temp.clear();
    temp = Be_Cheak_Data["user"]["user_name"];
    sprintf(Command_Password, "SELECT password FROM user WHERE name='%s'",temp.c_str());
    Select_Data(Command_Password, rel_password);//查询密码是否正确

    if(rel_user_name.empty()==true)//如果用户名为空 报:用户名不存在
    {
        information = "The user name does not exist!\n";
        return false;
    }
    if (Be_Cheak_Data["user"]["password"] != rel_password[0][0])//客户端上传的密码不正确报密码错误
    {
        information = "Password Error!\n";
        return false;
    }
    information = "Ok";//正确查询允许登录
    return true;
}

/**
 * @brief 注册校验函数
 * 
 * @param data_len 数据长度
 * @param reply_data 准备返回给客户端的信息
 * @param data_stream 数据读取位置
 * @returns 成功 true 失败 false 
*/
bool Register_Check(int data_len, std::string &reply_data,FCGX_Stream *data_stream)
{
    if (spdlog::get("User_Register") == nullptr)//如果不存在名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("User_Register", Server_Conf::PATH_USER_REGISTER_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto db_log = spdlog::get("User_Register"); // 获取名为 Data_base 的日志管理器
    std::string old_data;//获取原始数据
    for (int i = 0; i < data_len;i++)
    {
        old_data.push_back(FCGX_GetChar(data_stream));
    }
    //db_log->info("this odl data {}", old_data);

    json get_data; // 定义json对象
    get_data = json::parse(old_data);//初始化json格式数据

    char db_command[256];//数据库命令
    std::string time;//获取当前时间
    Get_Now_Time(time);

    sprintf(db_command, "INSERT INTO user (name,nickname,password,phone,createtime,email) VALUES ('%s','%s','%s','%s','%s','%s')",
            std::string(get_data["user_name"]).c_str(),
            std::string(get_data["nick_name"]).c_str(),
            std::string(get_data["password"]).c_str(),
            std::string(get_data["telephone_number"]).c_str(),
            time.c_str(),
            std::string(get_data["email"]).c_str()); // 设置数据库命令

    //db_log->info("{}", db_command);
    std::string information;
    if (Insert_Data(db_command,information) == false)
    {
        reply_data = std::move(information);
        return false;
    }
    //db_log->info("insert seccuss ");
    reply_data = "Ok";
    return true;
}

/**
 * @brief 获取客户端参数
 * @param client_request_header 客户端请求头
 * @param need_find_arg 需要解析出来的参数名
 * @return 返回解析的参数
 */
std::string Get_Client_Arg(std::string client_request_header,std::string need_find_arg)
{
    std::string res;
    need_find_arg += "=\"";
    auto get_it = client_request_header.find(need_find_arg);
    for (int i = get_it+need_find_arg.size(); i < client_request_header.size(); i++)
    {
        if (client_request_header[i]=='\"')
        {
            break;
        }
        if(client_request_header[i]!='\n'||client_request_header[i]!='\r')
        {
            res += client_request_header[i];
        }
    }
    return res;
}

/**
 * @brief 获取本地文件路径
 * @param FDFS_ID fast_dfs 的文件ID
 * @return 成功返回
 */
bool Get_File_URL(std::string &FDFS_ID,std::string &Local_URL)
{
    int line_count=0;
    for(auto it : FDFS_ID)
    {
        if(it == '/')
        {
            line_count++;
        }
        if (line_count >= 2)
        {
            Local_URL.push_back(it);
        }
    }
    Local_URL = Server_Conf::FDFS_STORAGE_PATH + Local_URL;

    return true;
}

/**
 * @brief Md5文件校验,防止服务器出现重复的文件
 * @param Data_size 数据大小
 * @param data_strean 数据流
 * @return 成功返回 true \n 失败返回 false
 */
bool Md5_Check(int Data_size, FCGX_Stream *data_strean)
{

    if (spdlog::get("Md5_Check") == nullptr) // 如果不存在名为 Data_base 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Md5_Check",Server_Conf::PATH_MD5_CHECK_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto db_log = spdlog::get("Md5_Check"); // 获取名为 Data_base 的日志管理器

    std::string get_data;
    for (int i = 0; i < Data_size; i++)
    {
        int bytes = FCGX_GetChar(data_strean); // 读取所有数据
        get_data += (char)bytes;
    }
    json Client_Md5 = json::parse(get_data);

    if (Client_Md5.empty())
    {
        db_log->error("Can't get client data");
        return false;
    }

    char Check_Command[512];

    sprintf(Check_Command, "SELECT md5 FROM file_info WHERE md5='%s'", std::string(Client_Md5["file_md5"]).c_str());
    
    //通过Md5查询文件是否在服务器中已存在
    std::vector<std::vector<std::string>> res;
    if(Select_Data(Check_Command,res)==false)
    {
        db_log->info("Select_Data false");
        return false;
    }
    if(res.empty())
    {
        db_log->info("Select_Data empty");
        return false;
    }
    //文件已经存在,更新对应文件的文件计数
    memset(Check_Command,0,sizeof(Check_Command));
    sprintf(Check_Command, "UPDATE file_info SET count=count+1 WHERE md5='%s'",std::string(Client_Md5["file_md5"]).c_str());
    std::string db_info;
    Insert_Data(Check_Command, db_info);

    memset(Check_Command, 0, sizeof(Check_Command));
    std::string now_time;

    //未知的原因,首次获取Linux本地时间,时间并不准确,为确保数据正确,获取两次时间
    Get_Now_Time(now_time);
    Get_Now_Time(now_time);


    //对应用户数据更新
    sprintf(Check_Command, "INSERT into user_file_list(user,md5,createtime,filename,shared_status) values ('%s','%s','%s','%s','%d')",
            std::string(Client_Md5["user_name"]).c_str(),
            std::string(Client_Md5["file_md5"]).c_str(),
            now_time.c_str(),
            std::string(Client_Md5["file_name"]).c_str(),
            0);
    db_log->info(Check_Command);
    std::string db_info_2;
    Insert_Data(Check_Command, db_info_2);

    return true;
}

/**
 * @brief 删除文件...............请使用事务函数更新该函数!!!!!!!!!!!!!!!
 * @param data_stream 数据流
 * @param data_length 数据长度
 *
 * @return 成功返回true,失败返回false
 */
bool Delete_File(FCGX_Stream *data_stream,int data_length)
{
    if (spdlog::get("Delete_File") == nullptr) // 如果不存在名为 Delete_File 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Delete_File", Server_Conf::PATH_DELETE_FILE_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto db_log = spdlog::get("Delete_File"); // 获取名为 Delete_File 的日志管理器

    std::string string_data;
    int get_bytes;
    for (int i = 0; i < data_length; i++)
    {
        get_bytes = FCGX_GetChar(data_stream);
        string_data += (char)get_bytes;
    }//获取全部数据

    json json_data = json::parse(string_data);//序列化数据为json对象

    char db_command[512]; // 设置sql命令

    snprintf(db_command, sizeof(db_command), "SELECT DISTINCT f.count,f.md5,u.shared_status FROM file_info AS f INNER JOIN user_file_list as u ON u.md5 = f.md5 WHERE u.filename = '%s' AND u.user='%s'",
             std::string(json_data["delete_file_name"]).c_str(),
             std::string(json_data["user_name"]).c_str());

    std::vector<std::vector<std::string>> get_file_count;

    if(Select_Data(db_command, get_file_count)!=true)//查询
    {
        return false;
    }
    int count = std::atoi(get_file_count[0][0].c_str());//获取文件计数
    int shared_statue = std::atoi(get_file_count[0][2].c_str());

    db_log->info("Test:shared {}", shared_statue);

    /**
     * 当文件计数大于 1 时服务器会将对应的文件计数-1 当文件计数等于 1 时则会直接删除掉服务器上的文件并清空改文件在SQL中的数据
     */
    if (count == 1) // 当文件计数为 1 执行删除文件的逻辑
    {
        std::string get_file_url_command = "SELECT url FROM file_info WHERE md5='" + get_file_count[0][1] + "'";
        std::vector<std::vector<std::string>> File_Url;
        
        if (Select_Data(get_file_url_command.c_str(), File_Url) == false)
        {
            // Select_Data失败内部会写 SQL 日志
            return false;
        }
        else
        {
            //返回空的查询结果
            if (File_Url.empty())
            {
                db_log->info("Dont't have data of command:{}", get_file_url_command);//写日志并返回 false
                return false;
            }
            if(remove(File_Url[0][0].c_str()) == -1)//删除文件
            {
                db_log->error("Remove Server Resoure Failed:{}",strerror(errno));
                return false;
            }
        }
        std::string command = "DELETE FROM file_info WHERE md5='" + get_file_count[0][1] + "'";//删除对应文件在SQL中的数据
        if(Delete_DB_Data(command.c_str()) == false)
        {
            return false;
        }
    }
    else//当文件计数大于 1 执行SQL命令将该文件计数 -1
    {
        std::string db_info;
        char command[512];
        sprintf(command, "UPDATE file_info SET count=count-1 WHERE md5='%s'", get_file_count[0][1].c_str());

        if (Insert_Data(command, db_info) == false)
        {
            return false;
        }
    }
    
    //以下代码应和上方的数据库操作共同使用事务函数进行以确保原子性!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //删除对应文件在对应用户下的数据
    std::string create_time = json_data["delete_file_create_time"];
    if (create_time.find('\n') == std::string::npos)
    {
        create_time += '\n';
    }

    if (shared_statue == 1)
    {
        char delete_shared[512];

        sprintf(db_command, "DELETE FROM share_file_list WHERE createtime='%s' AND filename='%s' AND user='%s'",
                create_time.c_str(),
                std::string(json_data["delete_file_name"]).c_str(),
                std::string(json_data["user_name"]).c_str());

        if (Delete_DB_Data(db_command) == false)
        {
            return false;
        }
    }

    memset(db_command, 0, sizeof(db_command));

    sprintf(db_command, "DELETE FROM user_file_list WHERE createtime='%s' AND filename='%s' AND user='%s'",
            create_time.c_str(),
            std::string(json_data["delete_file_name"]).c_str(),
            std::string(json_data["user_name"]).c_str());

    if (Delete_DB_Data(db_command) == false)
    {
        return false;
    }
    return true;
    // 以上代码应和上方的数据库操作共同使用事务函数进行以确保原子性!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
/**
 * @brief 获取 Get 请求中URL携带的参数
 * @param url url携带参数的部分
 * @param be_get_arg 需要获取的参数名
 * 
 * @return 返回获取的参数
 */
std::string Get_Url_Arg(std::string url,std::string be_get_arg)
{

    std::string res;
    for (int i = url.find(be_get_arg)+be_get_arg.size()+1; i < url.size(); i++)
    {
        if (url[i]=='&')
        {
            break;
        }
        res.push_back(url[i]);
    }
    res = acl_url_decode(res.c_str());//对数据进行解码

    return res;
}

/**
 * @brief 获取需要下载的文件的URL
 * @param data_stream 数据流
 * @param data_length 数据长度
 * @param DownLoad_File_URL 返回资源URL
 * @return 返回客户端请求的文件的URL
 */
bool Get_DownLoad_File_URL(FCGX_Stream *data_stream, int data_length, FCGX_ParamArray *evnp,std::string &DownLoad_File_URL)
{
    if (spdlog::get("DownLoad_File") == nullptr) // 如果不存在名为 Delete_File 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("DownLoad_File",Server_Conf::PATH_DOWNLOAD_FILE_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto downLoad_log = spdlog::get("DownLoad_File"); // 获取名为 Delete_File 的日志管理器
    std::string Client_data;
    int get_bytes;
    for (int i = 0; i < data_length; i++)
    {
        get_bytes = FCGX_GetChar(data_stream);
        Client_data += (char)get_bytes;
    }//读取所有数据

    std::string get_client_arg = FCGX_GetParam("QUERY_STRING", *evnp);//获取URL中携带的参数

    /* 前端没写关于下载量的显示UI什么时候需要写了取消注释再做吧.....................
    json client_data;
    client_data["user_name"] = Get_Url_Arg(get_client_arg, "user_name");
    client_data["download_file_name"] = Get_Url_Arg(get_client_arg, "download_file_name");
    client_data["shared_statue"] = Get_Url_Arg(get_client_arg, "shared_statue");

    if(Check_Data_wholeness(client_data, "user_name", "download_file_name","shared_statue") == false)
    {
        return false;
    }
    */

    char select_command[512];

    snprintf(select_command, sizeof(select_command),
             "SELECT DISTINCT f.url FROM file_info AS f INNER JOIN user_file_list AS u ON u.md5 = f.md5 WHERE u.user = '%s' AND u.filename='%s'",
             Get_Url_Arg(get_client_arg, "user_name").c_str(),
             Get_Url_Arg(get_client_arg, "download_file_name").c_str()); // 拼接数据库查询指令

    std::vector<std::vector<std::string>> Db_reply;

    Select_Data(select_command, Db_reply);//查询
    if(Db_reply.empty())//返回结果为空
    {
        downLoad_log->warn("Don't have file in server!");
        return false;
    }

    DownLoad_File_URL = std::move(Db_reply[0][0]);
    return true;
}

/**
 * @brief 文件共享
 * @param data_length 接收数据长度
 * @param data_stream 数据流
 * @return 成功返回 true,失败返回 flase
 */
bool Shared_File(int data_length,FCGX_Stream *data_stream,std::string &reply_info)
{
    if (spdlog::get("Shared_File") == nullptr) // 如果不存在名为 Delete_File 的日志管理器则创建
    {
        auto logger_db = spdlog::rotating_logger_mt("Shared_File",Server_Conf::PATH_SHARED_FILE_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_db->flush_on(spdlog::level::trace);
    }

    auto Shared_log = spdlog::get("Shared_File"); // 获取名为 Delete_File 的日志管理器

    std::string Client_data;
    int get_bytes;
    for (int i = 0; i < data_length; i++)
    {
        get_bytes = FCGX_GetChar(data_stream);
        Client_data += (char)get_bytes;
    } // 读取所有数据

    json client_json_data = json::parse(Client_data);//序列化用户数据

    if (Check_Data_wholeness(client_json_data, "user_name", "create_time", "file_name") == false)
    {
        return false;
    }

    //组织数据库查询语句
    char select_command[512];
    snprintf(select_command, sizeof(select_command), "SELECT md5,shared_status FROM user_file_list WHERE user='%s' AND createtime='%s' AND filename='%s'",
             std::string(client_json_data["user_name"]).c_str(),
             std::string(client_json_data["create_time"]).c_str(),
             std::string(client_json_data["file_name"]).c_str());

    std::vector<std::vector<std::string>> get_md5;
    if(Select_Data(select_command,get_md5)==false)
    {
        return false;
    }

    if (std::atoi(get_md5[0][1].c_str())==1)
    {
        reply_info = "The file already be shared,needn't shared that again";
        return false;
    }

    char insert_share_file_list[512];

    snprintf(insert_share_file_list, sizeof(insert_share_file_list), "INSERT INTO share_file_list(user,md5,createtime,filename,pv) VALUE('%s','%s','%s','%s','%d')",
             std::string(client_json_data["user_name"]).c_str(),
             get_md5[0][0].c_str(),
             std::string(client_json_data["create_time"]).c_str(),
             std::string(client_json_data["file_name"]).c_str(),
             0);

    char update_user_file_list[512];

    snprintf(update_user_file_list, sizeof(update_user_file_list), "UPDATE user_file_list SET shared_status=1 WHERE user='%s' AND filename='%s' AND createtime='%s'",
             std::string(client_json_data["user_name"]).c_str(),
             std::string(client_json_data["file_name"]).c_str(),
             std::string(client_json_data["create_time"]).c_str());

    if (DB_transaction(insert_share_file_list, update_user_file_list)==false)
    {
        return false;
    }

    return true;
}

/**
 * @brief 取消共享
 *
 * @param data_stream 数据流
 * @param data_length 数据长度
 * 
 * @return 成功返回true,失败回false
 */
bool Cancel_Shared(FCGX_Stream *data_stream,int data_length)
{
    std::string get_client_data;//从客户端获取的原始数据
    json json_data;//对原始数据序列化成json对象

    int get_bytes;
    for (int i = 0; i < data_length; i++)
    {
        get_bytes = FCGX_GetChar(data_stream);
        get_client_data += (char)get_bytes;
    } // 读取所有数据

    json_data = json::parse(get_client_data);//序列化

    if (Check_Data_wholeness(json_data, "user_name", "file_create_time", "file_name") == false)//检查是否有数据缺失
    {
        return false;
    }

    char delete_shared_command[512];//删除的命令
    char update_statue_command[512];//更新的命令

    //---------- 拼接命令
    snprintf(delete_shared_command, sizeof(delete_shared_command), "DELETE FROM share_file_list WHERE user='%s' AND createtime='%s' AND filename='%s'",
             std::string(json_data["user_name"]).c_str(),
             std::string(json_data["file_create_time"]).c_str(),
             std::string(json_data["file_name"]).c_str());

    snprintf(update_statue_command, sizeof(update_statue_command), "UPDATE user_file_list SET shared_status = '0' WHERE shared_status = '1' AND user='%s' AND createtime='%s' AND filename='%s'",
             std::string(json_data["user_name"]).c_str(),
             std::string(json_data["file_create_time"]).c_str(),
             std::string(json_data["file_name"]).c_str());
    //---------- 拼接命令

    bool stat = DB_transaction(delete_shared_command, update_statue_command);//执行事务
    if (stat == false)
    {
        return false;
    }

    return true;
}