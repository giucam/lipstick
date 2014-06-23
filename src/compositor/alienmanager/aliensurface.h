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

#ifndef ALIENSURFACE_H
#define ALIENSURFACE_H

#include <QtCompositor/QWaylandSurfaceInterface>

#include "qwayland-server-alien-manager.h"

class AlienClient;

class AlienSurface : public QObject, public QWaylandSurfaceInterface, public QtWaylandServer::alien_surface
{
    Q_OBJECT
public:
    AlienSurface(AlienClient *client, QWaylandSurface *surface, uint32_t id);
    ~AlienSurface();

    static AlienSurface *fromResource(::wl_resource *resource);

protected:
    bool runOperation(QWaylandSurfaceOp *op) Q_DECL_OVERRIDE;

    void alien_surface_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void alien_surface_set_title(Resource *resource, const QString &title) Q_DECL_OVERRIDE;
    void alien_surface_set_transform_mask(Resource *resource, uint32_t transform) Q_DECL_OVERRIDE;
    void alien_surface_ack_configure(Resource *resource, uint32_t serial) Q_DECL_OVERRIDE;
    void alien_surface_request_state(Resource *resource, wl_array *states) Q_DECL_OVERRIDE;

private:
    void configure(bool hasBuffer);
    void sendConfigure(int width, int height);
    void updateStates();

    AlienClient *m_client;
    bool m_hidden;
    bool m_coverized;
    QSize m_lastSize;
    uint32_t m_serial;
    bool m_deleting;
};

#endif
