#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Exception.h>
#include <Poco/ThreadPool.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <AxcientStorageLibrary/storage_api.h>

#include <unordered_map>
#include <iostream>
#include <regex>


const axcient::block_size_t BLOCK_SIZE = 1024;
const axcient::blob_size_t BLOB_SIZE = 10;
const uint16_t	PORT_NUMBER = 9765;

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

class EmptyRequestHandler
	: public HTTPRequestHandler {
public:
	EmptyRequestHandler() {	}

	void handleRequest(HTTPServerRequest& request,
		HTTPServerResponse& response) { }

};


class StorageRequestHandler
	: public HTTPRequestHandler {
	axcient::block_size_t block_size;
	axcient::blob_size_t blob_size;
	size_t block_id;
	static const size_t memory_for_header_raw = 512;
public:
	StorageRequestHandler(axcient::block_size_t bk_z, axcient::blob_size_t bb_z, size_t block_id)
		: block_size(bk_z), blob_size(bb_z), block_id(block_id)	{
	}

	void handleRequest(HTTPServerRequest& request,
		HTTPServerResponse& response) {
			try {
				Application& app = Application::instance();
				app.logger().information("Request from "
					+ request.clientAddress().toString());
				

				if(request.getMethod() == HTTPServerRequest::HTTP_POST) {
					std::vector<axcient::byte> data( (size_t)block_size +memory_for_header_raw);
					request.stream().read((char*)data.data(), data.size());
					size_t start_body = body(data);
					if(axcient::api::put_block(block_id, data.data() +start_body)) {
						throw std::bad_exception("axcient::api::put_block failed");
					}
					request.clear();
					response.setChunkedTransferEncoding(true);
					response.setContentType("text/html");
					response.send() << "";
				}
				else if(request.getMethod() == HTTPServerRequest::HTTP_GET) {
					std::vector<axcient::byte> data( (size_t)block_size );
					if(axcient::api::get_block(block_id, data.data())) {
						throw std::bad_exception("axcient::api::get_block failed");
					}
					response.send().write((char*)data.data(), data.size());
				}
			}
			catch (std::exception& ex) {
				std::cout << ex.what() << std::endl;
				response.setStatus(HTTPServerResponse::HTTP_EXPECTATION_FAILED);
			}
			catch (...) {
				response.setStatus(HTTPServerResponse::HTTP_EXPECTATION_FAILED);
			}
	}
	size_t body (const std::vector<axcient::byte>& data) {
		const uint8_t spec_cnt = 4;
		for(size_t di = 0, dn = data.size(); di < dn; ++di) {
			if(di +spec_cnt < dn 
				&& data[di   ] == '\r' && data[di +1] == '\n'
				&& data[di +2] == '\r' && data[di +3] == '\n'
				) {
				if(di +4 > memory_for_header_raw) {
					throw std::bad_exception("memory for header is small");
				}
				return di +spec_cnt;
			}
		}
		throw std::bad_exception("request data without body");
	}
};

class StorageRequestHandlerFactory
	: public HTTPRequestHandlerFactory {

	axcient::block_size_t block_size;
	axcient::blob_size_t blob_size;
public:
	StorageRequestHandlerFactory(axcient::block_size_t bk_z, axcient::blob_size_t bb_z )
		: block_size(bk_z), blob_size(bb_z)	{
	}

	HTTPRequestHandler* createRequestHandler(
		const HTTPServerRequest& request)
	{
		const auto& uri = request.getURI();
		std::regex reg("\\/(\\d+)\\/?");
		std::smatch match;
		if (std::regex_search(uri, match, reg) && match.size() > 1) {
			size_t bid = std::atoi(match.str(1).c_str());
			return new StorageRequestHandler(block_size, blob_size, bid);
		}
		return new EmptyRequestHandler();
	}
};

class HTTPStorageServer
	: public Poco::Util::ServerApplication {

public:
	HTTPStorageServer() { }

	~HTTPStorageServer() { }

protected:
	void initialize(Application& self) {
		loadConfiguration();
		ServerApplication::initialize(self);
	}

	void uninitialize() {
		ServerApplication::uninitialize();
	}

	int main(const std::vector<std::string>& args) {
		ServerSocket svs(PORT_NUMBER);
		HTTPServer srv(
			new StorageRequestHandlerFactory(BLOCK_SIZE, BLOB_SIZE)
			, 	svs
			,	new HTTPServerParams);

		srv.start();
		waitForTerminationRequest();
		srv.stop();

		return Application::EXIT_OK;
	}
};

int main(int argc, char** argv) {
	axcient::api::init(BLOCK_SIZE, BLOB_SIZE);

	HTTPStorageServer app;
	return app.run(argc, argv);
}