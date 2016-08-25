/*****************************************************************************
  Header file for ringmessagechannel.cpp
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
      void processReceivedMessage(MapStringString payload, uint senderHandle, const QString &senderID);
      qulonglong sendRingAccountMessage(const QString &content);


  protected:
      Tp::BaseChannelMessagesInterfacePtr mMessagesInterface;
      Tp::BaseChannelChatStateInterfacePtr mChatStateInterface;

      Connection *mConnection;
      uint mTargetHandle;
      QString mTargetID;
  };
}


#endif
