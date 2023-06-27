#pragma once
#include "server_confg.h"
class File_Struct
{
    private:
        char File_Path[256];
        std::string File_Owner;
        size_t File_Size = 0;

    public:
        std::string File_Content;
        std::string File_Name;
        char File_Fdfs_ID[128];

    public:
        //function

        void SET_File_Path(std::string Path)
        {
            strcpy(File_Path,Path.c_str());
        }

        void SET_File_Owner(std::string Owner_Name)
        {
            File_Owner = std::move(Owner_Name);
        }

        void CREATE_File(std::string Path)
        {
            SET_File_Path(Path);
            File_Size = File_Content.size();

            if(File_Size < 0)
            {
                return;
            }
            SET_File_Path(this->File_Path);
            int file_fd = open(this->File_Path, O_CREAT | O_WRONLY, 0664);
            write(file_fd,File_Content.c_str(),File_Size);
        }
        const char *Get_File_Path()
        {
            return File_Path;
        }
        //void SET_Fdfs_Path(std::string Path_ID)
        //{
        //    strcpy(File_Fdfs_ID, Path_ID.c_str());
        //}
};