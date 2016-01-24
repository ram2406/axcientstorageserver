#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>
#include <AxcientStorageLibrary/istorage.h>
#include <Poco/Data/Session.h>
#include <memory>

namespace axcient {

class DataBaseStorage 
	: public IStorage {
	std::unique_ptr<Poco::Data::Session> session;
	DataBaseStorage(const DataBaseStorage&) {}
public:
	DataBaseStorage() {}
	DataBaseStorage(DataBaseStorage&& db)
		: session(std::move(db.session)) {

	}

	void	PutBlock (Block& block) override;
	void	GetBlock (block_id_t id, Block&) override;
	void	Init(block_size_t, blob_size_t) override;
	void	Exists(Block& b, std::vector<Block>& clones) override;
};

}	//axcient