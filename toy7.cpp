#include <iostream>
// #include <string>
// #include <vector>
// #include <memory>
// #include <algorithm>
// #include <assert.h>
// #include <boost/variant.hpp>
// #include <array>
// #include <functional>
#include <array>
#include <thread>
#include <chrono>
// #include <tuple>
// #include <optional>
#include <future>

using namespace std::literals::chrono_literals;
#if 0
struct Timer {
    Timer () : start(std::chrono::high_resolution_clock::now())
    {  }
    ~Timer ()
    {
        std::chrono::time_point end =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end-start;
        std::cout << "now: " << duration.count() << std::endl;        
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::tuple<std::string, int> person () {
    return { "name", 77 };
}

std::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

std::optional<std::string> foo () {
    return {};
}
#endif

template<size_t N>
void wait_all (std::array<unsigned, N> waits,
               std::function<unsigned(unsigned)> each,
               std::function<void(std::array<unsigned, N>&)> done) {
    std::array<unsigned, N> results;
    std::future<unsigned> futures[N];
    unsigned pending = 0;
    for (size_t i = 0; i < N; ++i) {
        ++pending;
        auto callPause =
            [&waits, &results, &pending, i, &each, &done]()->unsigned {
                unsigned ret = each(waits[i]);
                results[i] = ret;
                if (!--pending)
                    // called in whatever thread hapens to finish last
                    done(results);
                return ret;
            };
        futures[i] = std::async(std::launch::async, callPause);
    }
}

class Random
{
public:
    Random (const Random&) = delete;
    static Random& Get()
    {
        static Random instance;
        return instance;
    }

    static float Float() { return Get().IFloat(); }
private:
    float IFloat() { return m_RandomGenerator; }
    Random() {
    std::cerr << __LINE__ << "\n";
    }

    float m_RandomGenerator = 0.5f;
};

void* operator new(size_t size)
{
    void* ptr = malloc(size);
    std::cout << "+ " << ptr << ":" << size << "\n";
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::cout << "- " << ptr << "\n";
    free(ptr);
}

int main ()
{
    std::string test = "1234567890123456";
    std::cerr << __LINE__ << "\n";
    std::cout << Random::Float() << std::endl;
    std::cerr << __LINE__ << "\n";
    std::cout << Random::Float() << std::endl;
    #if 0
    static std::mutex talkingStick;
    std::array<unsigned, 5> waits = {4, 1, 3, 4, 2};
    std::future<unsigned> futures[waits.size()];
    auto each =
        [](unsigned howLong) {
            {
                std::lock_guard<std::mutex> lock(talkingStick);
                std::cout << "0/" << howLong << std::endl;
            }
            for (unsigned i = 0; i < howLong; ++i) {
                std::this_thread::sleep_for(100ms);
                std::lock_guard<std::mutex> lock(talkingStick);
                std::cout << i+1 << '/' << howLong << std::endl;
            }
            return howLong * 2;
        };
    // vv this barfs when auto vv
    std::function<void(std::array<unsigned, waits.size()>&)> done =
        [](std::array<unsigned, waits.size()> &results) {
              std::cout << "done" << std::endl;
              for (size_t r = 0; r < results.size(); ++r)
                  std::cout << r << ": " << results[r] << std::endl;
          };
    for (size_t i = 0; i < waits.size(); ++i) {
        futures[i] = std::async(std::launch::async, each);
    }
    std::future<bool> d1 = std::experimental::when_all(futures).then(doen());
    // wait_all(futures, each, done);
    #endif
    return 0;
#if 0
    auto[gpa, grade, name] = get_student(0);
    // double gpa;
    // char grade;
    // std::string name;    
    // std::tie(gpa, grade, name) = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << gpa << ", "
              << "grade: " << grade << ", "
              << "name: " << name << '\n'; 

    std::optional<std::string> ret = foo();
    std::cout << "ret: " << (ret ? *ret : "N/A") << std::endl;
    Timer t;
    // std::this_thread::sleep_for(1s);

    auto[label, height] = person();
    std::cout << label << height << std::endl;
    return 0;
#endif
}
