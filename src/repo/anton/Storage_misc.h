#ifndef STORAGE_MISC_H
#define STORAGE_MISC_H

#include "entity_types.h"

class R_ObjId
{
	public:
		static R_ObjId makeId(int objId = R_BAD_ID, int type = typeUnknown)
		{
			R_ObjId ret;
			ret.objId = objId;
			ret.type = type;
			return ret;
		}

		R_ObjId clone()
		{
			R_ObjId ret;
			ret.objId = this->objId;
			ret.type = this->type;
			return ret;
		}

		bool equals(R_ObjId id) const
		{
			return (this->objId == id.objId);
		}

		int objId;
		int type;
};

#define MAX_DATA_SIZE (16384 - sizeof(unsigned long) - sizeof(R_ObjId)*2 - sizeof(permissions))

#endif

