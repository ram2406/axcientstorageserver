#pragma once

#include <AxcientStorageLibrary/axcient_defs.h>
#include <AxcientStorageLibrary/blob.h>
#include <AxcientStorageLibrary/data_base_storage.h>
#include <AxcientStorageLibrary/block.h>
#include <AxcientStorageLibrary/istorage.h>
#include <AxcientStorageLibrary/file_storage.h>

namespace axcient {
	class Storage 
		: public IStorage {

		string			storage_path;
	
		DataBaseStorage		db;
		FileStorage			fs;

		Storage	() {}
		Storage& operator=(const Storage&) {}
		//Storage(const Storage& s) : db(s.db) {}
		Storage(Storage&& s) 
			: storage_path(std::move(s.storage_path))
			, db(std::move(s.db))
			, fs(std::move(s.fs))
		{
		}
	public:

		static Storage&	instance () {
			static Storage	s = std::move(Storage::create());
			return s;
		}
		static Storage create() {
			Storage s;
			if(!IStorage::Block_size() || !IStorage::Blob_size()) {
				throw std::invalid_argument("need axcient::api::init before it");
			}
			s.Init(IStorage::Block_size(), IStorage::Blob_size());
			return s;
		}
		

		void	PutBlock (Block& block) override;
		void	GetBlock (block_id_t id, Block&) override;
		void	Init(block_size_t, blob_size_t) override;
		void	Exists(Block& b, std::vector<Block>& clones) override;
		bool	Exists(Block& b);
	};

	inline Storage& storage() { return Storage::instance(); }

}	//axcient