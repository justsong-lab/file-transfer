//
// Created by song on 6/20/2022.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "muduo/net/TcpServer.h"

#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include <functional>
#include <memory>
#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class EchoServer {
public:
    EchoServer(EventLoop *loop, const InetAddress &listenAddr)
            : loop_(loop),
              server_(loop, listenAddr, "EchoServer") {
        server_.setConnectionCallback(
                std::bind(&EchoServer::onConnection, this, _1));
        server_.setMessageCallback(
                std::bind(&EchoServer::onMessage, this, _1, _2, _3));
    }

    void start() {
        server_.start();
    }

private:
    void onConnection(const TcpConnectionPtr &conn);

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

    EventLoop *loop_;
    TcpServer server_;
};


#endif //SERVER_SERVER_H
