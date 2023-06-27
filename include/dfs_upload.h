#pragma once
/**
 *  @brief 上传文件所用函数
 *  @param file_path 需要进行上传的文件路径
 *  @param file_id_count 存放上传完成的file_id
 **/
int Fdfs_Upload(const char *file_path, char *file_id_cont);