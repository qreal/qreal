#ifndef __DEFS_H__
#define __DEFS_H__

const int STATUS_ERROR	= -1;
const int STATUS_OK	= 1;

const int CMD_CREATE_ENTITY		= 1;
const int CMD_SET_NAME			= 2;
const int CMD_GET_NAME			= 3;
const int CMD_SET_POSITION		= 4;
const int CMD_GET_POSITION		= 5;
const int CMD_SET_CONFIGURATION		= 6;
const int CMD_GET_CONFIGURATION		= 7;
const int CMD_SET_PROPERTY		= 8;
const int CMD_GET_PROPERTY		= 9;

const int CMD_GET_ENTITY		= 10;
const int CMD_GET_OBJECTS_BY_TYPE	= 11;
const int CMD_GET_OBJECT_DATA		= 12;
const int CMD_GET_ENTIRE_OBJECT		= 13;
const int CMD_GET_CHILDREN		= 14;
const int CMD_GET_LINKS_BY_OBJECT	= 15;
const int CMD_GET_OBJECTS_BY_LINK	= 16;



const int CMD_GET_TYPES_COUNT	= 21;
const int CMD_GET_TYPE_INFO	= 22;

const int TYPE_OBJECT	= 1;
const int TYPE_LINK	= 2;

const int INVALID_ID 	= -1;

#endif // __DEFS_H__
