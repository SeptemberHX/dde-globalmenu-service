//
// Created by ragdoll on 2022/4/3.
//

#ifndef DDE_GLOBALMENU_SERVICE_DEBUG_H
#define DDE_GLOBALMENU_SERVICE_DEBUG_H

#include <QLoggingCategory>

static const QLoggingCategory &DBUSMENUPROXY() {
    static const QLoggingCategory category("DBusMenuProxy");
    return category;
}

#endif //DDE_GLOBALMENU_SERVICE_DEBUG_H
