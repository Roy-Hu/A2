#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"

using namespace std;

MyDB_PageRecordIterator :: MyDB_PageRecordIterator (MyDB_PageReaderWriter &pageReaderWriterIn, MyDB_RecordPtr recordIn) : pageReaderWriter(pageReaderWriterIn){
    record = recordIn;
    bytesRead = 0;

    page = pageReaderWriter.getPage();
}

void MyDB_PageRecordIterator :: getNext () {
	if (hasNext()) {
        PageOverlay *pagePtr = (PageOverlay *)page->getBytes();

        void *nextPos = record->fromBinary(&(pagePtr->bytes[bytesRead + sizeof(PageOverlay)]));
        bytesRead += (char *)nextPos - &pagePtr->bytes[bytesRead + sizeof(PageOverlay)];
    }
}

bool MyDB_PageRecordIterator :: hasNext () {
    PageOverlay *pagePtr = (PageOverlay *)page->getBytes();
    if (bytesRead < pagePtr->numBytesUsed) {
        return true;
    }

    return false;
}

#endif
