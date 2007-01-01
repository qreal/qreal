#include "R_Entity.h"
#include <sstream>

using namespace Storage;
using namespace std;

bool R_Entity::equals(R_ObjId id1, R_ObjId id2)
{
	return id1.equals(id2);
}

int R_Entity::toInt(std::string str)
{
	if (str.empty())
		return 0;

	stringstream ss;
	ss << str;
	int ret;
	ss >> ret;
	return ret;
}

unsigned long R_Entity::toULong(std::string str)
{
	if (str.empty())
		return 0L;

	stringstream ss;
	ss << str;
	unsigned long ret;
	ss >> ret;
	return ret;
}

std::string R_Entity::toStr(int intgr)
{
	stringstream ss;
	ss << intgr;
	string ret;
	ss >> ret;
	return ret;
}

std::string R_Entity::toStr(unsigned long ulong)
{
	stringstream ss;
	ss << ulong;
	string ret;
	ss >> ret;
	return ret;
}

R_Entity::R_Entity(R_ObjId objId, R_ObjId ownerId, Interfaces::Repository *stor, unsigned long userId)
{
	storage = stor;

	time = 0;

	id = objId;

	owner = ownerId;

	perms.userId = userId;
	perms.groupP = PERMS_READ;
	perms.othersP = PERMS_NONE;

	virgin = true;

	childsCount = 0;
	linksCount = 0;
}

R_Entity::~R_Entity()
{
	// todo: add code for syncing with repository
}

Interfaces::Repository* R_Entity::getStorage()
{
	return storage;
}

const R_ObjId R_Entity::getId()
{
	return id;
}

const R_ObjId R_Entity::getOwner()
{
	return owner;
}

void R_Entity::setOwner(R_ObjId id)
{
	owner = id;
}

const permissions R_Entity::getPerms()
{
	return perms;
}

void R_Entity::setPerms(permissions perms)
{
	this->perms = perms;
}

unsigned long R_Entity::getTime()
{
	return time;
}

void R_Entity::setTime(unsigned long time)
{
	this->time = time;
}

bool R_Entity::isVirgin()
{
	return virgin;
}

void R_Entity::modify()
{
	virgin = false;
}

bool R_Entity::correctType(vector<int> types, int type)
{
	for (int i = 0; i < types.size(); i++) {
		if (types[i] == type)
			return true;
	}

	return false;
}

int R_Entity::getChildsCount()
{
	return childsCount;
}

bool R_Entity::addChild(R_Entity *child)
{
	if (!child)
		return false;

	if (child->getId().objId == R_BAD_ID)
		return false;

	int pos;
	for (pos = 0; pos < childsCount; pos++) {
		if (equals(childs[pos], child->getId()))
			return false;
	}

	childsCount++;
	childs.push_back(child->getId());
	child->setOwner(getId());
	return true;
}

R_ObjId R_Entity::nextChild(R_ObjId child, vector<int> typeFilter)
{
	R_ObjId ret = R_ObjId::makeId();
	int pos;

	if (childsCount == 0) {
		return ret;
	} else if (typeFilter.size() == 0) {
		if (child.objId == R_BAD_ID) {
			ret = childs[0];
		} else {
			for (pos = 0; pos < (childsCount-1); pos++) {
				if (childs[pos].objId == child.objId) {
					ret = childs[pos+1];
					break;
				}
			}
		}
	} else {
		if (child.objId == R_BAD_ID) {
			for (pos = 0; pos < childsCount; pos++) {
				if (correctType(typeFilter, childs[pos].type)) {
					ret = childs[pos];
					break;
				}
			}
		} else {
			for (pos = 0; pos < childsCount; pos++) {
				if (childs[pos].objId == child.objId)
					break;
			}
			pos++;
			if (pos < childsCount) {
				for (; pos < childsCount; pos++) {
					if (correctType(typeFilter, childs[pos].type)) {
						ret = childs[pos];
						break;
					}
				}
			}
		}
	}
	return ret;
}

int R_Entity::getLinksCount()
{
	return linksCount;
}

bool R_Entity::addLink(R_Entity *link)
{
	if (!link)
		return false;

	if (link->getId().objId == R_BAD_ID)
		return false;

	int pos;
	for (pos = 0; pos < linksCount; pos++) {
		if (equals(links[pos], link->getId()))
			return false;
	}

	linksCount++;
	links.push_back(link->getId());
	return true;
}

R_ObjId R_Entity::nextLink(R_ObjId link, vector<int> typeFilter)
{
	R_ObjId ret = R_ObjId::makeId();
	int pos;

	if (linksCount == 0) {
		return ret;
	} else if (typeFilter.size() == 0) {
		if (link.objId == R_BAD_ID) {
			ret = links[0];
		} else {
			for (pos = 0; pos < (linksCount-1); pos++) {
				if (links[pos].objId == link.objId) {
					ret = links[pos+1];
					break;
				}
			}
		}
	} else {
		if (link.objId == R_BAD_ID) {
			for (pos = 0; pos < linksCount; pos++) {
				if (correctType(typeFilter, links[pos].type)) {
					ret = links[pos];
					break;
				}
			}
		} else {
			for (pos = 0; pos < linksCount; pos++) {
				if (links[pos].objId == link.objId)
					break;
			}
			pos++;
			if (pos < linksCount) {
				for (; pos < linksCount; pos++) {
					if (correctType(typeFilter, links[pos].type)) {
						ret = links[pos];
						break;
					}
				}
			}
		}
	}
	return ret;
}

std::string R_Entity::toString()
{
	string ret, first, second;
	unsigned int i;
	map<string, string>::iterator p = data.begin();
	while (p != data.end()) {
		pair<string, string> pr = *p;
		first = "";
		second = "";
		for (i = 0; i < pr.first.length(); i++) {
			if ((pr.first[i] != ';') && (pr.first[i] != '=')) {
				first += pr.first[i];
			} else if (pr.first[i] == ';') {
				first += ";;";
			} else if (pr.first[i] == '=') {
				first += "==";
			}
		}

		for (i = 0; i < pr.second.length(); i++) {
			if ((pr.second[i] != ';') && (pr.second[i] != '=')) {
				second += pr.second[i];
			} else if (pr.second[i] == ';') {
				second += ";;";
			} else if (pr.second[i] == '=') {
				second += "==";
			}
		}

		ret += first + "=" + second + ";";
		p++;
	}

	return ret;
}

void R_Entity::fromString(std::string contents)
{
	data.clear();

	string name, value;
	int i, stage = 0;
	int len = contents.length() - 1;
	for (i = 0; i < len; i++) {
		if ((contents[i] == ';') && (contents[i+1] != ';')) {
			data[name] = value;
			name = "";
			value = "";
			stage = 0;
		} else if ((contents[i] == '=') && (contents[i+1] != '=')) {
			stage ++;
		} else if ((contents[i] == '=') && (contents[i+1] == '=')) {
			if (stage == 0)
				name += contents[i];
			else
				value += contents[i];
			i++;
		} else if ((contents[i] == ';') && (contents[i+1] == ';')) {
			if (stage == 0)
				name += contents[i];
			else
				value += contents[i];
			i++;
		} else {
			if (stage == 0)
				name += contents[i];
			else
				value += contents[i];
		}
	}

	if (contents[len] == ';') {
		data[name] = value;
	}
}

int R_Entity::getSize()
{
	return 0;
}

int R_Entity::toMemory(char *buf)
{
	if (!buf)
		return 0;

	// store link & child information
	int offset = 0;

	/*
	   int size
	   R_ObjId id
	   R_ObjId ownerId
	   permissions perms
	   bool virgin
	   unsigned long time
	   int childsCount
	   int linksCount
	   R_ObjId childs[..]
	   R_ObjId links[..]

	 */
	int size = 2*sizeof(int) + sizeof(R_ObjId)*2 + sizeof(perms) + sizeof(virgin) + sizeof(time) + (getChildsCount() + getLinksCount())*sizeof(R_ObjId);
	memcpy(buf + offset, &size, sizeof(size));
	offset += sizeof(size);

	memcpy(buf + offset, &id, sizeof(id));
	offset += sizeof(id);

	memcpy(buf + offset, &owner, sizeof(owner));
	offset += sizeof(owner);

	memcpy(buf + offset, &perms, sizeof(perms));
	offset += sizeof(perms);

	memcpy(buf + offset, &virgin, sizeof(virgin));
	offset += sizeof(virgin);

	memcpy(buf + offset, &time, sizeof(time));
	offset += sizeof(time);

	size = getChildsCount();
	memcpy(buf + offset, &size, sizeof(size));
	offset += sizeof(size);
	size = getLinksCount();
	memcpy(buf + offset, &size, sizeof(size));
	offset += sizeof(size);

	vector<int> types;
	R_ObjId tmpId = R_ObjId::makeId();

	// store childs
	tmpId = nextChild(tmpId, types);
	while (!equals(tmpId, R_ObjId::makeId())) {
		memcpy(buf + offset, &tmpId, sizeof(tmpId));
		offset += sizeof(tmpId);
		tmpId = nextChild(tmpId, types);
	}

	// store links	
	tmpId = nextLink(tmpId, types);
	while (!equals(tmpId, R_ObjId::makeId())) {
		memcpy(buf + offset, &tmpId, sizeof(tmpId));
		offset += sizeof(tmpId);
		tmpId = nextLink(tmpId, types);
	}

	// store additional string data
	string str = toString();
	memcpy(buf + offset, str.c_str(), min(MAX_DATA_SIZE, str.length()));
	return offset + str.size();
}

bool R_Entity::fromMemory(char *buf, int size)
{
	if (!buf || !size)
		return false;

	childs.clear();
	links.clear();

	int offset = 0;
	int tmpInt;

	// load string entity data
	memcpy(&tmpInt, buf, sizeof(tmpInt));
	tmpInt += sizeof(tmpInt);
	char *data = new char[MAX_DATA_SIZE];
	memset(data, 0, MAX_DATA_SIZE);
	memcpy(data, buf + tmpInt, size - tmpInt);
	string str = data;
	delete data;
	fromString(str);

	offset = sizeof(tmpInt);

	// load additional information
	memcpy(&id, buf + offset, sizeof(id));
	offset += sizeof(id);

	memcpy(&owner, buf + offset, sizeof(owner));
	offset += sizeof(owner);

	memcpy(&perms, buf + offset, sizeof(perms));
	offset += sizeof(perms);

	memcpy(&virgin, buf + offset, sizeof(virgin));
	offset += sizeof(virgin);

	memcpy(&time, buf + offset,sizeof(time));
	offset += sizeof(time);

	// load childs & links information
	int cCount;
	int lCount;
	memcpy(&cCount, buf + offset, sizeof(cCount));
	offset += sizeof(cCount);
	memcpy(&lCount, buf + offset, sizeof(lCount));
	offset += sizeof(lCount);

	// load childs
	R_ObjId tmpId;
	childsCount = 0;
	while (cCount) {
		memcpy(&tmpId, buf + offset, sizeof(tmpId));
		childs.push_back(tmpId);
		childsCount++;
		offset += sizeof(tmpId);
		cCount--;
	}

	// load links
	lCount = 0;
	while (lCount) {
		memcpy(&tmpId, buf + offset, sizeof(tmpId));
		links.push_back(tmpId);
		linksCount++;
		offset += sizeof(tmpId);
		lCount--;
	}

	return true;
}

void R_Entity::setType(int type)
{
	id.type = type;
}

void R_Entity::deleteIt()
{
	deleted = true;
}

bool R_Entity::isDeleted()
{
	return deleted;
}

void R_Entity::sync(bool fromStorage)
{
	if (fromStorage) {
		// reload object from storage
	} else {
		// write all data to storage
	}
}

void R_Entity::setValue(std::string name, std::string value)
{
	data[name] = value;
}

std::string R_Entity::getValue(std::string name)
{
	return data[name];
}

void R_Entity::writeId(std::string name, R_ObjId id)
{
	data[name + ".id"] = toStr(id.objId);
	data[name + ".type"] = toStr(id.type);
}

R_ObjId R_Entity::readId(std::string name)
{
	R_ObjId ret = R_ObjId::makeId();
	if (data[name + ".id"] != "") {
		ret.objId = toInt(data[name + ".id"]);
	}

	if (data[name + ".type"] != "") {
		ret.type = toInt(data[name + ".type"]);
	}

	return ret;
}
