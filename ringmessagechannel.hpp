#ifndef BELL_RING_MESSAGE_CHANNEL
#define BELL_RING_MESSAGE_CHANNEL

#include <TelepathyQt/BaseChannel>
#include <TelepathyQt/Constants>
#include <TelepathyQt/Types>

#include "connection.hpp"


namespace Bell {
  class RingMessageChannel;
  typedef Tp::SharedPtr<RingMessageChannel> RingMessageChannelPtr;
  class RingMessageChannel : public Tp::BaseChannelTextType
  {
      Q_OBJECT
  public:
      static RingMessageChannelPtr create(Connection *connection, Tp::BaseChannel *baseChannel);

  public slots:
      virtual void onMessageReceived(QString accountID, QString from, MapStringString payload);

  protected:
      RingMessageChannel(Connection *connection, Tp::BaseChannel *baseChannel);
      QString sendMessage(const Tp::MessagePartList &messageParts, uint flags, Tp::DBusError *error);
      void setChatState(uint state, Tp::DBusError *error);
      void messageAcknowledged(const QString &messageId);

  protected:
      Tp::BaseChannelMessagesInterfacePtr mMessagesInterface;
      Tp::BaseChannelChatStateInterfacePtr mChatStateInterface;

      Connection *mConnection;
      uint mTargetHandle;
  };
}


#endif
