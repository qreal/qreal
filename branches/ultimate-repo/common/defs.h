#ifndef __DEFS_H__
#define __DEFS_H__

const int STATUS_ERROR	= -1;
const int STATUS_OK	= 1;

const int CMD_CREATE_ENTITY		= 1;
const int CMD_DELETE_ENTITY		= 2;
const int CMD_SET_NAME			= 3;
const int CMD_GET_NAME			= 4;
const int CMD_SET_POSITION		= 5;
const int CMD_GET_POSITION		= 6;
const int CMD_SET_CONFIGURATION	= 7;
const int CMD_GET_CONFIGURATION	= 8;
const int CMD_SET_PROPERTY		= 9;
const int CMD_GET_PROPERTY		= 10;
const int CMD_SET_DESCRIPTION	= 11;
const int CMD_GET_DESCRIPTION	= 12;

const int CMD_GET_ENTITY		= 20;
const int CMD_GET_OBJECTS_BY_TYPE	= 21;
const int CMD_GET_OBJECT_DATA		= 22;
const int CMD_GET_ENTIRE_OBJECT		= 23;
const int CMD_GET_CHILDREN		= 24;
const int CMD_GET_LINKS_BY_OBJECT	= 25;
const int CMD_GET_OBJECTS_BY_LINK	= 26;

const int CMD_GET_TYPES_COUNT		= 31;
const int CMD_GET_ALL_TYPES		= 32;
const int CMD_GET_TYPES_BY_METATYPE	= 33;
const int CMD_GET_TYPE_INFO		= 34;
const int CMD_GET_TYPE_BY_NAME		= 35;

const int CMD_ADD_LINK			= 51;
const int CMD_REMOVE_LINK		= 52;

const int TYPE_OBJECT	= 1;
const int TYPE_LINK	= 2;

const int INVALID_ID 	= -1;

const int INCOMING_LINK		= 1;
const int OUTCOMING_LINK	= 2;


#endif // __DEFS_H__
