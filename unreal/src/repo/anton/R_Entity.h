//#pragma once

//#pragma warning(disable: 4251)
//#pragma warning(disable: 4275)

//#include "../../common/win_export.h"
#include "IStorage.h"
#include "proto_defs.h"
#include "entity_types.h"
#include "Storage_misc.h"

#include <string>
#include <map>
#include <vector>

namespace Storage
{
	class R_Entity
	{
	public:
		static bool equals(R_ObjId id1, R_ObjId id2);
		static int toInt(std::string str);
		static unsigned long toULong(std::string str);
		static std::string toStr(int intgr);
		static std::string toStr(unsigned long ulong);


	protected:
		R_ObjId id;
		R_ObjId owner;
		Interfaces::Repository *storage;
		bool deleted;

		bool virgin;

		permissions perms;
		unsigned long time;

		int childsCount;
		int linksCount;
		std::vector<R_ObjId> childs;
		std::vector<R_ObjId> links;

		std::map<std::string, std::string> data;

		std::string toString();
		void fromString(std::string contents);

	public:
		R_Entity(R_ObjId objId, R_ObjId ownerId, Interfaces::Repository *stor, unsigned long userId = 0);
		~R_Entity();

		const R_ObjId getId();

		const R_ObjId getOwner();
		void setOwner(R_ObjId id);

		const permissions getPerms();
		void setPerms(permissions perms);

		unsigned long getTime();
		void setTime(unsigned long time);

		bool isVirgin();
		void modify();

		bool correctType(std::vector<int> types, int type);

		int getChildsCount();
		bool addChild(R_Entity *child);
		R_ObjId nextChild(R_ObjId child, std::vector<int> typeFilter);

		int getLinksCount();
		bool addLink(R_Entity *link);
		R_ObjId nextLink(R_ObjId link, std::vector<int> typeFilter);

		int getSize();
		int toMemory(char *buf);
		bool fromMemory(char *buf, int size);
        
		Interfaces::Repository *getStorage();

		void setType(int type);

		void deleteIt();
		bool isDeleted();

		void sync(bool fromStorage = false);

		void setValue(std::string name, std::string value);
		std::string getValue(std::string name);

		void writeId(std::string name, R_ObjId id);
		R_ObjId readId(std::string name);
	};
}
