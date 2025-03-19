//
// Created by Administrator on 2025/2/18.
//
#include "httplib.h"
#include "AsyncHTTPWorker.h"

#include "CLog.h"
#include "Plugin.h"

AsyncHTTPTask::AsyncHTTPTask(const std::string &_host, const std::string &_path, AsyncHTTPMethod _method,
                             const AsyncHTTPCallback_t &_callback) : host(_host), path(_path), method(_method), callback(_callback) {
}
AsyncHTTPWorker::~AsyncHTTPWorker() {
    running = false;
    cv.notify_all();
    for (auto& it : threadPool) {
        if (it.joinable()) {
            it.join();
        }
    }
}

AsyncHTTPWorker::AsyncHTTPWorker() {
    running = true;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threadPool[i] = std::thread(&AsyncHTTPWorker::work, this);
    }
}

void AsyncHTTPWorker::emplace(const AsyncHTTPTask &task) {
    //获得锁
    queueMutex.lock();
    taskQueue.emplace(task);
    queueMutex.unlock();
    cv.notify_one();
}

void AsyncHTTPWorker::retriveAndCall() {
    callMutex.lock();
    while (!callQueue.empty()) {
        auto r = std::move(callQueue.front());
        callQueue.pop();
        std::get<AsyncHTTPCallback_t>(r)(
            std::get<bool>(r),
            std::get<HTTPResponse>(r)
            );
    }
    callMutex.unlock();
}

void AsyncHTTPWorker::work() {
    while (running) {
        std::unique_lock lock(queueMutex);
        //如果条件为true，则唤醒线程
        cv.wait(lock, [this]() { return !taskQueue.empty() || !running; });
        //在等待时，queueMutex 被释放，
        //其他线程能够继续修改 taskQueue，
        //而唤醒时，cv.wait() 会重新获取锁，确保线程继续执行之前对共享资源的访问是同步的。

        if (!running)
            return;
        if (!taskQueue.empty()) {
            AsyncHTTPTask task = taskQueue.front();
            taskQueue.pop();
            lock.unlock();

            httplib::Client cli(task.host);
            httplib::Result result;
            if (task.method == METHOD_GET) {
                result = cli.Get(task.path);
            }
            else if (task.method == METHOD_POST) {
                result = cli.Post(task.path, task.params);
            }

            callMutex.lock();
            if (result.error() == httplib::Error::Success) {
                callQueue.emplace(task.callback, true, HTTPResponse(result->status,result->body));
            } else {
                callQueue.emplace(task.callback, false, HTTPResponse(0,""));
            }
            callMutex.unlock();
        }
    }
}


