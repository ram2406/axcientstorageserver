#include <AxcientStorageLibrary/storage.h>

using namespace axcient;

void Storage::PutBlock( Block& block ) {
	
	if(this->Exists(block)) {
		db.PutBlock(block);
		return;
	}
	fs.PutBlock(block);
	db.PutBlock(block);
}


void Storage::GetBlock( block_id_t id, Block &b) {
	db.GetBlock(id, b);
	fs.GetBlock(id, b);
}

void Storage::Init( block_size_t bk_z, blob_size_t bb_z) {
	db.Init(bk_z, bb_z);
	fs.Init(bk_z, bb_z);
}

void Storage::Exists( Block& b, std::vector<Block>& clones ) {
	db.Exists(b, clones);
	fs.Exists(b, clones);
	if(clones.size() > 1) {
		throw std::logic_error("clones many too");
	}
}

bool Storage::Exists( Block& b ) {
	std::vector<Block> clones;
	Exists(b, clones);
	if(clones.size()) {
		b.blobPath(clones[0].blobPath());
		b.blobOrder(clones[0].blobOrder());
		return true;
	}
	return false;
}
