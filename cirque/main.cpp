#include <cassert>
#include <atomic>
#include <thread>
#include <vector>
#include <unistd.h>
#include "ring_buffer.h"

constexpr char err[] = "queue is full";
constexpr int  errlen = sizeof(err);

ring_buffer<uint16_t> rb(100);
ring_buffer<uint16_t> oo(100);
bool stop = false;

auto prnt= [&]()
{
    char msg[100] = {0};
    for (size_t i = 0; !stop; ++i)
    {
        if (!oo.pop_front(msg, sizeof(msg)))
            continue;
        std::cout << "cons: " << msg << std::endl;
    }
};

auto prod = [&](const char* msg)
{
    int  msglen = std::strlen(msg) + 1;
    for (size_t i = 0; !stop; ++i)
    {
        if (!rb.push_back(msg, msglen))
            oo.push_back(err, errlen);
        if (i % 100 == 0)
            usleep(1);
    }
};

auto cons = [&]()
{
    char msg[100] = {0};
    for (size_t i = 0; !stop; ++i)
    {
        if (rb.pop_front(msg, sizeof(msg)))
            oo.push_back(msg, std::strlen(msg)+1);
    }
};

int main(int argc, char**)
{
    assert(argc == 1);
    std::vector<std::thread> workers;
    workers.emplace_back(prnt);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(cons);
    workers.emplace_back(prod, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    workers.emplace_back(prod, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    workers.emplace_back(prod, "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
    workers.emplace_back(prod, "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    workers.emplace_back(prod, "ggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg");
    workers.emplace_back(prod, "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");
    workers.emplace_back(prod, "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
    workers.emplace_back(prod, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
    workers.emplace_back(prod, "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
    workers.emplace_back(prod, "sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss");
    workers.emplace_back(prod, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
    workers.emplace_back(prod, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
    for (auto& w : workers)
        w.join();
    return 0;
}

