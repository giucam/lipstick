/***************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Giulio Camuffo <giulio.camuffo@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef LIPSTICKSURFACEINTERFACE
#define LIPSTICKSURFACEINTERFACE

#include <QtCompositor/QWaylandSurfaceInterface>

#include "lipstickglobal.h"

class LIPSTICK_EXPORT LipstickSurfaceInterface : public QObject, public QWaylandSurfaceInterface
{
    Q_OBJECT
public:
    LipstickSurfaceInterface(QWaylandSurface *surface);

    QWaylandSurface *cover() const;
    void setCover(QWaylandSurface *surf);

signals:
    void coverChanged();

private:
    QWaylandSurface *m_cover;
};

class LIPSTICK_EXPORT LipstickCoverSurfaceInterface : public QWaylandSurfaceInterface
{
public:
    LipstickCoverSurfaceInterface(QWaylandSurface *surface);

};

#endif