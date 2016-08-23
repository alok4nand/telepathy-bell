/*****************************************************************************
  Header file for connection.cpp
******************************************************************************
*  Copyright (C) 2016 Alok Anand
*
*  Author: Alok Anand <alok4nand@gmail.com>
*
*  Telepathy-bell is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.
*
*  Telepathy-bell is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
*/

#ifndef BELL_CONNECTION_H
#define BELL_CONNECTION_H

#include <TelepathyQt/BaseConnection>
#include <TelepathyQt/BaseChannel>
#include <QtDBus/QtDBus>
#include <QMap>

#include "parameters.hpp"



namespace Bell {
class Connection : public Tp::BaseConnection
{
   Q_OBJECT
public:
   Connection(const QDBusConnection &dbusConnection,const QString &cmName, const QString &protocolName,
              const QVariantMap &parameters);
   ~Connection();

private:
void onConnected();
uint setPresence(const QString &status, const QString &message, Tp::DBusError *error);
QStringList inspectHandles(uint handleType, const Tp::UIntList &handles, Tp::DBusError *error);
Tp::UIntList requestHandles(uint handleType, const QStringList &identifiers, Tp::DBusError *error);
QString getAlias(uint handle, Tp::DBusError *error);
Tp::ContactAttributesMap getContactAttributes(const Tp::UIntList &handles, const QStringList &ifaces, Tp::DBusError *error);
uint ensureHandle(const QString& identifier);
Tp::BaseChannelPtr createChannel(const QVariantMap &request, Tp::DBusError *error);

void sendRegister(bool enable);
void setAccountActive(bool enable);

private slots:
void doConnect(Tp::DBusError *error);
void onRegistrationStateChanged(QString accountID, QString state);
void onVolatileAccountDetailsChanged(QString accountID, MapStringString volatileAccountDetails);
void onIncomingAccountMessage(QString one, QString two , MapStringString map);
void onIncomingCall(QString accountID, QString callID, QString contact);
void doDisconnect();

private:
QString mAccountID;
QString mAlias;
Tp::BaseConnectionContactsInterfacePtr mContactsInterface;
Tp::BaseConnectionSimplePresenceInterfacePtr mSimplePresenceInterface;
Tp::BaseConnectionContactListInterfacePtr mContactListInterface;
Tp::BaseConnectionAliasingInterfacePtr mAliasingInterface;
Tp::BaseConnectionAvatarsInterfacePtr mAvatarInterface;
Tp::BaseConnectionRequestsInterfacePtr mRequestsInterface;
Tp::SimplePresence mSelfPresence;
QMap<uint,QString> mHandles;
QMap<QString, uint> mIdentifiers;
bool isConnected;
long nextHandleId;

public:
QDBusInterface mConfigurationManagerInterface;
QDBusInterface mCallManagerInterface;
QDBusInterface mInstanceInterface;
};

}

#endif
