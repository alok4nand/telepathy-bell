#include <TelepathyQt/Constants>

#include "connection.hpp"
#include "common.hpp"
#include "protocol.hpp"

using namespace Bell;

Connection::Connection(const QDBusConnection&dbusConnection, const QString &cmName, const QString &protocolName, const QVariantMap &parameters)
: Tp::BaseConnection(dbusConnection, cmName, protocolName, parameters),
mConfigurationManagerInterface("cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager"),
mCallManagerInterface("cx.ring.Ring","/cx/ring/Ring/CallManager","cx.ring.Ring.CallManager"),
mInstanceInterface("cx.ring.Ring","/cx/ring/Ring/Instance","cx.ring.Ring.Instance")
{
qDebug() << Q_FUNC_INFO;
// for(QVariantMap::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
//      qDebug() << iter.key() << iter.value();
//     }
mAccountID = parameters[QLatin1String("AccountID")].toString();
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

 /* Connection.Interface.Aliasing */
mAliasingInterface = Tp::BaseConnectionAliasingInterface::create();
//mAliasingInterface->setGetAliasesCallback(Tp::memFun(this, &Connection::getAliases));
//mAliasingInterface->setSetAliasesCallback(Tp::memFun(this, &Connection::setAliases));
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mAliasingInterface));

/* Connection.Interface.Avatars */
mAvatarInterface = Tp::BaseConnectionAvatarsInterface::create();
mAvatarInterface->setAvatarDetails(Common::getAvatarSpec());
//mAvatarInterface->setGetKnownAvatarTokensCallback(Tp::memFun(this, &Connection::getKnownAvatarTokens));
//mAvatarInterface->setRequestAvatarsCallback(Tp::memFun(this, &Connection::requestAvatars));
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mAvatarInterface));

/* Connection.Interface.Requests */
mRequestsInterface = Tp::BaseConnectionRequestsInterface::create(this);
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mRequestsInterface));

/* Set Callbacks for client */
setConnectCallback(Tp::memFun(this, &Connection::doConnect));
/*setInspectHandlesCallback(Tp::memFun(this, &Connection::inspectHandles));
setRequestHandlesCallback(Tp::memFun(this, &Connection::requestHandles));
*/
mConfigurationManagerInterface.connection().connect(
  "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
  "registrationStateChanged",this,SLOT(onRegistrationStateChanged(QString, QString)));
connect(this, SIGNAL(disconnected()), SLOT(doDisconnect()));
}

Connection::~Connection()
{
  qDebug() << Q_FUNC_INFO;

}

void Connection::doConnect(Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << mAccountID;
  mConfigurationManagerInterface.call("sendRegister",mAccountID,true);
  //TODO SET presences

}

void Connection::onRegistrationStateChanged(QString accountID, QString state)
{
  qDebug() << Q_FUNC_INFO << accountID << state ;
  if (accountID == mAccountID && state == "REGISTERED"){
    setStatus(Tp::ConnectionStatusConnected, Tp::ConnectionStatusReasonRequested);
  }
  if  (accountID == mAccountID && state == "TRYING"){
   setStatus(Tp::ConnectionStatusConnecting, Tp::ConnectionStatusReasonRequested);
  }
  if (accountID == mAccountID && state == "UNREGISTERED"){
    setStatus(Tp::ConnectionStatusDisconnected, Tp::ConnectionStatusReasonRequested);
    emit(disconnected());
  }
}

void Connection::doDisconnect()
{
  qDebug() << Q_FUNC_INFO << mAccountID;
  mConfigurationManagerInterface.call("setAccountActive",mAccountID,false);
}

uint Connection::setPresence(const QString &status, const QString &message, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << status << message << error;
  // TODO
  return selfHandle();
}
/*
QStringList inspectHandles(uint handleType, const Tp::UIntList &handles, Tp::DBusError *error)
{

}

Tp::UIntList requestHandles(uint handleType, const QStringList &identifiers, Tp::DBusError *error)
{

}
*/
