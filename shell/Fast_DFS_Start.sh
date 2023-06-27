#!/bin/bash
tracker_start()
{
    ps aux | grep fdfs_trackerd | grep -v grep > /dev/null
    if [ $? -eq 0 ];then
        echo "The program fdfs_trcked is runing...."
    else 
        sudo fdfs_trackerd /etc/fdfs/tracker.conf 
        if [ $? -eq 0 ];then
            echo "The program fdfs_tracked start success..."
        else    
            echo "The program fdfs_trackend start failed... (more info please reading log file)"
        fi
    fi
}

storaged_start()
{
    ps aux | grep fdfs_storaged | grep -v grep > /dev/null
    if [ $? -eq 0 ];then
        echo "The program fdfs_storaged is runing...."
    else    
        sudo fdfs_storaged /etc/fdfs/storage.conf 

        if [ $? -eq 0 ];then
            echo "The program fdfs_storaged start success.."
        else 
            echo "The program fadfs_trackend start failed... (more info please reading log file)"
        fi
    fi
}

stop_fdfs()
{
    sudo fdfs_trackerd /etc/fdfs/tracker.conf stop
    sudo fdfs_storaged /etc/fdfs/storage.conf stop
}

case $1 in
    stop)
        stop_fdfs
        ;;
    *)
        tracker_start
        storaged_start
        ;;
esac 

