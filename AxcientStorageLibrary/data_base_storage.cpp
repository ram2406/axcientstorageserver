#include <AxcientStorageLibrary/data_base_storage.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/RecordSet.h>

static struct hack {
	hack() {	Poco::Data::SQLite::Connector::registerConnector();	}
	~hack() {	Poco::Data::SQLite::Connector::unregisterConnector();	}
} hack;

using namespace axcient;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;


void axcient::DataBaseStorage::PutBlock( Block& block )
{
	Statement stmt (*session);
	stmt	<< "INSERT OR REPLACE INTO block(id, blob, blob_order, crc32) VALUES ("
			<< block.getId() << ", "
			<< "\"" << block.getBlobPath() << "\", "
			<< block.getBlobOrder() << ", "
			<< block.getCrc32() << ");";
	stmt.execute(true);
}

void axcient::DataBaseStorage::GetBlock( block_id_t id, Block& b)
{
	long border, crc32;
	std::string blob;
	Statement stmt (*session);
	stmt	<< "SELECT blob, blob_order, crc32 "
			<< "FROM block "
			<< "WHERE id = " << id
			, into(blob), into(border), into(crc32);
	if(stmt.execute(true)) {
		b.setCrc32((crc32_t)crc32);
		b.blobPath(blob);
		b.blobOrder(border);
	}
	else {
		throw std::invalid_argument("block id not found");
	}
}

void axcient::DataBaseStorage::Init( block_size_t, blob_size_t )
{
	const string& prefix = IStorage::GetPrefix();
	session.reset(new Session("SQLite", prefix + L("_storage.db")));
	*session << "CREATE TABLE IF NOT EXISTS block (id INTEGER PRIMARY KEY, blob TEXT, blob_order INTEGER, crc32 INTEGER);"
		//<< "CREATE TABLE IF NOT EXISTS blob (id INTEGER PRIMARY KEY, name TEXT, blocks INTEGER);"
		<< "CREATE INDEX IF NOT EXISTS block_crc32_index ON block (crc32);"
		, now;

}

void DataBaseStorage::Exists( Block& b, std::vector<Block>& clones )
{
	long border, id;
	std::string blob;
	Statement stmt (*session);
	stmt	<< "SELECT id, blob, blob_order "
			<< "FROM block "
			<< "WHERE crc32 = " << b.getCrc32() << ";";
	stmt.execute();
	Poco::Data::RecordSet rs (stmt);
	for(Poco::Data::Row& row : rs) {
		 row.get(0).convert(id);
		 row.get(1).convert(blob);
		 row.get(2).convert(border);

		 auto block = Block::create(id, border, blob, b.getCrc32());
		 clones.emplace_back(std::move(block));
	}
}
