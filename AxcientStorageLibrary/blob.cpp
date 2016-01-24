#include <AxcientStorageLibrary/blob.h>
#include <Poco/File.h>

using namespace axcient;

void Blob::write(Block& block )
{
	if(block_count >= blob_size) {
		throw std::invalid_argument("Blob already filled");
	}
	fs.write((const char*)block.getData(), block.getSize());
	block.blobPath(this->path);
	block.blobOrder(block_count);
	++block_count;
}

void Blob::read( Block& b)
{
	if(block_count <= b.getBlobOrder()) {
		throw std::invalid_argument("Blob already filled");
	}
	fs.seekp(b.getBlobOrder() * block_size);
	std::vector<byte> data((size_t)block_size);
	fs.read((char*)data.data(), block_size);
	b.setData(std::move(data));
}

std::unique_ptr<Blob> Blob::create( const Blob& b, const string path ) {
	return std::unique_ptr<Blob>(new Blob(b, path + createName()));
}

std::unique_ptr<Blob> Blob::create( block_size_t bk_z, blob_size_t bb_z
								  , blob_size_t bk_c, const string& path ) {
	return std::unique_ptr<Blob>( new Blob(bk_z, bb_z, bk_c, path + createName() ) );
}

std::unique_ptr<Blob> Blob::createForRead( const Blob& b, const string& path ) {
	return std::unique_ptr<Blob>( new Blob(b.block_size, b.blob_size, b.blob_size, path) );
}
