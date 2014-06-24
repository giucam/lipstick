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

#include "lipsticksurfaceinterface.h"

LipstickSurfaceInterface::LipstickSurfaceInterface(QWaylandSurface *surface)
                        : QWaylandSurfaceInterface(surface)
                        , m_cover(Q_NULLPTR)
{
}

QWaylandSurface *LipstickSurfaceInterface::cover() const
{
    return m_cover;
}

void LipstickSurfaceInterface::setCover(QWaylandSurface *surf)
{
    if (m_cover == surf)
        return;

    m_cover = surf;
    emit coverChanged();
}



LipstickCoverSurfaceInterface::LipstickCoverSurfaceInterface(QWaylandSurface *surface)
                             : QWaylandSurfaceInterface(surface)
{
}
