#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_TableRecordIterator.h"
#include "MyDB_PageRecordIterator.h"

using namespace std;

MyDB_TableRecordIterator :: MyDB_TableRecordIterator (MyDB_TableReaderWriter &tableReaderWriterIn, MyDB_RecordPtr recordIn, MyDB_TablePtr tableIn) : tableReaderWriter(tableReaderWriterIn) {
    pageIdx = 0;
    table = tableIn;
    record = recordIn;
    recordIteratorPtr = ((tableReaderWriter[pageIdx]).getIterator(record));
}

void MyDB_TableRecordIterator :: getNext () {
    recordIteratorPtr->getNext();
}

bool MyDB_TableRecordIterator :: hasNext () {
    if (recordIteratorPtr->hasNext()) {
        return true;
    }

    while (pageIdx < table->lastPage()) {
        recordIteratorPtr = (tableReaderWriter[++pageIdx]).getIterator(record);

        if (recordIteratorPtr->hasNext()) {
            return true;
        } 
    }

    return false;
}

#endif
