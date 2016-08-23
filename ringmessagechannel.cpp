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
   mTargetHandle(baseChannel->targetHandle())
{
  qDebug() << Q_FUNC_INFO;
  QStringList supportedContentTypes = QStringList() << QLatin1String("text/plain");
  Tp::UIntList messageTypes = Tp::UIntList() << Tp::ChannelTextMessageTypeNormal
                                               << Tp::ChannelTextMessageTypeDeliveryReport;

  uint messagePartSupportFlags = Tp::MessageSendingFlagReportDelivery | Tp::MessageSendingFlagReportRead;
  uint deliveryReportingSupport = Tp::DeliveryReportingSupportFlagReceiveSuccesses | Tp::DeliveryReportingSupportFlagReceiveRead;

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
  return QString();
}

void RingMessageChannel::onMessageReceived(QString accountID, QString from, MapStringString payload)
{
  qDebug() << Q_FUNC_INFO << accountID << from;
  QMap<QString,QString>::iterator iter;
  for(iter = payload.begin(); iter != payload.end(); ++iter)
  {
    qDebug() << iter.key() << iter.value();
  }
  // processReceivedMessage(payload, mTargetHandle, from);
}

void RingMessageChannel::processReceivedMessage(MapStringString payload, uint senderHandle, const QString &senderID)
{
  qDebug() << Q_FUNC_INFO;

}

void RingMessageChannel::setChatState(uint state, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
}

void RingMessageChannel::messageAcknowledged(const QString &messageId)
{
  qDebug() << Q_FUNC_INFO;
}
