#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>

namespace axcient { namespace api {

/*
block_size � ������ ������������� ����� ������ � ������
blob_size � ������ ��������� blob � ���������� ������
����� ������������ ��������� ��������������� ����������.
������������ ��������: 0 � ������ ������, ����� �������� �� 0 � ������ �������.
*/
int init(block_size_t block_size, blob_size_t blob_size);

/*
block_id � ���������� ������������� �����
block_data ������ ���� �������� block_size, ������ �����
����� �������� ���� �� ��������.
������������ ��������: 0 � ������ ������, ����� �������� �� 0 � ������ �������.
*/
int put_block(block_id_t block_id, const byte* block_data);

/*
block_id � ���������� ������������� �����
block_data � ��������� �� ��������� ����� � ������ �������� block_size
����� ��������� block_data ������� ����� block_id
*/
int get_block(block_id_t block_id, byte* block_data);

}	//api
}	//axcient