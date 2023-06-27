#include "server_confg.h"
#include "Data_Solve.hpp"
#include "File_Class.hpp"
#include "make_log.h"
#include "dfs_upload.h"
#include "db_operation.h"

int main()
{
    auto logger_shared = spdlog::rotating_logger_mt("Shared_File",Server_Conf::PATH_SHARED_FILE_LOG, 1024 * 1024 * 3, 3);
    logger_shared->flush_on(spdlog::level::trace); // 创建日志管理器
    FCGX_Stream *out_put, *in_put, *error;
    FCGX_ParamArray evnp;

    while (FCGX_Accept(&in_put, &out_put, &error, &evnp) >= 0) // 阻塞监听
    {
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == nullptr)
        {
            logger_shared->error("ERROR:Unkonw request!");
        }

        char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp); // 获取数据长度

        if (pLenstr == NULL)
        {
            logger_shared->error("ERROR: Get data failed:{}", strerror(errno));
            continue;
        }

        int len = atoi(pLenstr);
        std::string reply_info;
        if (Shared_File(len, in_put, reply_info))
        {
            FCGX_FPrintF(out_put, "Content-type: application/json\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n"
                                  "{\"Shared_Statue\":\"Ok\"}");       
        }
        else
        {
            FCGX_FPrintF(out_put, "Content-type: application/json\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n"
                                  "{\"Shared_Statue\":\"%s\"}",reply_info.c_str());
        }
    }
    return 0;
}