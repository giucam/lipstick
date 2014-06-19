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

#include "sailfishshell.h"
#include "sailfishsurface.h"

SailfishShellGlobal::SailfishShellGlobal()
{

}

const wl_interface *SailfishShellGlobal::interface() const
{
    return &sailfish_shell_interface;
}

void SailfishShellGlobal::bind(wl_client *client, uint32_t version, uint32_t id)
{
    Q_UNUSED(version)
    new SailfishShell(client, id);
}



SailfishShell::SailfishShell(wl_client *client, uint32_t id)
             : QtWaylandServer::sailfish_shell(client, id)
{
}

void SailfishShell::ping(uint32_t serial, QWaylandSurface *surface)
{
    m_pings.insert(serial, surface);
    send_ping(serial);
}

void SailfishShell::sailfish_shell_get_sailfish_surface(Resource *resource, uint32_t id, ::wl_resource *surface)
{
    Q_UNUSED(resource)

    QWaylandSurface *surf = QWaylandSurface::fromResource(surface);
    new SailfishSurface(this, surf, id);
}

void SailfishShell::sailfish_shell_get_sailfish_window(Resource *resource, uint32_t id, ::wl_resource *surface)
{
    Q_UNUSED(resource)
    SailfishSurface *surf = SailfishSurface::fromResource(surface);
    new SailfishWindow(surf, id);
}

void SailfishShell::sailfish_shell_pong(Resource *resource, uint32_t serial)
{
    Q_UNUSED(resource)
    QWaylandSurface *surf = m_pings.value(serial);
    if (surf)
        surf->pong();
}
