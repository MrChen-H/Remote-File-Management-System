/*
    待优化项: 
        服务器可以先创建临时文件,然后由Fast DFS 上传至存储介阶段,获取到FDFS 文件路径后直接在最终的文件进行内容写入,避免一次文件拷贝

*/
#include "server_confg.h"
#include "Data_Solve.hpp"
#include"File_Class.hpp"
#include"make_log.h"
#include"dfs_upload.h"
#include"db_operation.h"
int main()
{
    FCGX_Stream *input, *output, *error;
    FCGX_ParamArray evnp;

    auto logger_upload = spdlog::rotating_logger_mt("UpLoad_File", "/usr/local/nginx/logs/CGI_Log/UpLoad_File/upload_file.log", 1024 * 1024 * 3, 3);
    logger_upload->flush_on(spdlog::level::trace);

    while (FCGX_Accept(&input, &output, &error, &evnp) >= 0) // 阻塞
    {
        File_Struct File;
        std::string Get_Client_Data;
        std::string Data;
        std::string client_request_header;
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == NULL)
        {
            logger_upload->error("getevn REQUEST_METHOD is failed.");
            break;
        }
        // 判断请求方法
        if (strcmp(pRequestMoethod, "POST") == 0)
        {
            // 获取请求中URL携带的参数，POST也会在URL中携带参数
            Get_Client_Data = FCGX_GetParam("QUERY_STRING", evnp);
            // 获取POST请求携带的参数长度
            char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp);
            if (pLenstr != NULL)
            {
                long len = atoi(pLenstr);
                //std::string postData;
                // 获取POST携带的数据   
                POST_Data_Get(File,len,input,client_request_header);
            }
            else
            {
                logger_upload->error("getenv failed.");
            }
        }
        else
        {
            logger_upload->error("This is unknown quest {}", strerror(errno));
        }

        // 向客户端返回用到数据
       
        
        File.CREATE_File("/home/mrfeng/"+File.File_Name);//在服务器创建资源

        if(Fdfs_Upload(File.Get_File_Path(), File.File_Fdfs_ID) != 0)
        {
            FCGX_FPrintF(output, "Content-type: application/json\r\n"
                                 "Charset: utf-8\r\n"
                                 "\r\n"
                                 "{\"UpLoad Statue\":\"Failed\"}");
            continue;
        }

        if (remove(std::string("/home/mrfeng/" + File.File_Name).c_str()) != 0)//删除服务器临时文件
        {
            logger_upload->error("Server Temp file delete failed {}", strerror(errno));
        }

        char db_command[512];
        std::string Local_URL;

        std::string Fdfs_ID_Temp(File.File_Fdfs_ID);
        Get_File_URL(Fdfs_ID_Temp, Local_URL);

        sprintf(db_command, "INSERT into file_info(md5,file_id,url,size,type,count) values ('%s','%s','%s','%s','%s','%d')",
                Get_Client_Arg(client_request_header, "file_md5").c_str(),
                File.File_Fdfs_ID,
                Local_URL.c_str(),
                Get_Client_Arg(client_request_header, "file_size").c_str(),
                Get_Client_Arg(client_request_header, "file_type").c_str(),
                1);

        if (DB_Insert(db_command) != 0)
        {
            FCGX_FPrintF(output, "Content-type: application/json\r\n"
                                 "Charset: utf-8\r\n"
                                 "\r\n"
                                 "{\"UpLoad Statue\":\"Failed\"}");
            continue;
        } // 将文件信息上传至 file_info表
        memset(db_command, 0, sizeof(db_command));//清空指令字符串数组
        std::string now_time;
        Get_Now_Time(now_time);

        sprintf(db_command, "INSERT into user_file_list(user,md5,createtime,filename,shared_status) values ('%s','%s','%s','%s','%d')",
                Get_Client_Arg(client_request_header, "user_name").c_str(),
                Get_Client_Arg(client_request_header, "file_md5").c_str(),
                now_time.c_str(),
                File.File_Name.c_str(),
                0);

        if (DB_Insert(db_command) != 0)
        {
            FCGX_FPrintF(output, "Content-type: application/json\r\n"
                                 "Charset: utf-8\r\n"
                                 "\r\n"
                                 "{\"UpLoad Statue\":\"Failed\"}");
            continue;
        } // 将用户 和 文件的对应信息上传至 user_file_list 表*/

        FCGX_FPrintF(output, "Content-type: application/json\r\n"
                             "Charset: utf-8\r\n"
                             "\r\n"
                             "{\"UpLoad Statue\":\"Success\"}");

    }
    return 0;
}