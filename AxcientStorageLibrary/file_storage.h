#pragma once
#include <AxcientStorageLibrary/istorage.h>
#include <AxcientStorageLibrary/blob.h>
#include <memory>
namespace axcient {
	class FileStorage
		: public IStorage {
		string	storage_path;
		std::unique_ptr<Blob>	current_blob;
		
	public:
		FileStorage() {}
		FileStorage(FileStorage&& fs) 
			: storage_path(std::move(fs.storage_path))
			, current_blob(std::move(fs.current_blob)) {

		}
		void	PutBlock (Block& block) override;
		void	GetBlock (block_id_t id, Block&) override;
		void	Init(block_size_t, blob_size_t) override;
		void	Exists(Block& b, std::vector<Block>& clones) override;
	};
} //axcient