
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_PageReaderWriter.h"

class MyDB_PageRecordIterator;

typedef shared_ptr <MyDB_PageRecordIterator> MyDB_PageRecordIteratorPtr;

class MyDB_PageRecordIterator : public MyDB_RecordIterator {

public:
    MyDB_PageRecordIterator (MyDB_PageReaderWriter &pageReaderWriterIn, MyDB_RecordPtr recordIn);
	~MyDB_PageRecordIterator() {};

	void getNext ();
	bool hasNext ();

private:
    int bytesRead;

	MyDB_PageHandle page;
	MyDB_RecordPtr record;
	MyDB_PageReaderWriter &pageReaderWriter;

};

#endif
