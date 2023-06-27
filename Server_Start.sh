#!/bin/bash
nginx_start()
{
    ps aux | grep nginx | grep -v grep > /dev/null
    if [ $? -eq 0 ];then
        sudo nginx -s quit  
        sudo nginx -c $(pwd)/NGINX_conf/nginx.conf
        echo "Nginx start success..."
    else 
        sudo nginx -c $(pwd)/NGINX_conf/nginx.conf
        if [ $? -eq 0 ];then
            echo "Nginx start success..."
        else    
            echo "Nginx start failed... (more info please reading log file)\n"
        fi
    fi
}

CGI_bind()
{
    sudo spawn-fcgi CGI_bin/Up_Load_File -p 10001
    sudo spawn-fcgi CGI_bin/Login_Check -p 10002
    sudo spawn-fcgi CGI_bin/User_Register -p 10003
    sudo spawn-fcgi CGI_bin/Md5_Check -p 10004
    sudo spawn-fcgi CGI_bin/Delete_File -p 10005
    sudo spawn-fcgi CGI_bin/DownLoad_File -p 10006
    sudo spawn-fcgi CGI_bin/Shared_File -p 10007
    sudo spawn-fcgi CGI_bin/Cancel_Shared_File -p 10008
}
stop_server()
{
    GET_PROT=`sudo ps -aux |grep -i Up_Load_File |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i Login_Check |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i User_Register |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i Md5_Check |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i Delete_File |grep -v grep|awk '{print $2}'`   
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i DownLoad_File |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i Shared_File |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi

    GET_PROT=`sudo ps -aux |grep -i Cancel_Shared_File |grep -v grep|awk '{print $2}'`
    if [ "$GET_PROT" ];then
        sudo kill -9 $GET_PROT
    fi
}


case $1 in
    stop)
        stop_server
        ;;
    *)
        sudo ./s
        hell/Fast_DFS_Start.sh
        nginx_start
        CGI_bind
        ;;
esac 