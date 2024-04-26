// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <atomic>
#include <chrono>
#include <cstdint>

struct state_type
{
    std::uint64_t timestamp;
    std::uint16_t clock_seq;
};

state_type get_new_state( state_type const& oldst ) noexcept
{
    state_type newst( oldst );

    std::uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    if( timestamp <= newst.timestamp )
    {
        newst.clock_seq = ( newst.clock_seq + 1 ) & 0x3FFF;
    }

    newst.timestamp = timestamp;

    return newst;
}

state_type generate( std::atomic<state_type>* ps ) noexcept
{
    auto oldst = ps->load( std::memory_order_relaxed );

    for( ;; )
    {
        auto newst = get_new_state( oldst );

        if( ps->compare_exchange_strong( oldst, newst, std::memory_order_relaxed, std::memory_order_relaxed ) )
        {
            return newst;
        }
    }
}

int main()
{
    int const N = 1024;

    std::atomic<state_type> state{{ 0, 0x2222 }};

    for( int i = 0; i < N; ++i )
    {
        generate( &state );
    }
}
