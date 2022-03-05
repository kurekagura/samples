#ifndef QUEUECHANNEL_H
#define QUEUECHANNEL_H

//#include <deque>
#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class QueueChannel
{
public:
    explicit QueueChannel(std::size_t max_length = default_max_length)
      : max_length_(max_length) {}

    void put(const T& val)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [this]() {return buff_.size() < max_length_;});

        buff_.push(val);
        cond_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [this]() { return !buff_.empty();});

        T val = buff_.front();
        buff_.pop();
        cond_.notify_one();
        return val;
    }

private:
    static constexpr std::size_t default_max_length = 10;
    std::size_t max_length_;
    std::queue<T> buff_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

#endif // QUEUECHANNEL_H
