#include "server_confg.h"
#include "Data_Solve.hpp"
#include "File_Class.hpp"
#include "make_log.h"
#include "dfs_upload.h"
#include "db_operation.h"

int main()
{
    auto logger_md5 = spdlog::rotating_logger_mt("Md5_Check",Server_Conf::PATH_MD5_CHECK_LOG, 1024 * 1024 * 3, 3);
    logger_md5->flush_on(spdlog::level::trace);
    FCGX_Stream *out_put, *in_put, *error;
    FCGX_ParamArray evnp;

    while (FCGX_Accept(&in_put, &out_put, &error, &evnp) >= 0)
    {
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == nullptr)
        {
            logger_md5->error("ERROR:Unkonw request!");
        }

        char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp);

        if (pLenstr == NULL)
        {
            logger_md5->error("ERROR: Get data failed:{}", strerror(errno));
            continue;
        }
        
        int len = atoi(pLenstr);
        if (Md5_Check(len,in_put))
        {
            //  回写md5秒传成功
            FCGX_FPrintF(out_put, "Content-type: application/json\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n"
                                  "{\"allow_md5_save\":\"Yes\"}");
        }
        else
        {
            // 回写md5秒传失败
            FCGX_FPrintF(out_put, "Content-type: application/json\r\n"
                                  "Charset: utf-8\r\n"
                                  "\r\n"
                                  "{\"allow_md5_save\":\"No\"}");
        }

    }
}