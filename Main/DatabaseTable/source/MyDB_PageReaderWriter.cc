
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecordIterator.h"
#include "MyDB_Table.h"

MyDB_PageHandle MyDB_PageReaderWriter :: getPage () {
	return page;
}

MyDB_PageReaderWriter :: MyDB_PageReaderWriter (MyDB_BufferManagerPtr bfMgrIn, MyDB_TablePtr whichTable, long i) {
	bfMgr = bfMgrIn;
	page = bfMgr->getPage(whichTable, i);
}

void MyDB_PageReaderWriter :: clear () {
	PageOverlay *pagePtr = (PageOverlay *)page->getBytes();
	pagePtr->numBytesUsed = 0;
	setType(MyDB_PageType :: RegularPage);
	page->wroteBytes();
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	PageOverlay *pagePtr = (PageOverlay *)page->getBytes();
	return pagePtr->pageType;
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr iterateIntoMe) {
	return make_shared<MyDB_PageRecordIterator>(*this, iterateIntoMe);
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageTypeIn) {
	PageOverlay *pagePtr = (PageOverlay *)page->getBytes();
	pagePtr->pageType = pageTypeIn;
	page->wroteBytes();
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr appendMe) {
	PageOverlay *pagePtr = (PageOverlay *)page->getBytes();

	if (pagePtr->numBytesUsed + sizeof(PageOverlay) + appendMe->getBinarySize() > bfMgr->getPageSize()) {
		return false;
	} 

	void *end = appendMe->toBinary(&(pagePtr->bytes[pagePtr->numBytesUsed + sizeof(PageOverlay)]));
	pagePtr->numBytesUsed += (char *)(end) - &(pagePtr->bytes[pagePtr->numBytesUsed+ sizeof(PageOverlay)]);
	page->wroteBytes();

	return true;
}

#endif
