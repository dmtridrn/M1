#include <deque>
#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <random>
#include <condition_variable>
#include <chrono>

using namespace std;

template <typename T>
class BoundedBlockingQueue
{
    deque<T> queue_;
    size_t capacity_;
    mutable mutex m;
    condition_variable cond;

public:
    explicit BoundedBlockingQueue(size_t capacity)
        : capacity_(capacity) {}

    void push(const T &value){
        { // critical section
            std::unique_lock lock(m);
            cond.wait(lock, [this]
                      { return queue_.size() < capacity_; });
            queue_.push_back(value);
        }
        cond.notify_all(); // notify after releasing lock
    }

    T pop(){
        std::unique_lock lock(m);
        cond.wait(lock, [this]
                  { return !queue_.empty(); });
        T value = queue_.front();
        queue_.pop_front();
        // explicit unlock : the critical section ends here, value stays alive for the return.
        // unique_lock::unlock is handy whenever the end of the critical section is not the end of
        // a block, typically to notify outside the critical section without nesting a block.
        lock.unlock();
        cond.notify_all(); // notify after releasing lock
        return value;
    }
};

int main()
{
    // Une file de capacité 3 : le producteur va devoir bloquer régulièrement !
    BoundedBlockingQueue<int> q(3);

    // Thread producteur : pousse 10 entiers
    std::thread producteur([&q]()
                           {
        for (int i = 1; i <= 10; ++i) {
            q.push(i);
            std::cout << "[Prod] Ajouté : " << i << std::endl;
        } });

    // Thread consommateur : retire 10 entiers (plus lentement que le producteur)
    std::thread consommateur([&q]()
                             {
        for (int i = 1; i <= 10; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            int val = q.pop();
            std::cout << "[Cons] Retiré : " << val << std::endl;
        } });

    producteur.join();
    consommateur.join();

    return 0;
}