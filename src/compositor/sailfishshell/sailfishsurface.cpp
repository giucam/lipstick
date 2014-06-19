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

#include <QDebug>

#include "sailfishsurface.h"
#include "sailfishshell.h"
#include "lipstickcompositor.h"
#include "lipstickcompositorwindow.h"

SailfishSurface::SailfishSurface(SailfishShell *shell, QWaylandSurface *surface, uint32_t id)
               : QWaylandSurfaceInterface(surface)
               , QtWaylandServer::sailfish_surface(static_cast<wl_client*>(surface->client()), id)
               , m_shell(shell)
               , m_window(Q_NULLPTR)
               , m_hidden(false)
               , m_lastSize(0, 0)
               , m_serial(0)
               , m_deleting(false)
{
    connect(surface, &QWaylandSurface::configure, this, &SailfishSurface::configure);
    setSurfaceType(QWaylandSurface::Toplevel);
}

SailfishSurface::~SailfishSurface()
{
    if (m_window && m_window->cover() && surface()) {
        m_window->setCover(Q_NULLPTR);
    }

    // We must destroy the wl_resource here, but be careful not to do it
    // if we're here from shell_surface_destroy_resource(), i.e. if the
    // wl_resource was destroyed already
    if (!m_deleting) {
        m_deleting = true;
        wl_resource_destroy(resource()->handle);
    }
}

SailfishWindow *SailfishSurface::window() const
{
    return m_window;
}

bool SailfishSurface::runOperation(QWaylandSurfaceOp *op)
{
    switch (op->type()) {
        case QWaylandSurfaceOp::Close:
            if (m_window) {
                m_window->send_close();
                return true;
            }
            break;
        case QWaylandSurfaceOp::SetVisibility: {
            QWindow::Visibility v = static_cast<QWaylandSurfaceSetVisibilityOp *>(op)->visibility();
            m_hidden = v == QWindow::Hidden;
            updateStates();
            return true;
        }
        case QWaylandSurfaceOp::Ping:
            m_shell->ping(static_cast<QWaylandSurfacePingOp *>(op)->serial(), surface());
            return true;
        case QWaylandSurfaceOp::Resize: {
            QSize s = static_cast<QWaylandSurfaceResizeOp *>(op)->size();
            sendConfigure(s.width(), s.height());
            return true;
        }
        default:
            break;
    }
    return false;
}

void SailfishSurface::sailfish_surface_destroy_resource(Resource *)
{
    if (!m_deleting) {
        m_deleting = true;
        delete this;
    }
}

void SailfishSurface::sailfish_surface_ack_configure(Resource *resource, uint32_t serial)
{
    Q_UNUSED(resource);
    if (serial == m_serial) {
        m_serial = 0;
    }
}

void SailfishSurface::sailfish_surface_request_state(Resource *resource, wl_array *array)
{
    Q_UNUSED(resource);
    bool wasHidden = m_hidden;
    m_hidden = false;

    for (uint32_t *ptr = (uint32_t *)array->data; (char *)ptr < (char *)array->data + array->size; ++ptr) {
        if (*ptr == SAILFISH_SURFACE_STATE_HIDDEN) {
            m_hidden = true;
        }
    }
    if (wasHidden && !m_hidden) {
        emit surface()->raiseRequested();
    } else {
        updateStates();
    }
}

SailfishSurface *SailfishSurface::fromResource(::wl_resource *resource)
{
    return static_cast<SailfishSurface *>(Resource::fromResource(resource)->sailfish_surface_object);
}

void SailfishSurface::configure(bool hasBuffer)
{
    if (hasBuffer && m_serial == 0) {
        m_lastSize = surface()->size();
    }
    surface()->setMapped(hasBuffer);
}

void SailfishSurface::sendConfigure(int w, int h)
{
    QVector<uint32_t> states;
    if (m_hidden) {
        states << SAILFISH_SURFACE_STATE_HIDDEN;
    }
    QByteArray data = QByteArray::fromRawData((char *)states.data(), states.size() * sizeof(uint32_t));
    m_serial = wl_display_next_serial(surface()->compositor()->waylandDisplay());

    send_configure(w, h, data, m_serial);
    m_lastSize = QSize(w, h);
}

void SailfishSurface::updateStates()
{
    sendConfigure(m_lastSize.width(), m_lastSize.height());
}


SailfishWindow::SailfishWindow(SailfishSurface *surface, uint32_t id)
              : QtWaylandServer::sailfish_window(static_cast<wl_client*>(surface->surface()->client()), id)
              , m_surface(surface)
              , m_cover(Q_NULLPTR)
              , m_deleting(false)
{
    LipstickCompositor *compositor = LipstickCompositor::instance();
    surface->sendConfigure(compositor->width(), compositor->height());
    surface->m_window = this;
}

SailfishWindow::~SailfishWindow()
{
    if (!m_deleting) {
        m_deleting = true;
        wl_resource_destroy(resource()->handle);
    }
}

QWaylandSurface *SailfishWindow::surface() const
{
    return m_surface->surface();
}

QWaylandSurface *SailfishWindow::cover() const
{
    return m_cover->surface();
}

void SailfishWindow::setCover(SailfishSurface *surf)
{
    if (m_cover == surf)
        return;

    m_cover = surf;
    emit coverChanged();
}

void SailfishWindow::sailfish_window_destroy_resource(Resource *)
{
    if (!m_deleting) {
        m_deleting = true;
        delete this;
    }
}

void SailfishWindow::sailfish_window_set_title(Resource *resource, const QString &title)
{
    Q_UNUSED(resource)
    Q_UNUSED(title)
    m_surface->setSurfaceTitle(title);
}

void SailfishWindow::sailfish_window_set_app_id(Resource *resource, const QString &app_id)
{
    Q_UNUSED(resource)
    Q_UNUSED(app_id)
    m_surface->setSurfaceClassName(app_id);
}

void SailfishWindow::sailfish_window_set_transform_mask(Resource *resource, uint32_t transform)
{
    Q_UNUSED(resource)
    Q_UNUSED(transform)
}

void SailfishWindow::sailfish_window_set_cover_surface(Resource *resource, ::wl_resource *cover)
{
    Q_UNUSED(resource)

    SailfishSurface *surface = cover ? SailfishSurface::fromResource(cover) : Q_NULLPTR;
    if (m_cover == surface)
        return;

    setCover(surface);
    if (!surface || surface == m_surface)
        return;

    surface->m_window = this;
    surface->sendConfigure(0, 0); //can we use some more meaningful size here?
}
