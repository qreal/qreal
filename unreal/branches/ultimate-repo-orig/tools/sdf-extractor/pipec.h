#pragma once

#include <QtGui/QWidget>
#include <QPaintDevice>
#include "newengine.h"

namespace sdfExtractor
{

	class pipec : public QPaintDevice
	{
	public:
		pipec(QString path, QPoint size, QPaintDevice *parent = 0);
		~pipec();

		QPaintEngine * paintEngine () const;
	private:
		NewEngine *device;
		QPoint picture_size;
	};

}
