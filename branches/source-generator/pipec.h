#ifndef PIPEC_H
#define PIPEC_H

#include <QtGui/QWidget>
//#include "ui_pipec.h"
#include <QPaintDevice>
#include "newengine.h"

class pipec : public QPaintDevice
{
public:
	 pipec(QString path,QPoint size, QPaintDevice *parent = 0);
    ~pipec();

    QPaintEngine * paintEngine () const;

private:
     NewEngine *device;
     QPoint picture_size;
 //    int metric(QPaintDevice::PaintDeviceMetric metric) const;
};

#endif // PIPEC_H
