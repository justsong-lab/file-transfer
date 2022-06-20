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

const char *g_file = nullptr;
const int bufferSize = 64 * 1024;
typedef std::shared_ptr<FILE> FilePtr;

void onConnection(const TcpConnectionPtr &conn) {
    LOG_INFO << "FileServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected()) {
        LOG_INFO << "FileServer - Sending file" << g_file << " to " << conn->peerAddress().toIpPort();
        FILE *fp = fopen(g_file, "rb");
        if (fp) {
            FilePtr ctx(fp, fclose);
            conn->setContext(ctx);
            char buf[bufferSize];
            size_t num_read = fread(buf, 1, sizeof buf, fp);
            conn->send(buf, static_cast<int>(num_read));
        } else {
            conn->shutdown();
            LOG_INFO << "FileServer - file not found";
        }
    }
}

void onWriteComplete(const TcpConnectionPtr &conn) {
    const auto &fp = boost::any_cast<const FilePtr &>(conn->getContext());
    char buf[bufferSize];
    size_t num_read = fread(buf, 1, sizeof buf, get_pointer(fp));
    if (num_read > 0) {
        conn->send(buf, static_cast<int>(num_read));
    } else {
        conn->shutdown();
        LOG_INFO << "FileServer - done";
    }
}

int main(int argc, char *argv[]) {
    LOG_INFO << "pid = " << getpid();
    if (argc > 1) {
        g_file = argv[1];
        EventLoop loop;
        InetAddress listenAddr(3000);
        TcpServer server(&loop, listenAddr, "FileServer");
        server.setConnectionCallback(onConnection);
        server.setWriteCompleteCallback(onWriteComplete);
        server.start();
        loop.loop();
    } else {
        fprintf(stderr, "Usage: %s file_path\n", argv[0]);
    }
}
