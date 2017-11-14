#include <string>

#include "btNode.h"

#ifndef SRC_HEADERS_FILEIO_H_
#define SRC_HEADERS_FILEIO_H_

class btOperations {

public:
	static void writeBtToFileSystem(btNodePointer bit, std::string foldername,int depth);
	static void colapseFileSystemBt(std::string foldername,int depth);
	static void colapseInMemoryBt(btNodePointer bit,int depth);
};

#endif /* SRC_HEADERS_FILEIO_H_ */
