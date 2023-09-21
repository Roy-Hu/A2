#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_TableReaderWriter.h"
#include "MyDB_RecordIterator.h"

class MyDB_TableRecordIterator;

typedef shared_ptr <MyDB_TableRecordIterator> MyDB_TableRecordIteratorPtr;

class MyDB_TableRecordIterator : public MyDB_RecordIterator {

public:
    MyDB_TableRecordIterator (MyDB_TableReaderWriter &tableReaderWriter, MyDB_RecordPtr recordIn, MyDB_TablePtr tableIn);
	~MyDB_TableRecordIterator() {};

	void getNext ();
	bool hasNext ();

private:

	int pageIdx;
	MyDB_TablePtr table;
	MyDB_RecordPtr record;
	MyDB_RecordIteratorPtr recordIteratorPtr;
	MyDB_TableReaderWriter &tableReaderWriter;
};

#endif
