#include <AxcientStorageLibrary/block.h>

using namespace axcient;


Block Block::create( block_id_t id ) {
	return Block(id);
}

Block Block::create( block_id_t id, const byte* data, block_size_t size ) {
	std::vector<byte> d (data, data +size);
	Block b(id, std::move(d), calc_crc32(data, size));
	return std::move(b);
}

Block Block::create( block_id_t id, blob_size_t blob_order, string blob_path, crc32_t crc32 ) {
	return Block(id, blob_order, blob_path, crc32);
}
