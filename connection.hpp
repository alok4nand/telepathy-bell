#ifndef BELL_CONNECTION_H
#define BELL_CONNECTION_H

#include <TelepathyQt/BaseConnection>
#include <TelepathyQt/BaseChannel>
#include <QtDBus/QtDBus>
#include <QMap>

#include "parameters.hpp"

/*This models a connection to a single user account.*/

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
void onIncomingMessage(QString one, QString two , MapStringString map);
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
