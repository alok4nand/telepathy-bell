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

}

void RingMessageChannel::setChatState(uint state, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
}

void RingMessageChannel::messageAcknowledged(const QString &messageId)
{
  qDebug() << Q_FUNC_INFO;
}
