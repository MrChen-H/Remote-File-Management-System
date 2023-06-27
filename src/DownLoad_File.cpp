#include "server_confg.h"
#include "Data_Solve.hpp"
#include "File_Class.hpp"
#include "make_log.h"
#include "dfs_upload.h"
#include "db_operation.h"

int main()
{
    auto logger_donwload = spdlog::rotating_logger_mt("DownLoad_File",Server_Conf::PATH_DOWNLOAD_FILE_LOG, 1024 * 1024 * 3, 3);
    logger_donwload->flush_on(spdlog::level::trace);//创建日志管理器
    FCGX_Stream *out_put, *in_put, *error;
    FCGX_ParamArray evnp;

    while (FCGX_Accept(&in_put, &out_put, &error, &evnp) >= 0)//阻塞监听
    {
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == nullptr)
        {
            logger_donwload->error("ERROR:Unkonw request!");
        }

        char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp);//获取数据长度

        if (pLenstr == NULL)
        {
            logger_donwload->error("ERROR: Get data failed:{}", strerror(errno));
            continue;
        }

        int len = atoi(pLenstr);
        std::string be_get_file_url;
        if (Get_DownLoad_File_URL(in_put, len, &evnp, be_get_file_url))//获取请求的资源路径
        {
            std::ifstream be_send_file;
            be_send_file.open(be_get_file_url, std::ifstream::in | std::ios::binary);//打开本地资源文件

            //获取文件内容
            int data_length;
            be_send_file.seekg(0, be_send_file.end);
            data_length = be_send_file.tellg();
            be_send_file.seekg(0, be_send_file.beg);
            char *file_buffer = new char[data_length];
            be_send_file.read(file_buffer,data_length);
            be_send_file.close();

            FCGX_FPrintF(out_put, "Content-type: multipart/form-data\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n");

            for (int i = 0; i < data_length;i++)//返回给客户端文件内容
            {
                FCGX_PutChar(file_buffer[i],out_put);
            }

            delete[] file_buffer; // 释放 file_buffer
        }
        else // 如果失败 返回给客户端 Failed 信息R
        {
            FCGX_FPrintF(out_put, "Content-type: application/json\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n"
                                  "{\"Down_Load_Statue\":\"Failed\"}");
        }
    }
    return 0;
}