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

#ifndef SAILFISHSURFACE_H
#define SAILFISHSURFACE_H

#include <QtCompositor/QWaylandSurface>

#include "lipsticksurfaceinterface.h"
#include "qwayland-server-sailfish-shell.h"

class SailfishShell;
class SailfishWindow;

class SailfishSurface : public QObject, public QWaylandSurfaceInterface, public QtWaylandServer::sailfish_surface
{
public:
    SailfishSurface(SailfishShell *shell, QWaylandSurface *surface, uint32_t id);
    ~SailfishSurface();

    SailfishWindow *window() const;
    void sendConfigure(int width, int height);
    static SailfishSurface *fromResource(::wl_resource *resource);

protected:
    bool runOperation(QWaylandSurfaceOp *op) Q_DECL_OVERRIDE;

    void sailfish_surface_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void sailfish_surface_ack_configure(Resource *resource, uint32_t serial) Q_DECL_OVERRIDE;
    void sailfish_surface_request_state(Resource *resource, wl_array *states) Q_DECL_OVERRIDE;

private:
    void configure(bool hasBuffer);
    void updateStates();

    SailfishShell *m_shell;
    SailfishWindow *m_window;
    bool m_hidden;
    QSize m_lastSize;
    uint32_t m_serial;
    bool m_deleting;

    friend class SailfishWindow;
};

class SailfishWindow : public LipstickSurfaceInterface, public QtWaylandServer::sailfish_window
{
public:
    SailfishWindow(SailfishSurface *surface, uint32_t id);
    ~SailfishWindow();

    QWaylandSurface *surface() const;

protected:
    bool runOperation(QWaylandSurfaceOp *op) Q_DECL_OVERRIDE;

    void sailfish_window_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void sailfish_window_set_title(Resource *resource, const QString &title) Q_DECL_OVERRIDE;
    void sailfish_window_set_app_id(Resource *resource, const QString &app_id) Q_DECL_OVERRIDE;
    void sailfish_window_set_transform_mask(Resource *resource, uint32_t transform) Q_DECL_OVERRIDE;
    void sailfish_window_set_cover_surface(Resource *resource, ::wl_resource *cover) Q_DECL_OVERRIDE;

private:
    SailfishSurface *m_surface;
    bool m_deleting;
};

#endif
