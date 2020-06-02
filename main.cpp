#include <QApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include "menuimporter.h"
#include "menuimporteradaptor.h"

#include "menuproxy.h"

static const char* DBUS_SERVICE = "com.canonical.AppMenu.Registrar";
static const char* DBUS_OBJECT_PATH = "/com/canonical/AppMenu/Registrar";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qputenv("QT_QPA_PLATFORM", "xcb");

    QGuiApplication::setDesktopSettingsAware(false);
//    QCoreApplication::setAttribute(Qt::AA_DisableSessionManager);

    app.setQuitOnLastWindowClosed(false);

    MenuProxy proxy;

    /*
      Create a Chat instance and register it with the session bus only if
      the service isn't already available.
    */
    auto *m_menuImporter = new MenuImporter(&app);
    m_menuImporter->connectToBus();
//    QDBusConnection connection = QDBusConnection::sessionBus();
//    if (!connection.interface()->isServiceRegistered(DBUS_SERVICE))
//    {
//        auto *m_menuAdaptor = new MenuImporterAdaptor(m_menuImporter);
//        if (!connection.registerService(DBUS_SERVICE))
//        {
//            qFatal("Could not register service!");
//        }
//
//        if (!connection.registerObject(DBUS_OBJECT_PATH, m_menuImporter))
//        {
//            qFatal("Could not register Chat object!");
//        }
//    }

    return app.exec();
}
