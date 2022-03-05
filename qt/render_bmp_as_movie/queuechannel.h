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
      : max_length_(max_length), is_closed_(false) {}

    void push(T* val)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [this]() {return buff_.size() < max_length_;});

        buff_.push(val);
        cond_.notify_one();
    }

    T* pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        //spurious wakeupが発生した場合でも、ラムダ式がfalseを戻せばまたwaitする。
        //trueが戻ればwaitは解除される。
        cond_.wait(lock, [this]() { return !buff_.empty() || is_closed_;});

        if(buff_.empty()){
            return nullptr;
        }

        T* val = buff_.front();
        buff_.pop();
        cond_.notify_one();
        return val;
    }

    void close() {
        std::scoped_lock lock(mtx_);
        is_closed_ = true;
        cond_.notify_all();
    }

private:
    static constexpr std::size_t default_max_length = 10;
    std::size_t max_length_;
    std::queue<T*> buff_;
    std::mutex mtx_;
    std::condition_variable cond_;
    bool is_closed_;
};

#endif // QUEUECHANNEL_H
