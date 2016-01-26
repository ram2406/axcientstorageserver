#include <AxcientStorageLibrary/storage_api.h>
#include <AxcientStorageLibrary/storage.h>

using namespace axcient;
using namespace axcient::api;

enum ERROR_CODE {
	
	ERROR_NONE = 0,
	ERROR_ARGUMENT_INVALID,
	ERROR_UNKNOWN = 99
};

int axcient::api::init(uint64_t block_size, uint32_t blob_size) try {
	if(!block_size || !blob_size) {
		return ERROR_ARGUMENT_INVALID;
	}
	Storage::Block_size(block_size);
	Storage::Blob_size(blob_size);
	return ERROR_NONE;
}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return ERROR_UNKNOWN;
}
catch (...) {
	std::cout << "unknown error" << std::endl;
	return ERROR_UNKNOWN;
}

	


int axcient::api::put_block(block_id_t block_id, const byte* block_data) try {
	if(!block_id || !block_data) {
		return ERROR_ARGUMENT_INVALID;
	}
	auto block = std::move(Block::create(block_id, block_data, storage().Block_size()));
	storage().PutBlock(block);
	return ERROR_NONE;
}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return ERROR_UNKNOWN;
}
catch (...) {
	std::cout << "unknown error" << std::endl;
	return ERROR_UNKNOWN;
}
	


int axcient::api::get_block(block_id_t block_id, byte* block_data) try {
	if(!block_id || !block_data) {
		return ERROR_ARGUMENT_INVALID;
	}
	auto block = std::move(Block::create(block_id));
	storage().GetBlock(block_id, block);
	block.copyTo(block_data, storage().Block_size());
	return ERROR_NONE;
}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return ERROR_UNKNOWN;
}
catch (...) {
	std::cout << "unknown error" << std::endl;
	return ERROR_UNKNOWN;
}
