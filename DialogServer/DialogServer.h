#ifndef DIALOG_SERVER_H
#define DIALOG_SERVER_H
#include <string>
#include <vector>
#include <map>
#include <thread>
#ifndef __linux__
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
typedef SOCKET int;
#endif

#define MAX_BUFFER 2048
#define CONTENT_BUFFER 2048

/* 注意与客务端保持一致 */
#define SOCKET_BUFFER_SIZE 20480
#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 4321

#define FORMAT_PREFIX "<%s, %d> "
#define FILENAME ( __builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__ )
#define dialog_info(format, args...) printf(FORMAT_PREFIX#format, FILENAME , __LINE__, ##args);printf("\n");
#ifdef DEBUG
#define dialog_debug(format, args...) printf(FORMAT_PREFIX#format, FILENAME, __FUNCTION__ , __LINE__, ##args);printf("\n");
#else
#define dialog_debug(format, args...)
#endif

class DialogServer;
typedef void (DialogServer::*DIALOG_PRINT)(const char *, ...);
typedef void (DialogServer::*MENU_FUNC)(uint32_t idx);

struct DialogRegisterData {
    std::string name;
    DialogServer *p_instance;
    MENU_FUNC func;
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
    static DialogRegisterData *register_main(const std::string &, MENU_FUNC, DialogServer *p_instance);

//    // 数据打印函数
    virtual void dialog(uint32_t idx) = 0;

    // 接收数据并整合到一起（自动添加换行符）
    static void dialog_print(const char *, ...);

private:
    // 后台线程，接收连接并处理操作指令
    static void dialog_thread();

    // 目录输出命令
    static std::string get_tips_table(int idx);
private:
    static bool s_init_status;
    static std::string s_dialog_buffer;

    static uint32_t s_dialog_idx;
    static std::map<uint32_t, DialogRegisterData> s_dialog_map;

    static bool s_thread_exit_flag;
    static std::thread *s_thread_ctrl;

    // 套接字定义
    static SOCKET s_dialog_server_socket;
};


#endif //DIALOG_SERVER_H
