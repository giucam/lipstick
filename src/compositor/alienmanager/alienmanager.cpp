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

#include <QtCompositor/QWaylandSurface>

#include "alienmanager.h"
#include "aliensurface.h"

AlienManagerGlobal::AlienManagerGlobal()
{

}

const wl_interface *AlienManagerGlobal::interface() const
{
    return &alien_manager_interface;
}

void AlienManagerGlobal::bind(wl_client *client, uint32_t version, uint32_t id)
{
    Q_UNUSED(version)
    new AlienManager(client, id);
}



AlienManager::AlienManager(wl_client *client, uint32_t id)
            : QtWaylandServer::alien_manager(client, id)
{
}

void AlienManager::ping(uint32_t serial, QWaylandSurface *surface)
{
    m_pings.insert(serial, surface);
    send_ping(serial);
}

void AlienManager::alien_manager_create_alien_client(Resource *resource, uint32_t id, const QString &package)
{
    new AlienClient(this, resource->client(), id, package);
}

void AlienManager::alien_manager_pong(Resource *resource, uint32_t serial)
{
    Q_UNUSED(resource)
    QWaylandSurface *surf = m_pings.value(serial);
    if (surf)
        surf->pong();
}


AlienClient::AlienClient(AlienManager *mgr, wl_client *client, uint32_t id, const QString &package)
           : QtWaylandServer::alien_client(client, id)
           , m_package(package)
           , m_manager(mgr)
{
}

AlienManager *AlienClient::manager() const
{
    return m_manager;
}

void AlienClient::alien_client_get_alien_surface(Resource *resource, uint32_t id, ::wl_resource *surface)
{
    Q_UNUSED(resource)

    QWaylandSurface *surf = QWaylandSurface::fromResource(surface);
    new AlienSurface(this, surf, id);
}
