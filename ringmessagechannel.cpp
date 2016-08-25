/*****************************************************************************
  Channel used for sending and receiving text messages.
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
#include "ringmessagechannel.hpp"
#include "connection.hpp"

using namespace Bell;

RingMessageChannel::RingMessageChannel(Connection *connection, Tp::BaseChannel *baseChannel)
  :Tp::BaseChannelTextType(baseChannel),
   mConnection(connection),
   mTargetHandle(baseChannel->targetHandle()),
   mTargetID(baseChannel->targetID())
{
  qDebug() << Q_FUNC_INFO;
  QStringList supportedContentTypes = QStringList() << QLatin1String("text/plain");
  Tp::UIntList messageTypes = Tp::UIntList() << Tp::ChannelTextMessageTypeNormal;

  uint messagePartSupportFlags = 0;
  uint deliveryReportingSupport = 0;
  setMessageAcknowledgedCallback(Tp::memFun(this, &RingMessageChannel::messageAcknowledged));
  mMessagesInterface = Tp::BaseChannelMessagesInterface::create(this,
                                                               supportedContentTypes,
                                                               messageTypes,
                                                               messagePartSupportFlags,
                                                               deliveryReportingSupport);

  baseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast(mMessagesInterface));
  mMessagesInterface->setSendMessageCallback(Tp::memFun(this, &RingMessageChannel::sendMessage));

  mChatStateInterface = Tp::BaseChannelChatStateInterface::create();
  mChatStateInterface->setSetChatStateCallback(Tp::memFun(this, &RingMessageChannel::setChatState));
  baseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast(mChatStateInterface));
}


RingMessageChannelPtr RingMessageChannel::create(Connection *connection, Tp::BaseChannel *baseChannel)
{
  return RingMessageChannelPtr(new RingMessageChannel(connection, baseChannel));
}

QString RingMessageChannel::sendMessage(const Tp::MessagePartList &messageParts, uint flags, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
  QString content;
  QString messageID;
  for(auto &part : messageParts)
  {
  if(part.count(QLatin1String("content-type")) &&
     part.value(QLatin1String("content-type")).variant().toString() == QLatin1String("text/plain") &&
     part.count(QLatin1String("content")))
    {
    content = part.value(QLatin1String("content")).variant().toString();
    break;
    }
  }
  qulonglong id = sendRingAccountMessage(content);
  qDebug() << "Sent Message ID:" << id ;
  return messageID.number(id);
}

void RingMessageChannel::onMessageReceived(QString accountID, QString from, MapStringString payload)
{
  qDebug() << Q_FUNC_INFO ;
  processReceivedMessage(payload, mTargetHandle, mTargetID);
}

void RingMessageChannel::processReceivedMessage(MapStringString payload, uint senderHandle, const QString &senderID)
{
  qDebug() << Q_FUNC_INFO << "Sender Handle:" << senderHandle << "Sender RingID:" << senderID;
  QMap<QString,QString>::iterator iter;
  for(iter = payload.begin(); iter != payload.end(); ++iter)
  {
    qDebug() << iter.key() << iter.value();
  }

}

qulonglong RingMessageChannel::sendRingAccountMessage(const QString &content)
{
  qDebug() << Q_FUNC_INFO << content ;
  QString accountID = mConnection->getAccountID();
  QString to = mTargetID;
  MapStringString payload;
  payload["text/plain"] = content;

  QList<QVariant> argumentList;
  argumentList << accountID << to << QVariant::fromValue(payload);

  QDBusPendingCall mPendingCallReply = mConnection->mConfigurationManagerInterface.asyncCallWithArgumentList("sendTextMessage",argumentList);
  QDBusPendingReply<qulonglong> mPendingReply(mPendingCallReply);
  if(mPendingReply.isError())
  {
  qDebug() << "Error sending account text message";
  }
  return mPendingReply;
}

void RingMessageChannel::setChatState(uint state, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
}

void RingMessageChannel::messageAcknowledged(const QString &messageId)
{
  qDebug() << Q_FUNC_INFO;
}
