#include "pipec.h"
#include "newengine.h"

//NewEngine* device = new NewEngine();

pipec::pipec(QString path, QPoint size, QPaintDevice *parent)
{
	device = new NewEngine(path,size);

}
QPaintEngine * pipec::paintEngine () const
{
	return device;
}

/*int pipec::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmDepth:
        return 32;
    case QPaintDevice::PdmWidth:
        return 1488;
    case QPaintDevice::PdmHeight:
        return 1488;
    case QPaintDevice::PdmDpiX:
        return 1200;
    case QPaintDevice::PdmDpiY:
        return 1200;
    case QPaintDevice::PdmHeightMM:
        return qRound(1488 * 25.4 / 1200);
    case QPaintDevice::PdmWidthMM:
        return qRound(1488 * 25.4 / 1200);
    case QPaintDevice::PdmNumColors:
        return 0xffffffff;
    case QPaintDevice::PdmPhysicalDpiX:
        return 1200;
    case QPaintDevice::PdmPhysicalDpiY:
        return 1200;
    default:
        qWarning("QSvgGenerator::metric(), unhandled metric %d\n", metric);
        break;
    }
    return 0;
}*/

pipec::~pipec()
{
	delete device;
}



