#pragma once
///c
#include <stdint.h>

///stl
#include <string>

///POCO
#include <Poco/UUID.h>

#define L(x)	x

extern "C" uint32_t calc_crc32(uint32_t, const void *, uint64_t);

namespace axcient {

	typedef Poco::UUID UUID;
	typedef std::string string;
	typedef uint8_t byte;
	typedef uint64_t block_size_t;
	typedef uint32_t blob_size_t;
	typedef uint32_t block_id_t;
	typedef uint32_t crc32_t;
	inline uint32_t block_crc32(uint32_t crc, const byte *buf, block_size_t size) {
		return calc_crc32(crc, (const void*)buf, (uint64_t)size);
	}	
} //axcient