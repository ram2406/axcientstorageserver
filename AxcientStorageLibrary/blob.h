#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/FileStream.h>
#include <AxcientStorageLibrary/block.h>
#include <memory>
namespace axcient {

class Blob {
	block_size_t	block_size;
	blob_size_t		blob_size;
	blob_size_t		block_count;		
	string			path;
	Poco::FileStream	fs;
	static const char		df_byte = 0;
	Blob& operator=(const Blob&) { return *this; }
	
	Blob( block_size_t		block_size
		, blob_size_t		blob_size
		, blob_size_t		block_count
		, const string&		path)
		: block_size(block_size)
		, blob_size(blob_size)
		, block_count(block_count)
		, path(path)
		, fs(path, std::ios::in | std::ios::out | std::ios::app) {

	}
	Blob(const Blob& b, const string path)
		: block_size(b.blob_size)
		, blob_size(b.blob_size)
		, block_count(0)
		, path(path)
		, fs(path, std::ios::in | std::ios::out | std::ios::app) {

	}
public:
	

	void write(Block& block);
	void read(Block& b);
	bool isFilled() const { return block_size == block_count; }
	static std::unique_ptr<Blob> create(const Blob& b, const string path);
	static std::unique_ptr<Blob> create(block_size_t block_size, blob_size_t blob_size
		, blob_size_t block_count, const string& path);
	static std::unique_ptr<Blob> createForRead(const Blob& b, const string& path);
	static std::string createName() {
		Poco::UUIDGenerator gen;
		return gen.createOne().toString();
	}
	block_size_t getBlockSize() const { return block_size; }
	blob_size_t getBlobSize() const { return blob_size; }
	string	getBlobPath() const { return path; }
};

} //axcient