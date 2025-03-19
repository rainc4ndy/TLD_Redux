//
// Created by Administrator on 2025/2/18.
//

#ifndef ASYNCHTTPWORKER_H
#define ASYNCHTTPWORKER_H


#include <functional>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#include <map>

enum AsyncHTTPMethod {
    METHOD_GET,
    METHOD_POST
};

struct HTTPResponse {
    int code;
    std::string body;
    HTTPResponse(int _code, std::string _body) : code(_code), body(_body) {}
};

using AsyncHTTPCallback_t = std::function<void(bool,HTTPResponse)>;

class AsyncHTTPTask {
public:
    std::string host;
    std::string path;
    AsyncHTTPMethod method;
    AsyncHTTPCallback_t callback;
    std::multimap<std::string, std::string> params;
    AsyncHTTPTask(
        const std::string& _host,
        const std::string& _path,
        AsyncHTTPMethod _method,
        const AsyncHTTPCallback_t &_callback);
};

class AsyncHTTPWorker {
    static constexpr int THREAD_COUNT = 3;
    std::queue<AsyncHTTPTask> taskQueue;
    std::array<std::thread, THREAD_COUNT> threadPool;
    std::mutex queueMutex; //队列锁
    std::condition_variable cv; //worker条件变量

    std::atomic<bool> running;
    void work(); //线程主函数

    std::mutex callMutex; //队列锁 (结果队列用的)
    std::queue<std::tuple<AsyncHTTPCallback_t, bool, HTTPResponse>> callQueue;

public:
    AsyncHTTPWorker();
    ~AsyncHTTPWorker();

    // 添加新任务
    void emplace(const AsyncHTTPTask &task);

    void retriveAndCall();
};

#endif //ASYNCHTTPWORKER_H
