#ifndef BOOST_UUID_DETAIL_BASIC_NAME_GENERATOR_HPP_INCLUDED
#define BOOST_UUID_DETAIL_BASIC_NAME_GENERATOR_HPP_INCLUDED

// Boost basic_name_generator.hpp header file  -----------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
//  https://www.boost.org/LICENSE_1_0.txt)

#include <boost/uuid/namespaces.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/static_assert.hpp>
#include <boost/config.hpp>
#include <string>
#include <cstdint>
#include <cstring> // for strlen, wcslen

namespace boost {
namespace uuids {
namespace detail {

template<class HashAlgo>
class basic_name_generator
{
private:

    uuid namespace_uuid_;

public:

    typedef uuid result_type;
    typedef typename HashAlgo::digest_type digest_type;

    explicit basic_name_generator( uuid const& namespace_uuid ) noexcept
        : namespace_uuid_( namespace_uuid )
    {}

    uuid operator()( char const* name ) const noexcept
    {
        HashAlgo hash;

        hash.process_bytes( namespace_uuid_.begin(), namespace_uuid_.size() );
        process_characters( hash, name, std::strlen( name ) );

        return hash_to_uuid( hash );
    }

    uuid operator()( wchar_t const* name ) const noexcept
    {
        HashAlgo hash;

        hash.process_bytes( namespace_uuid_.begin(), namespace_uuid_.size() );
        process_characters( hash, name, std::wcslen( name ) );

        return hash_to_uuid( hash );
    }

    template<class Ch, class Traits, class Alloc>
    uuid operator()( std::basic_string<Ch, Traits, Alloc> const& name ) const noexcept
    {
        HashAlgo hash;

        hash.process_bytes( namespace_uuid_.begin(), namespace_uuid_.size() );
        process_characters( hash, name.c_str(), name.length() );

        return hash_to_uuid( hash );
    }

    uuid operator()( void const* buffer, std::size_t byte_count ) const noexcept
    {
        HashAlgo hash;

        hash.process_bytes( namespace_uuid_.begin(), namespace_uuid_.size() );
        hash.process_bytes( buffer, byte_count );

        return hash_to_uuid( hash );
    }

private:
    // we convert all characters to uint32_t so that each
    // character is 4 bytes regardless of sizeof(char) or
    // sizeof(wchar_t).  We want the name string on any
    // platform / compiler to generate the same uuid
    // except for char
    template<class Ch>
    void process_characters( HashAlgo& hash, Ch const* characters, std::size_t count ) const noexcept
    {
        BOOST_UUID_STATIC_ASSERT( sizeof(std::uint32_t) >= sizeof(Ch) );

        for( std::size_t i = 0; i < count; ++i)
        {
            std::size_t c = characters[ i ];

            hash.process_byte( static_cast<unsigned char>( (c >>  0) & 0xFF ) );
            hash.process_byte( static_cast<unsigned char>( (c >>  8) & 0xFF ) );
            hash.process_byte( static_cast<unsigned char>( (c >> 16) & 0xFF ) );
            hash.process_byte( static_cast<unsigned char>( (c >> 24) & 0xFF ) );
        }
    }

    void process_characters( HashAlgo& hash, char const* characters, std::size_t count ) const noexcept
    {
        hash.process_bytes( characters, count );
    }

    uuid hash_to_uuid( HashAlgo& hash ) const noexcept
    {
        digest_type digest;
        hash.get_digest(digest);

        BOOST_UUID_STATIC_ASSERT( sizeof(digest_type) >= 16 );

        uuid u;
        std::memcpy( u.data, digest, 16 );

        // set variant: must be 0b10xxxxxx
        *(u.begin()+8) &= 0xBF;
        *(u.begin()+8) |= 0x80;

        // set version
        unsigned char hashver = hash.get_version();
        *(u.begin()+6) &= 0x0F;             // clear out the relevant bits
        *(u.begin()+6) |= (hashver << 4);   // and apply them

        return u;
    }
};

} // namespace detail
} // uuids
} // boost

#endif // BOOST_UUID_DETAIL_BASIC_NAME_GENERATOR_HPP_INCLUDED
