//
// Created by delta1037 on 2022/8/18.
//

#ifndef DIALOG_SERVER_H
#define DIALOG_SERVER_H
#include <string>
#include <vector>
#include <list>
#ifndef __linux__
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
typedef SOCKET int;
#endif

#define MAX_BUFFER 2048
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4321

#define FORMAT_PREFIX "<%s, %s, %d> "
#define FILENAME ( __builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__ )
#define dialog_log(format, args...) printf(FORMAT_PREFIX#format, FILENAME, __FUNCTION__ , __LINE__, ##args);

typedef void DIALOG_PRINT(const char *, ...);
typedef void MENU_FUNC(uint32_t idx, DIALOG_PRINT);

struct DialogRegisterData {
    std::string name;
    MENU_FUNC *func;
    std::vector<std::pair<int, std::string>> menu;

    // menu内容注册接口
    void register_menu(uint32_t id, const std::string& detail){
        menu.emplace_back(id, detail);
    }
};

class DialogServer {
protected:
    DialogServer();
    ~DialogServer();

    // 注册新的菜单
    void register_menu(const std::string &, MENU_FUNC);

private:
    // 接收数据并整合到一起（自动添加换行符）
    void dialog_print(const char *, ...);

    // 后台线程，接收连接并处理操作指令
    void dialog_thread();

private:
    bool init_status;
    std::string m_dialog_buffer;

    uint32_t m_dialog_idx;
    std::list<std::pair<uint32_t, DialogRegisterData>> m_dialog_map;

    // 套接字定义
    SOCKET m_dialog_server_socket;
};


#endif //DIALOG_SERVER_H
