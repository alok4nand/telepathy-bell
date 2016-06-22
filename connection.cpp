#include <TelepathyQt/Constants>
#include <TelepathyQt/BaseChannel>

#include "connection.hpp"
#include "common.hpp"
#include "protocol.hpp"

using namespace Bell;

Connection::Connection(const QDBusConnection &dbusConnection, const QString &cmName, const QString &protocolName, const QVariantMap &parameters)
: Tp::BaseConnection(dbusConnection, cmName, protocolName, parameters)
{
/* Connection Interface Setup */
/* Connection.Interface.Contacts */
mContactsInterface = Tp::BaseConnectionContactsInterface::create();
mContactsInterface->setContactAttributeInterfaces(QStringList()
                                                << TP_QT_IFACE_CONNECTION
                                                << TP_QT_IFACE_CONNECTION_INTERFACE_CONTACT_LIST
                                                << TP_QT_IFACE_CONNECTION_INTERFACE_SIMPLE_PRESENCE
                                                << TP_QT_IFACE_CONNECTION_INTERFACE_REQUESTS
                                                );
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mContactsInterface));

/* Connection.Interface.SimplePresence for receiving and reporting presence information */
mSimplePresenceInterface = Tp::BaseConnectionSimplePresenceInterface::create();
mSimplePresenceInterface->setStatuses(Common::getSimpleStatusSpecMap());
mSimplePresenceInterface->setSetPresenceCallback(Tp::memFun(this, &Connection::setPresence));
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mSimplePresenceInterface));

 /* Connection.Interface.ContactList */
 mContactListInterface = Tp::BaseConnectionContactListInterface::create();
 mContactListInterface->setContactListPersists(true);
 mContactListInterface->setCanChangeContactList(true);
 mContactListInterface->setDownloadAtConnection(true);
 plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mContactListInterface));

/* Set Callbacks for client */
setConnectCallback(Tp::memFun(this, &Connection::doConnect));
/*setInspectHandlesCallback(Tp::memFun(this, &Connection::inspectHandles));
setRequestHandlesCallback(Tp::memFun(this, &Connection::requestHandles));
*/
}

Connection::~Connection()
{

}

void Connection::doConnect(Tp::DBusError *error)
{

}

void Connection::doDisconnect()
{

}

uint Connection::setPresence(const QString &status, const QString &message, Tp::DBusError *error)
{

}
/*
QStringList inspectHandles(uint handleType, const Tp::UIntList &handles, Tp::DBusError *error)
{

}

Tp::UIntList requestHandles(uint handleType, const QStringList &identifiers, Tp::DBusError *error)
{

}
*/
