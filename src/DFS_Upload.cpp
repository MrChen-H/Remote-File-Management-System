#include "server_confg.h"
#include"fastdfs/client_func.h"
#include"fastcommon/logger.h"
#include"make_log.h"

/**
 *  @brief 上传文件所用函数
 *  @param file_path 需要进行上传的文件路径
 *  @param file_id_count 存放上传完成的file_id  
**/
int  Fdfs_Upload(const char* file_path,char*file_id_cont)
{
    if (spdlog::get("DFS_Upload") == nullptr) // 如果不存在名为 Delete_File 的日志管理器则创建
    {
        auto logger_dfs = spdlog::rotating_logger_mt("DFS_Upload", Server_Conf::PATH_UPLOAD_FILE_LOG, 1024 * 1024 * 3, 3); // 初始化日志
        logger_dfs->flush_on(spdlog::level::trace);
    }

    auto logger_dfs = spdlog::get("DFS_Upload"); // 获取名为 Delete_File 的日志管理器
    int result = 0;
    ConnectionInfo *ptr_tracker;
    fdfs_client_init(Server_Conf::FDFS_CLIENT_CONF.c_str());
    ptr_tracker = tracker_get_connection();

    ConnectionInfo ptr_storage;
    char group_name[FDFS_MAX_GROUPS + 1];

    int store_path_index;

    result = tracker_query_storage_store(ptr_tracker, &ptr_storage, group_name, &store_path_index);

    if (result != 0)
    {
        logger_dfs->error("DFS 存储节点获取失败 %s", strerror(errno));
        //LOG("DFS Log", "error", "DFS 存储节点获取失败 %s\n", );
    }
    //char file_id[128] = {};
    result = storage_upload_by_filename1(ptr_tracker, &ptr_storage, store_path_index, file_path, NULL, NULL, 0, group_name, file_id_cont);
    if (result == 0)
    {
        fdfs_client_destroy();
        return 0;
    }
    logger_dfs->error("DFS 上传失败 {}", strerror(errno));

    fdfs_client_destroy();
    return -1;
}