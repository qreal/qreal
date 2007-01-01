#ifndef _ISTORAGE_H_
#define _ISTORAGE_H_ 

#include <string>

namespace Interfaces {
	class Repository {
	public:
		virtual void* getObject(int ObjId) { return NULL; };
		virtual void addObject(void *obj) { return; };
	};
};

#endif

