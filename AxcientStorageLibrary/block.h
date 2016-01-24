#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>
#include <vector>

namespace axcient {
	class Block {
		block_id_t			id;
		std::vector<byte>	data;
		crc32_t				crc32;
		
		string				blob_path;
		blob_size_t			blob_order;
		
		Block(block_id_t id) : id(id), crc32(0), blob_order(0) {} 
		Block(block_id_t id, std::vector<byte>&& data, crc32_t crc32)
			: id(id), data(std::move(data)), crc32(crc32), blob_order(0) {
		}
		Block(block_id_t id, blob_size_t blob_order, string blob_path, crc32_t crc32)
			: id(id), blob_order(blob_order), blob_path(blob_path), crc32(crc32) {
		}
		Block(Block& b) {}
	public:
		Block(Block&& b) 
			: id(b.id), data(std::move(b.data)), crc32(b.crc32)
			, blob_order(b.blob_order), blob_path(std::move(b.blob_path)) {}
		static Block	create(block_id_t id);
		static Block	create(block_id_t id, const byte* data, block_size_t size);
		static Block	create(block_id_t id, blob_size_t blob_order, string blob_path, crc32_t crc32);
		static inline uint32_t calc_crc32(const byte *buf, block_size_t size) { return block_crc32(42, buf, size); }

		const byte* getData () const { return data.data(); }
		size_t getSize () const { return data.size(); }
		crc32_t getCrc32 () const { return crc32; }
		void	setCrc32 (crc32_t val)  { crc32 = val; }

		string blobPath() const { return blob_path; }
		void blobPath(const string& val) { blob_path = val; }

		blob_size_t blobOrder() const { return blob_order; }
		void blobOrder(blob_size_t val) { blob_order = val; }

		block_id_t getId () const { return id; }
		const string& getBlobPath() const { return blob_path; }
		blob_size_t getBlobOrder() const { return blob_order; }

		void setData(std::vector<byte>&& data) { this->data = data; }

		void copyTo (byte* b, block_size_t sz) const {
			stdext::checked_array_iterator<byte *> chkd_test_array(b, (size_t)sz);
			std::copy(data.begin(), data.begin() +(size_t)sz, chkd_test_array); 
		}
		bool dataCompare (const Block& o) const { return data == o.data; }
	};
}	//axcient