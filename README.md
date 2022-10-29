# 3rdparty

仓库说明：第三方库集合

## dialog_*

- 项目状态：可用

- 项目目标：查看**运行中**的软件的数据

- 实现方案：分为服务端和客户端，通过socket互联。服务端与需要查看数据的程序集成到一起，并注册查看数据的命令；客户端通过向正在运行的程序发送指令和接收输出来显示正在运行的程序的数据。

- 服务端和客户端互联：服务端注册的内容就是客户端收到的菜单，客户端根据菜单输入对应的数字标号就可以获取对应内容的输出。

  - dialog_server：服务端部分

  - dialog_client：客户端部分

## dlog

- 项目状态：可用
- 项目目标：一个简易的日志管理程序，配置软件的日志输出位置和输出等级。

## http

- 项目状态：可用
- 项目目标：C++实现的http库，可以方便的实现HTTP请求和搭建HTTP服务器
- 项目来源：https://github.com/yhirose/cpp-httplib

## jsoncpp

- 项目状态：可用
- 项目目标：C++实现的json库，可以实现json文件的读写
- 项目来源：https://github.com/open-source-parsers/jsoncpp

## mysql-*

- 项目状态：可用
- 项目目标：mysql操作接口
- 项目来源：已安装的mysql

## dconfig

- 项目状态：可用

- 项目目标：读写配置文件封装

- 项目依赖：jsoncpp

