
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <sstream>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_TableRecordIterator.h"

using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer) {
	table = forMe;
	bfMgr = myBuffer;

	if (table->lastPage() == -1) {
		table->setLastPage(0);
		MyDB_PageReaderWriterPtr pageReaderWriter = make_shared <MyDB_PageReaderWriter> (bfMgr, table, table->lastPage());
		pageReaderWriter->clear();
	}
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: operator [] (size_t i) {
	MyDB_PageReaderWriterPtr pageReaderWriter = make_shared <MyDB_PageReaderWriter> (bfMgr, table, table->lastPage());

	while (table->lastPage() < i) {		
		table->setLastPage(table->lastPage() + 1);
		pageReaderWriter = make_shared <MyDB_PageReaderWriter>  (bfMgr, table, table->lastPage());
		pageReaderWriter->clear();
	}

	return  *(make_shared <MyDB_PageReaderWriter> (bfMgr, table, i));	
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
	return make_shared <MyDB_Record> (table->getSchema());
}

MyDB_PageReaderWriter MyDB_TableReaderWriter :: last () {
	return *(make_shared <MyDB_PageReaderWriter>  (bfMgr, table, table->lastPage()));
}

void MyDB_TableReaderWriter :: append (MyDB_RecordPtr appendMe) {
	MyDB_PageReaderWriterPtr pageReaderWriter = make_shared <MyDB_PageReaderWriter> (bfMgr, table, table->lastPage());

	while (!pageReaderWriter->append(appendMe)) {		
		table->setLastPage(table->lastPage() + 1);
		pageReaderWriter = make_shared <MyDB_PageReaderWriter>  (bfMgr, table, table->lastPage());
		pageReaderWriter->clear();
	}
}

void MyDB_TableReaderWriter :: loadFromTextFile (string fNameIn) {
	string fName = fNameIn;

	// try to open the file
	string line;
	ifstream myfile (fName);
	
	table->setLastPage(0);
	MyDB_PageReaderWriterPtr pageReaderWriter = make_shared <MyDB_PageReaderWriter> (bfMgr, table, table->lastPage());
	pageReaderWriter->clear();

	// if we opened it, read the contents
	if (myfile.is_open()) {
		MyDB_RecordPtr record = getEmptyRecord();	
		string line;
		// loop through all of the lines
		while (getline (myfile, line)) {
			record->fromString(line);
			append(record);
		}

		myfile.close();
	}
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr recordIn) {
	record = recordIn;
	return make_shared <MyDB_TableRecordIterator> (*this, record, table);
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string fNameIn) {
	string fName = fNameIn;

	// try to open the file
	string line;
	ofstream myfile (fName);

	if (myfile.is_open()) { 
		MyDB_RecordPtr recordPtr = getEmptyRecord();	
		MyDB_RecordIteratorPtr iter = getIterator(recordPtr);

		while (iter->hasNext()) {
			iter->getNext();
			ostringstream stream;
            stream << recordPtr;
            myfile << stream.str() << endl;
		}
	}

	myfile.close();
}

#endif

