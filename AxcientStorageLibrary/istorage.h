#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>
#include <AxcientStorageLibrary/block.h>

namespace axcient {
	class IStorage {
		static	block_size_t	block_size;
		static blob_size_t		blob_size;

		IStorage(const IStorage&) {}
		IStorage& operator= (const IStorage&) { *this; }
	protected:
		IStorage() {}
	public:
		
		static block_size_t Block_size() { return block_size; }
		static void Block_size(block_size_t val) { block_size = val; }
		static blob_size_t Blob_size() { return blob_size; }
		static void Blob_size(blob_size_t val) { blob_size = val; }

		virtual void	PutBlock (Block& block) = 0;
		virtual void	GetBlock (block_id_t id, Block&) = 0;
		virtual void	Init(block_size_t, blob_size_t) = 0;

		virtual	void	Exists(Block& b, std::vector<Block>& clones) = 0;

		static string GetPrefix () { 
			string prefix	=  std::to_string(blob_size) 
							+ L("_")
							+ std::to_string(block_size);
			return prefix;
		}
	};
}