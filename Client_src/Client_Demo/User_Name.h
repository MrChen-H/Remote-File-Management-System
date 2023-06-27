#ifndef USER_NAME_H
#define USER_NAME_H
#include "QString"
//const QString;
namespace Server_URL
{
    const QString UPLOAD_FILE_URL = "http://192.168.118.134:80/Upload";
    const QString REGISTER_URL = "http://192.168.118.134:80/Register";
    const QString LOGIN_URL = "http://192.168.118.134:80/Login";
    const QString MD5_CHECK_URL = "http://192.168.118.134:80/Md5_Check";
    const QString DELETE_FILE_URL = "http://192.168.118.134:80/Delete_File";
    const QString DOWNLOAD_URL = "http://192.168.118.134:80/DownLoad_File";
    const QString SHARED_URL = "http://192.168.118.134:80/Shared_File";
    const QString CANCEL_SHARED_URL = "http://192.168.118.134:80/Cancel_Shared_File";
}

class Get_User_Name
{
public:
    static Get_User_Name* Instance()
    {
        static Get_User_Name User_Name;
        return &User_Name;
    }
    QString Name;
};

#endif // USER_NAME_H
