#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define USER_REGIST_SUCCEED "regist succeed"
#define USER_REGIST_FAILED "rigist failed"

#define USER_LOGIN_SUCCEED "login succeed"
#define USER_LOGIN_FAILED_1 "login failed: parameter error"
#define USER_LOGIN_FAILED_2 "login failed: name or pswd error"
#define USER_LOGIN_FAILED_3 "login failed: user logined"

#define ADD_FRIEND_SUCCEED "add succeed"
#define ADD_FRIEND_FAILED_1 "add failed: parameter error"
#define ADD_FRIEND_FAILED_2 "add failed: user isn't exists"
#define ADD_FRIEND_FAILED_3 "add failed: friend is existed"
#define ADD_FRIEND_FAILED_4 "add failed: unknow error"

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN = 0,

    ENUM_MSG_TYPE_REGIST_REQUEST,       //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,       //注册响应

    ENUM_MSG_TYPE_LOGIN_REQUEST,       //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,       //登录响应

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,   //添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,   //添加好友响应

    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,     //同意添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,         //拒绝添加好友请求

    ENUM_MSG_TYPE_SHOW_ALL_FRIEND_REQUEST,      //显示所有在线好友申请请求
    ENUM_MSG_TYPE_SHOW_ALL_FRIEND_RESPOND,      //显示所有在线好友申请回应

    ENUM_MSG_TYPE_SEND_MSG_REQUEST,     //发送信息请求
    ENUM_MSG_TYPE_SEND_MSG_RESPOND,     //发送信息响应


    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

struct PDU
{
    uint usPDULen;      //协议的总长度
    uint usPDUType;     //协议的类型
    char usData[64];    //协议的内容
    uint usMsgLen;      //协议负载的长度
    uint usMsg[];        //协议的负载
};

PDU *mkPDU(uint usMsgLen);

#endif // PROTOCOL_H
