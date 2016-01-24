#include <AxcientStorageLibrary/file_storage.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/File.h>
using namespace axcient;

void FileStorage::PutBlock( Block& block )
{
	current_blob->write(block);
	if(current_blob->isFilled()) {
		const auto& b = *current_blob;
		current_blob = std::move(Blob::create(b, storage_path));
	}
}

void FileStorage::GetBlock( block_id_t id, Block& b) {
	if(b.blobPath() == current_blob->getBlobPath()) {
		current_blob->read(b);
		return;
	}
	auto bb = Blob::createForRead(*current_blob, b.getBlobPath());
	bb->read(b);
	
}

void FileStorage::Init( block_size_t bk_z, blob_size_t bb_z)
{
	storage_path	= L("storage/") + IStorage::GetPrefix() + L("/");

	Poco::File f (storage_path);
	f.createDirectories();

	current_blob = std::move(Blob::create(bk_z, bb_z, 0, storage_path));
}

void FileStorage::Exists( Block& b, std::vector<Block>& clones ) {
	Block* finded = nullptr;
	for(size_t ci = 0, clen = clones.size(); ci < clen; ++ci) {
		Block& bc = clones[ci];
		GetBlock(bc.getId(), bc);
		if(b.dataCompare(bc)) {
			finded = &bc;
			break;
		}
	}
	if(finded) {
		Block bc = std::move(*finded);
		clones.clear();
		clones.emplace_back(std::move(bc));
		return;
	}
	clones.clear();
}
