#include "pipec.h"
#include "newengine.h"

using namespace sdfExtractor;

pipec::pipec(QString path, QPoint size, QPaintDevice *parent)
{
	device = new NewEngine(path,size);
}

QPaintEngine * pipec::paintEngine () const
{
	return device;
}

pipec::~pipec()
{
	delete device;
}
