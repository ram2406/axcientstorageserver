#pragma once
#include <AxcientStorageLibrary/axcient_defs.h>

namespace axcient { namespace api {

/*
block_size — размер передаваемого блока данных в байтах
blob_size — размер хранимого blob в количестве блоков
Метод осуществляет установку соответствующих параметров.
Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
*/
int init(block_size_t block_size, blob_size_t blob_size);

/*
block_id — уникальный идентификатор блока
block_data —набор байт размером block_size, данные блока
Метод помещает блок на хранение.
Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
*/
int put_block(block_id_t block_id, const byte* block_data);

/*
block_id — уникальный идентификатор блока
block_data — указатель на свободное место в памяти размером block_size
Метод заполняет block_data данными блока block_id
*/
int get_block(block_id_t block_id, byte* block_data);

}	//api
}	//axcient