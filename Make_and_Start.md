# 构建与启动

## 构建
1. 执行 make.sh     

        ./make.sh

2. 进入build文件夹(自动生成)执行make命令

        make

## 启动
1. 创建 MySQL 表(server_sql.sql)

        mysql -u root -p your_password server_sql.sql

2. 执行Server_Start脚本 

        sudo ./Server_Start.sh