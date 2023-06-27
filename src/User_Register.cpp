#include"spdlog/spdlog.h"
#include "server_confg.h"
#include "Data_Solve.hpp"
int main()
{
    auto logger_reg = spdlog::rotating_logger_mt("Register_Log", Server_Conf::PATH_USER_REGISTER_LOG, 1024 * 1024 * 3, 3);
    logger_reg->flush_on(spdlog::level::trace);
    FCGX_Stream *input, *output, *error;
    FCGX_ParamArray evnp;

    while (FCGX_Accept(&input, &output, &error, &evnp) >= 0) // 阻塞
    {
        std::string Get_Client_Data;
        std::string Data;
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == NULL)
        {
            logger_reg->error("ERROR: getevn REQUEST_METHOD is failed: {}", strerror(errno));
            break;
        }
        char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp);
        if (pLenstr == NULL)
        {
            logger_reg->error("ERROR: Get data failed:{}", strerror(errno));
            break;
        }
        auto lenght = atoi(pLenstr);
        std::string information;
        Register_Check(lenght, information, input);
        char reply[128];
        sprintf(reply, "{\"info\":\"%s\"}", information.c_str());
        // 数据回写
        FCGX_FPrintF(output, "Content-type: application/json\r\n"
                             "Charset: utf-8\r\n"
                             "\r\n"
                             "%s",reply);
    }
    return 0;
}