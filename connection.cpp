/*****************************************************************************
    This models a connection to a single user account.
    It also provides capability to request and receive
    channels for communcication using Text and Calls.
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

#include <TelepathyQt/Constants>

#include "connection.hpp"
#include "common.hpp"
#include "protocol.hpp"
#include "ringcallchannel.hpp"
#include "ringmessagechannel.hpp"

using namespace Bell;

Connection::Connection(const QDBusConnection&dbusConnection, const QString &cmName, const QString &protocolName, const QVariantMap &parameters)
: Tp::BaseConnection(dbusConnection, cmName, protocolName, parameters),
isConnected(false),
nextHandleId(1),
mConfigurationManagerInterface("cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager"),
mCallManagerInterface("cx.ring.Ring","/cx/ring/Ring/CallManager","cx.ring.Ring.CallManager"),
mInstanceInterface("cx.ring.Ring","/cx/ring/Ring/Instance","cx.ring.Ring.Instance")
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
                                                << TP_QT_IFACE_CONNECTION_INTERFACE_ALIASING
                                                // << TP_QT_IFACE_CONNECTION_INTERFACE_AVATARS
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
mAliasingInterface->setGetAliasesCallback(Tp::memFun(this, &Connection::getAliases));
mAliasingInterface->setSetAliasesCallback(Tp::memFun(this, &Connection::setAliases));
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
mAlias = parameters[QLatin1String("Username")].toString();
QString mRingID = parameters[QLatin1String("RingID")].toString();
/* Setting self contact */
uint _self = ensureHandle(mRingID);
setSelfContact(_self, mRingID);

/* Set Callbacks for client */
setConnectCallback(Tp::memFun(this, &Connection::doConnect));
setInspectHandlesCallback(Tp::memFun(this, &Connection::inspectHandles));
setCreateChannelCallback(Tp::memFun(this, &Connection::createChannel));
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

  qDBusRegisterMetaType<MapStringString>();
  mConfigurationManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
    "registrationStateChanged",this,SLOT(onRegistrationStateChanged(QString, QString)));
  mConfigurationManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
    "volatileAccountDetailsChanged",this,SLOT(onVolatileAccountDetailsChanged(QString, MapStringString)));
  mConfigurationManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager",
    "incomingAccountMessage",this,SLOT(onIncomingAccountMessage(QString, QString, MapStringString)));
  mCallManagerInterface.connection().connect(
    "cx.ring.Ring","/cx/ring/Ring/CallManager","cx.ring.Ring.CallManager",
    "incomingCall",this,SLOT(onIncomingCall(QString, QString, QString )));
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

void Connection::onVolatileAccountDetailsChanged(QString accountID, MapStringString volatileAccountDetails)
{
  qDebug() << Q_FUNC_INFO << accountID ;
  QMap<QString,QString>::iterator iter;
  for(iter = volatileAccountDetails.begin(); iter != volatileAccountDetails.end(); ++iter)
  {
        qDebug() << iter.key() << iter.value();
  }
}

void Connection::onConnected()
{
  setStatus(Tp::ConnectionStatusConnected, Tp::ConnectionStatusReasonRequested);
  isConnected = true;
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

void Connection::onIncomingAccountMessage(QString accountID, QString from, MapStringString payload)
{
  qDebug() << Q_FUNC_INFO ;
  QString fromID = "ring:" + from;
  uint handle = ensureHandle(fromID);
  uint initiatorHandle = handle;
  QVariantMap request;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")] = TP_QT_IFACE_CHANNEL_TYPE_TEXT;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType")] = Tp::HandleTypeContact;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle")] = handle;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".InitiatorHandle")] = initiatorHandle;

  bool yours;
  Tp::DBusError error;
  Tp::BaseChannelPtr channel = ensureChannel(request, yours, false, &error);

  if (error.isValid())
  {
    qWarning() << "ensureChannel failed:" << error.name() << " " << error.message();
    return;
  }

  RingMessageChannelPtr RingMessageChannel = RingMessageChannelPtr::dynamicCast(channel->interface(TP_QT_IFACE_CHANNEL_TYPE_TEXT));
  if (!RingMessageChannel)
  {
    qDebug() << "Error, channel is not a RingMessageChannel?";
    return;
  }
  RingMessageChannel->onMessageReceived (accountID, fromID, payload);
}

void Connection::onIncomingCall(QString accountID, QString callID, QString contact)
{
  qDebug() << Q_FUNC_INFO << accountID << callID << contact ;
  QString contactAlias;
  QString contactID;
  QString contactRingID;
  QStringList  contactDetails = contact.split(" <");
  if(!contactDetails.isEmpty())
  {
  contactAlias = contactDetails.first();
  contactID = contactDetails.last();
  // ContactID is in form "ring:ID" + @ring.dht.
  QStringList idList = contactID.split('@');
  contactRingID = "ring:" + idList.first();
  }
  // setAlias will also call ensureHandle();
  uint handle = setAlias(contactAlias, contactRingID);
  qDebug() << handle;
  uint initiatorHandle = handle;
  QVariantMap request;
  // request[TP_QT_IFACE_CHANNEL + QLatin1String(".Requested")] = false;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")] = TP_QT_IFACE_CHANNEL_TYPE_CALL;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType")] = Tp::HandleTypeContact;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle")] = handle;
  request[TP_QT_IFACE_CHANNEL + QLatin1String(".InitiatorHandle")] = initiatorHandle;
  request["Call_ID"] = callID;

  bool yours;
  Tp::DBusError error;
  Tp::BaseChannelPtr channel = ensureChannel(request, yours, false, &error);

  if (error.isValid() || channel.isNull()) {
  qWarning() << "error creating the channel " << error.name() << error.message();
    return;
}

}

uint Connection::setPresence(const QString &status, const QString &message, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << status << message << error;
  /* Presence features are not implemented in Ring.
  All the other presences other than "Offline" shall keep the account
  in registered mode"
  */
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

uint Connection::setAlias(const QString &alias, const QString &identifier)
{
  qDebug() << Q_FUNC_INFO;
  uint id ;
  if(mIdentifiers.contains(identifier))
  {
    id = mIdentifiers[identifier];
    mAliases[id] = alias;
  }
  else{
    ensureHandle(identifier);
    id = mIdentifiers[identifier];
    mAliases[id] = alias;
  }
  return id;
}

QString Connection::getAlias(uint handle, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << handle;
  Q_UNUSED(error);

  if (handle == selfHandle()) {
      return mAlias;
  }
  QString alias = mAliases[handle];
  return alias;
}

Tp::AliasMap Connection::getAliases(const Tp::UIntList &handles, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
  for(uint handle : handles)
  {
    mAliases[handle] = getAlias(handle, error);
  }
  return mAliases;
}

void Connection::setAliases(const Tp::AliasMap &aliases, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
  //TODO
  // Since the daemon does handle contacts setting alias for all contacts
  // for an account at once is not implemented.
  error->set(TP_QT_ERROR_NOT_IMPLEMENTED, QLatin1String("Not implemented"));
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
        if (ifaces.contains(TP_QT_IFACE_CONNECTION_INTERFACE_ALIASING)) {
            attributes[TP_QT_IFACE_CONNECTION_INTERFACE_ALIASING + QLatin1String("/alias")] = QVariant::fromValue(getAlias(handle, error));
        }
        if (ifaces.contains(TP_QT_IFACE_CONNECTION_INTERFACE_SIMPLE_PRESENCE)) {
            attributes[TP_QT_IFACE_CONNECTION_INTERFACE_SIMPLE_PRESENCE+"/presence"] = QVariant::fromValue(mSelfPresence);
        }
        attributesMap[handle] = attributes;
    }
    return attributesMap;
}

  Tp::BaseChannelPtr Connection::createChannel(const QVariantMap &request, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO ;
  // for(QVariantMap::const_iterator iter = request.begin(); iter != request.end(); ++iter) {
  //       qDebug() << iter.key() << iter.value();
  //      }
  uint targetHandleType = Tp::HandleTypeNone;
  uint targetHandle = 0;
  QString targetID;

  bool requested = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".Requested")).toBool();
  const QString channelType = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".ChannelType")).toString();
  targetHandleType = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandleType")).toUInt();
  uint initiatorHandle = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".InitiatorHandle")).toUInt();

  switch (targetHandleType)
  {
  case Tp::HandleTypeContact:
    if (request.contains(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle")))
    {
      targetHandle = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetHandle")).toUInt();
      targetID = mHandles[targetHandle];
    }
    else if (request.contains(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID")))
    {
      targetID = request.value(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID")).toString();
      targetHandle = ensureHandle(targetID);
    }
    break;
  default:
    if (error)
    {
      error->set(TP_QT_ERROR_INVALID_ARGUMENT, QLatin1String("Unknown Handle type for target"));
      return Tp::BaseChannelPtr();
    }
   break;
  }

  if (targetHandleType == Tp::HandleTypeNone)
   {
     if (error)
       {
       error->set(TP_QT_ERROR_INVALID_ARGUMENT, QLatin1String("Target handle type is not present in the request details."));
       }
     return Tp::BaseChannelPtr();
   }
   if(targetID.isEmpty())
   {
     if(error)
       {
       error->set(TP_QT_ERROR_INVALID_HANDLE, QLatin1String("Target handle is unknown."));
       }
       return Tp::BaseChannelPtr();
   }

   Tp::BaseChannelPtr baseChannel = Tp::BaseChannel::create(this, channelType, Tp::HandleType(targetHandleType), targetHandle);
   baseChannel->setTargetID(targetID);
   baseChannel->setRequested(requested);

  if (channelType == TP_QT_IFACE_CHANNEL_TYPE_TEXT) {
      if (targetHandleType == Tp::HandleTypeContact)
      {
        qDebug() << "Incoming Message Channel";
        RingMessageChannelPtr RingMessageChannel = RingMessageChannel::create(this, baseChannel.data());
        baseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast(RingMessageChannel));
      }
  }
  else if (channelType == TP_QT_IFACE_CHANNEL_TYPE_CALL )
  {
      qDebug() << "Incoming call Channel";
      QString callID = request.value("Call_ID").toString();
      RingCallChannel *channel = new RingCallChannel(true, this, targetID, targetHandle, callID);
      channel->baseChannel()->setInitiatorHandle(initiatorHandle);
      return channel->baseChannel();
      // baseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast());
  }
  return baseChannel;
}
