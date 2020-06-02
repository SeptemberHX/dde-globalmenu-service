/*
  This file is part of the KDE project.

  Copyright (c) 2011 Lionel Chauvin <megabigbug@yahoo.fr>
  Copyright (c) 2011,2012 Cédric Bellegarde <gnumdk@gmail.com>
  Copyright (c) 2016 Kai Uwe Broulik <kde@privat.broulik.de>

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/

#include "menuimporter.h"
#include "menuimporteradaptor.h"
#include "dbusmenutypes_p.h"

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusServiceWatcher>
#include <QX11Info>

#include <KWindowSystem>
#include <KWindowInfo>

static const char* DBUS_SERVICE = "com.canonical.AppMenu.Registrar";
static const char* DBUS_OBJECT_PATH = "/com/canonical/AppMenu/Registrar";
static const QByteArray s_x11AppMenuServiceNamePropertyName = QByteArrayLiteral("_KDE_NET_WM_APPMENU_SERVICE_NAME");
static const QByteArray s_x11AppMenuObjectPathPropertyName = QByteArrayLiteral("_KDE_NET_WM_APPMENU_OBJECT_PATH");


MenuImporter::MenuImporter(QObject* parent)
: QObject(parent)
, m_serviceWatcher(new QDBusServiceWatcher(this))
{
    qDBusRegisterMetaType<DBusMenuLayoutItem>();
    m_serviceWatcher->setConnection(QDBusConnection::sessionBus());
    m_serviceWatcher->setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    connect(m_serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this, &MenuImporter::slotServiceUnregistered);
}

MenuImporter::~MenuImporter()
{
    QDBusConnection::sessionBus().unregisterService(DBUS_SERVICE);
}

bool MenuImporter::connectToBus()
{
    if (!QDBusConnection::sessionBus().registerService(DBUS_SERVICE)) {
        return false;
    }
    new MenuImporterAdaptor(this);
    QDBusConnection::sessionBus().registerObject(DBUS_OBJECT_PATH, this);

    return true;
}

void MenuImporter::RegisterWindow(WId id, const QDBusObjectPath& path)
{
    KWindowInfo info(id, NET::WMWindowType, NET::WM2WindowClass);
    NET::WindowTypes mask = NET::AllTypesMask;

    // Menu can try to register, right click in gimp for example
    if (info.windowType(mask) & (NET::Menu|NET::DropdownMenu|NET::PopupMenu)) {
        return;
    }

    if (path.path().isEmpty()) //prevent bad dbusmenu usage
        return;

    QString service = message().service();

    QString classClass = info.windowClassClass();
    m_windowClasses.insert(id, classClass);
    m_menuServices.insert(id, service);
    m_menuPaths.insert(id, path);

    if (!m_serviceWatcher->watchedServices().contains(service)) {
        m_serviceWatcher->addWatchedService(service);
    }

    auto *c = QX11Info::connection();

    static xcb_atom_t s_serviceNameAtom = XCB_ATOM_NONE;
    static xcb_atom_t s_objectPathAtom = XCB_ATOM_NONE;

    auto setWindowProperty = [c](WId id, xcb_atom_t &atom, const QByteArray &name, const QByteArray &value) {
        if (atom == XCB_ATOM_NONE) {
            const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(c, false, name.length(), name.constData());
            QScopedPointer<xcb_intern_atom_reply_t, QScopedPointerPodDeleter> reply(xcb_intern_atom_reply(c, cookie, nullptr));
            if (reply.isNull()) {
                return;
            }
            atom = reply->atom;
            if (atom == XCB_ATOM_NONE) {
                return;
            }
        }

        xcb_change_property(c, XCB_PROP_MODE_REPLACE, id, atom, XCB_ATOM_STRING,
                            8, value.length(), value.constData());
    };

    // TODO only set the property if it doesn't already exist

    setWindowProperty(id, s_serviceNameAtom, s_x11AppMenuServiceNamePropertyName, service.toUtf8());
    setWindowProperty(id, s_objectPathAtom, s_x11AppMenuObjectPathPropertyName, path.path().toUtf8());

    emit WindowRegistered(id, service, path);
}

void MenuImporter::UnregisterWindow(WId id)
{
    m_menuServices.remove(id);
    m_menuPaths.remove(id);
    m_windowClasses.remove(id);

    emit WindowUnregistered(id);
}

QString MenuImporter::GetMenuForWindow(WId id, QDBusObjectPath& path)
{
    path = m_menuPaths.value(id);
    return m_menuServices.value(id);
}

void MenuImporter::slotServiceUnregistered(const QString& service)
{
    WId id = m_menuServices.key(service);
    m_menuServices.remove(id);
    m_menuPaths.remove(id);
    m_windowClasses.remove(id);
    emit WindowUnregistered(id);
    m_serviceWatcher->removeWatchedService(service);
}
