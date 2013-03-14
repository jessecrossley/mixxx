/***************************************************************************
                         controlobjectthread.cpp  -  description
                            -------------------
   begin                : Thu Sep 23 2004
   copyright            : (C) 2004 by Tue Haste Andersen
   email                : haste@diku.dk
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QApplication>
#include <QtDebug>

#include "controlobjectthread.h"
#include "controlobject.h"

ControlObjectThread::ControlObjectThread(ControlObject* pControlObject, QObject* pParent)
        : QObject(pParent),
          m_dValue(0.0),
          m_pControlObject(pControlObject) {
    // Register with the associated ControlObject
    if (m_pControlObject != NULL) {
        m_pControlObject->addProxy(this);
        connect(m_pControlObject, SIGNAL(destroyed()),
                this, SLOT(slotParentDead()));
        connect(m_pControlObject, SIGNAL(valueChanged(double v)),
                this, SIGNAL(valueChanged(double v)));
        // Initialize value
        m_dValue = m_pControlObject->get();
    }
    emitValueChanged();
}

ControlObjectThread::~ControlObjectThread() {
    if (m_pControlObject) {
        // Our parent is still around, make sure it doesn't send us any more events
        m_pControlObject->removeProxy(this);
    }
}

double ControlObjectThread::get() {
    if (m_pControlObject) {
        return m_pControlObject->get();
    } else {
        return 0.0;
    }

}

void ControlObjectThread::slotSet(double v) {
    m_pControlObject->set(v);
}

bool ControlObjectThread::setExtern(double v) {
    return true;
}

void ControlObjectThread::emitValueChanged() {
    emit(valueChanged(get()));
}

void ControlObjectThread::add(double v) {
    m_pControlObject->add(v);
}

void ControlObjectThread::sub(double v) {
    m_pControlObject->sub(v);
}

void ControlObjectThread::updateControlObject(double v) {
    if (m_pControlObject) {
        m_pControlObject->queueFromThread(v, this);
    }
}

void ControlObjectThread::slotParentDead() {
    // Now we've got a chance of avoiding segfaults with judicious
    // use of if(m_pControlObject)
    m_pControlObject = NULL;
}

ControlObject* ControlObjectThread::getControlObject() {
   return m_pControlObject;
}
