// Copyright (c) 2010, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

// Customized with QReal-only needs

#ifndef QSKINETICSCROLLER_H
#define QSKINETICSCROLLER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtWidgets/QGesture>
#include <QtGui/QMouseEvent>

#include "thirdPartyDeclSpec.h"

class QsKineticScrollerImpl;
class QAbstractScrollArea;
class QEvent;

//! Vertical kinetic scroller implementation without overshoot and bouncing.
//! A temporary solution to get kinetic-like scrolling on Symbian.
class QRGUI_THIRDPARTY_EXPORT QsKineticScroller: public QObject
{
   Q_OBJECT

public:
   explicit QsKineticScroller(QObject* parent = 0);
   ~QsKineticScroller();
   //! enabled for one widget only, new calls remove previous association
   void enableKineticScrollFor(QAbstractScrollArea* scrollArea);

   bool shouldIgnoreMouseAction();

   void onTap();
   void onPan(QGesture *pan);
   void onMousePress(QMouseEvent *mouseEvent);
   void onMouseMove(QMouseEvent *mouseEvent);
   void onMouseRelease(QObject *eventSource, QMouseEvent *mouseEvent);

private slots:
   void onKineticTimerElapsed();

private:
   void computeFriction();

   QScopedPointer<QsKineticScrollerImpl> d;
};

#endif // QSKINETICSCROLLER_H
