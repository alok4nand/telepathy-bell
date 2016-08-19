#include <TelepathyQt/Constants>

#include "connection.hpp"
#include "common.hpp"
#include "protocol.hpp"

using namespace Bell;

Connection::Connection(const QDBusConnection&dbusConnection, const QString &cmName, const QString &protocolName, const QVariantMap &parameters)
: Tp::BaseConnection(dbusConnection, cmName, protocolName, parameters),
mConfigurationManagerInterface("cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager"),
mCallManagerInterface("cx.ring.Ring","/cx/ring/Ring/CallManager","cx.ring.Ring.CallManager"),
mInstanceInterface("cx.ring.Ring","/cx/ring/Ring/Instance","cx.ring.Ring.Instance"),
isConnected(false),
nextHandleId(1)
{
qDebug() << Q_FUNC_INFO;
/* Connection Interfaces Setup */
/* Connection.Interface.Contacts */
mContactsInterface = Tp::BaseConnectionContactsInterface::create();
mContactsInterface->setGetContactAttributesCallback(Tp::memFun(this, &Connection::getContactAttributes));
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
// mAliasingInterface->setGetAliasesCallback(Tp::memFun(this, &Connection::getAliases));
// mAliasingInterface->setSetAliasesCallback(Tp::memFun(this, &Connection::setAliases));
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mAliasingInterface));

/* Connection.Interface.Avatars */
mAvatarInterface = Tp::BaseConnectionAvatarsInterface::create();
mAvatarInterface->setAvatarDetails(Common::getAvatarSpec());
// mAvatarInterface->setGetKnownAvatarTokensCallback(Tp::memFun(this, &Connection::getKnownAvatarTokens));
// mAvatarInterface->setRequestAvatarsCallback(Tp::memFun(this, &Connection::requestAvatars));
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mAvatarInterface));

/* Connection.Interface.Requests */
mRequestsInterface = Tp::BaseConnectionRequestsInterface::create(this);

Tp::RequestableChannelClass text;
text.fixedProperties[TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")] = TP_QT_IFACE_CHANNEL_TYPE_TEXT;
text.fixedProperties[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType")]  = Tp::HandleTypeContact;
text.allowedProperties.append(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle"));
text.allowedProperties.append(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID"));

Tp::RequestableChannelClass call;
call.fixedProperties[TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")] = TP_QT_IFACE_CHANNEL_TYPE_CALL;
call.fixedProperties[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType")]  = Tp::HandleTypeContact;
call.fixedProperties[TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialAudio"]  = true;
//call.fixedProperties[TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialVideo"]  = false;
call.allowedProperties.append(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle"));
call.allowedProperties.append(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID"));
call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialAudio");
//call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialVideo");
call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialAudioName");
//call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialVideoName");
call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".InitialTransport");
call.allowedProperties.append(TP_QT_IFACE_CHANNEL_TYPE_CALL+".HardwareStreaming");
//call.allowedProperties.append(TP_QT_IFACE_CHANNEL_INTERFACE_CONFERENCE + QLatin1String(".InitialChannels"));

mRequestsInterface->requestableChannelClasses << text << call;
plugInterface(Tp::AbstractConnectionInterfacePtr::dynamicCast(mRequestsInterface));

// for(QVariantMap::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
//      qDebug() << iter.key() << iter.value();
//     }
mAccountID = parameters[QLatin1String("AccountID")].toString();
QString mRingID = parameters[QLatin1String("RingID")].toString();
/* Setting self contact */
uint _self = ensureHandle(mRingID);
setSelfContact(_self, mRingID);

/* Set Callbacks for client */
setConnectCallback(Tp::memFun(this, &Connection::doConnect));
setInspectHandlesCallback(Tp::memFun(this, &Connection::inspectHandles));
setRequestHandlesCallback(Tp::memFun(this, &Connection::requestHandles));
connect(this, SIGNAL(disconnected()), SLOT(doDisconnect()));
}

Connection::~Connection()
{
  qDebug() << Q_FUNC_INFO;

}

void Connection::sendRegister(bool enable)
{
  qDebug() << Q_FUNC_INFO << enable ;
  mConfigurationManagerInterface.call("sendRegister",mAccountID,enable);
}

void Connection::setAccountActive(bool enable)
{
  qDebug() << Q_FUNC_INFO << enable ;
  mConfigurationManagerInterface.call("setAccountActive",mAccountID,enable);
}

void Connection::doConnect(Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << mAccountID;
  setStatus(Tp::ConnectionStatusConnecting, Tp::ConnectionStatusReasonRequested);
  sendRegister(true);
  mContactListInterface->setContactListState(Tp::ContactListStateWaiting);

  mConfigurationManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
    "registrationStateChanged",this,SLOT(onRegistrationStateChanged(QString, QString)));
  mConfigurationManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
    "volatileAccountDetailsChanged",this,SLOT(onVolatileAccountDetailsChanged(QString, MapStringString)));
}

void Connection::onRegistrationStateChanged(QString accountID, QString state)
{
  qDebug() << Q_FUNC_INFO << accountID << state ;
  if(accountID == mAccountID && state == "REGISTERED")
    {
    onConnected();
    }
  if(accountID == mAccountID && state == "TRYING")
    {
    qDebug() << "TRYING" ;
    }
  if(accountID == mAccountID && state == "UNREGISTERED")
    {
   emit disconnected();
    }
}

void Connection::onConnected()
{
  setStatus(Tp::ConnectionStatusConnected, Tp::ConnectionStatusReasonRequested);
  mSelfPresence.type = Tp::ConnectionPresenceTypeAvailable;
  mSelfPresence.status = QLatin1String("available");
//   Tp::SimpleContactPresences presences;
//   presences[selfHandle()] = mSelfPresence;
//   mSimplePresenceInterface->setPresences(presences);
 }

 void Connection::doDisconnect()
 {
   qDebug() << Q_FUNC_INFO << mAccountID;
   sendRegister(false);
   setStatus(Tp::ConnectionStatusDisconnected, Tp::ConnectionStatusReasonRequested);
 }

uint Connection::setPresence(const QString &status, const QString &message, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << status << message << error;
 // TODO
  return selfHandle();
}

uint Connection::ensureHandle(const QString &identifier)
{
  qDebug() << Q_FUNC_INFO;
  if(!mIdentifiers.contains(identifier)) {
    long id = nextHandleId++;
    mHandles[id] = identifier;
    mIdentifiers[identifier] = id;
    }
  return mIdentifiers[identifier];
}

Tp::UIntList Connection::requestHandles(uint handleType, const QStringList &identifiers, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
  Tp::UIntList result;

  if(handleType != Tp::HandleTypeContact) {
     error->set(TP_QT_ERROR_INVALID_ARGUMENT, QLatin1String("Unknown Handle type"));
     return result;
  }

  Q_FOREACH(const QString &identifier,  identifiers) {
     ensureHandle(identifier);
     result.append(mIdentifiers[identifier]);
  }

  return result;
}

QStringList Connection::inspectHandles(uint handleType, const Tp::UIntList &handles, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
   if(handleType != Tp::HandleTypeContact) {
      error->set(TP_QT_ERROR_INVALID_ARGUMENT, QLatin1String("Unsupported handle type"));
      return QStringList();
   }

   QStringList result;

   foreach (uint handle, handles) {
      if(!mHandles.contains(handle)) {
         return QStringList();
      }

      result.append(mHandles.value(handle));
   }
   return result;
}

Tp::ContactAttributesMap Connection::getContactAttributes(const Tp::UIntList &handles, const QStringList &ifaces, Tp::DBusError *error)
{
    qDebug() << Q_FUNC_INFO << handles << ifaces;
    Tp::ContactAttributesMap attributesMap;
    Q_FOREACH(uint handle, handles) {
        QVariantMap attributes;
        QStringList inspectedHandles = inspectHandles(Tp::HandleTypeContact, Tp::UIntList() << handle, error);
        if (inspectedHandles.size() > 0) {
            attributes[TP_QT_IFACE_CONNECTION+"/contact-id"] = inspectedHandles.at(0);
        } else {
            continue;
        }
        if (ifaces.contains(TP_QT_IFACE_CONNECTION_INTERFACE_SIMPLE_PRESENCE)) {
            attributes[TP_QT_IFACE_CONNECTION_INTERFACE_SIMPLE_PRESENCE+"/presence"] = QVariant::fromValue(mSelfPresence);
        }
        attributesMap[handle] = attributes;
    }
    return attributesMap;
}
