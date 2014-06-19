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

#ifndef SAILFISHSHELL_H
#define SAILFISHSHELL_H

#include <QtCompositor/QWaylandGlobalInterface>

#include "qwayland-server-sailfish-shell.h"

class QWaylandSurface;

class SailfishShellGlobal : public QWaylandGlobalInterface
{
public:
    SailfishShellGlobal();

    const wl_interface *interface() const Q_DECL_OVERRIDE;
    void bind(wl_client *client, uint32_t version, uint32_t id) Q_DECL_OVERRIDE;
};

class SailfishShell : public QtWaylandServer::sailfish_shell
{
public:
    SailfishShell(wl_client *client, uint32_t id);

    void ping(uint32_t serial, QWaylandSurface *surface);

protected:
    void sailfish_shell_get_sailfish_surface(Resource *resource, uint32_t id, ::wl_resource *surface) Q_DECL_OVERRIDE;
    void sailfish_shell_get_sailfish_window(Resource *resource, uint32_t id, ::wl_resource *surface) Q_DECL_OVERRIDE;
    void sailfish_shell_pong(Resource *resource, uint32_t serial) Q_DECL_OVERRIDE;

private:
    QMap<uint32_t, QWaylandSurface *> m_pings;
};

#endif
