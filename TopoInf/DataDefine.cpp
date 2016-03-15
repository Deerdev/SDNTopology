#include "SDNDataDefine.h"

SockData::SockData()
{
	msgType = 1;
}

SockData::SockData(const SockData &_data)
{
	if ( &_data == this)
	{
		return;
	}
	msgType = _data.msgType;
}

SockData::~SockData()
{

}
