#ifndef BELL_CONNECTION_H
#define BELL_CONNECTION_H

#include <TelepathyQt/BaseConnection>
#include <TelepathyQt/BaseChannel>
#include <QtDBus/QtDBus>
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
uint setPresence(const QString &status, const QString &message, Tp::DBusError *error);
//QStringList inspectHandles(uint handleType, const Tp::UIntList &handles, Tp::DBusError *error);
//Tp::UIntList requestHandles(uint handleType, const QStringList &identifiers, Tp::DBusError *error);

private slots:
void doConnect(Tp::DBusError *error);
void onRegistrationStateChanged(QString accountID, QString state);
void doDisconnect();

private:
QString mAccountID;
Tp::BaseConnectionContactsInterfacePtr mContactsInterface;
Tp::BaseConnectionSimplePresenceInterfacePtr mSimplePresenceInterface;
Tp::BaseConnectionContactListInterfacePtr mContactListInterface;
Tp::BaseConnectionAliasingInterfacePtr mAliasingInterface;
Tp::BaseConnectionAvatarsInterfacePtr mAvatarInterface;
Tp::BaseConnectionRequestsInterfacePtr mRequestsInterface;
QDBusInterface mConfigurationManagerInterface;
QDBusInterface mCallManagerInterface;
QDBusInterface mInstanceInterface;
};

}

#endif
