#include "server_confg.h"
#include "make_log.h"
#include "json.hpp"
#include "Data_Solve.hpp"
#include "unistd.h"
using json = nlohmann::json;
int main()
{
    auto logger_login = spdlog::rotating_logger_mt("Login_Check",Server_Conf::PATH_LOGIN_CHECK_LOG, 1024 * 1024 * 3, 3);
    logger_login->flush_on(spdlog::level::trace);

    FCGX_Stream *input, *output, *error;
    FCGX_ParamArray evnp;

    while (FCGX_Accept(&input, &output, &error, &evnp) >= 0) // 阻塞
    {
        std::string Get_Client_Data;
        std::string Data;
        char *pRequestMoethod = FCGX_GetParam("REQUEST_METHOD", evnp);
        if (pRequestMoethod == NULL)
        {
            logger_login->error("getevn REQUEST_METHOD is failed.");
            break;
        }
        char *pLenstr = FCGX_GetParam("CONTENT_LENGTH", evnp);
        if (pLenstr == NULL)
        {
            logger_login->error("Get data failed.");
            break;
        }
        auto lenght = atoi(pLenstr);
        std::string information;
        Login_Cheak(lenght, input, information);
        char reply[128];
        sprintf(reply,"{\"info\":\"%s\"}",information.c_str());
        // 数据回写
        FCGX_FPrintF(output, "Content-type: application/json\r\n"
                             "Charset: utf-8\r\n"
                             "\r\n"
                            "%s",reply);   
    }
    return 0;
}