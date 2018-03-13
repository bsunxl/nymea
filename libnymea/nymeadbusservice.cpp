/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2017 Michael Zanetti <michael.zanetti@guh.io>            *
 *  Copyright (C) 2018 Simon Stürz <simon.stuerz@guh.io>                   *
 *                                                                         *
 *  This file is part of nymea.                                            *
 *                                                                         *
 *  nymea is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  nymea is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with nymea. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "nymeadbusservice.h"
#include "loggingcategories.h"

QDBusConnection NymeaDBusService::s_connection = QDBusConnection::systemBus();

NymeaDBusService::NymeaDBusService(const QString &objectPath, QObject *parent) : QObject(parent)
{
    bool status = s_connection.registerService("io.guh.nymead");
    if (!status) {
        qCWarning(dcApplication()) << "Failed to register D-Bus service.";
        return;
    }
    QString finalObjectPath;
    foreach (const QString &part, objectPath.split(' ')) {
        finalObjectPath.append(part.at(0).toUpper());
        finalObjectPath.append(part.right(part.length() - 1));
    }
    status = s_connection.registerObject(finalObjectPath, this, QDBusConnection::ExportScriptableSlots);
    if (!status) {
        qCWarning(dcApplication()) << "Failed to register D-Bus object:" << finalObjectPath;
        return;
    }
    m_isValid = true;
}

bool NymeaDBusService::isValid()
{
    return m_isValid;
}

QDBusConnection NymeaDBusService::connection() const
{
    return s_connection;
}

void NymeaDBusService::setBusType(QDBusConnection::BusType busType)
{
    if (busType == QDBusConnection::SessionBus) {
        s_connection = QDBusConnection::sessionBus();
    } else {
        s_connection = QDBusConnection::systemBus();
    }
}

