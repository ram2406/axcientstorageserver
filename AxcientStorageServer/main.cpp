#include <iostream>
#include <Poco/DynamicAny.h>

#include <AxcientStorageLibrary/storage_api.h>
const uint16_t bz = 10;	

axcient::byte test1[bz] = {0xff, 0xaa, 0xbb, 0x00};
axcient::byte test2[bz] = {0xff, 0xdd, 0xbb, 0x00};
axcient::byte test3[bz] = {0xff, 0xaa, 0xdd, 0x00};
axcient::byte test4[bz] = {0xf3, 0xaa, 0xbb, 0xcc};
axcient::byte test5[bz] = {0xf4, 0xaa, 0xbb, 0xcc};
axcient::byte test6[bz] = {0xf5, 0xaa, 0xbb, 0xcc};
axcient::byte test7[bz] = {0xf6, 0xaa, 0xbb, 0xcc};
axcient::byte test8[bz] = {0xf7, 0xaa, 0xbb, 0xcc};
axcient::byte test9[bz] = {0xf8, 0xaa, 0xbb, 0xcc};
axcient::byte test10[bz] = {0xfa, 0xaa, 0xbb, 0xcc};
axcient::byte test11[bz] = {0xfb, 0xaa, 0xbb, 0xcc};
axcient::byte test12[bz] = {0xfc, 0xaa, 0xbb, 0xcc};

void checkData(axcient::byte bytes1[], axcient::byte bytes2[]) {
	if(		(bytes1[0] == bytes2[0])
		&&	(bytes1[1] == bytes2[1])
		&&	(bytes1[2] == bytes2[2])
		&&	(bytes1[3] == bytes2[3])) {
		std::cout << "data equal" << std::endl;
	}
	else {
		std::cout << "data not equal" << std::endl;
	}
}


int main(int argc, char* argv[]) {
	axcient::byte bytes [bz] = { 0 };
	axcient::api::init(bz, 10);
	axcient::api::put_block(1,	test1);
	axcient::api::get_block(1,	bytes);

	checkData(bytes, test1);

	axcient::api::put_block(4,	test1);
	axcient::api::put_block(5,	test1);
	axcient::api::put_block(6,	test2);
	axcient::api::put_block(7,	test3);
	axcient::api::put_block(8,	test4);
	axcient::api::put_block(9,	test5);
	axcient::api::put_block(10,	test6);
	axcient::api::put_block(11,	test7);
	axcient::api::put_block(12,	test8);
	axcient::api::put_block(13,	test9);
	axcient::api::put_block(14,	test10);
	axcient::api::put_block(15,	test2);
	axcient::api::put_block(16,	test11);
	axcient::api::put_block(17,	test12);
	axcient::api::put_block(18,	test10);

	axcient::api::get_block(10,	bytes);
	checkData(bytes, test6);
	axcient::api::get_block(12,	bytes);
	checkData(bytes, test8);

	std::system("pause");
}