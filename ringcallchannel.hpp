/*****************************************************************************
  Header file for ringcallchannel.cpp
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

#ifndef BELL_RING_CALL_CHANNEL
#define BELL_RING_CALL_CHANNEL

#include <TelepathyQt/Constants>
#include <TelepathyQt/BaseChannel>
#include <TelepathyQt/BaseCall>
#include <TelepathyQt/Types>
#include "connection.hpp"

namespace Bell{

class Connection;

class RingCallChannel: public QObject
{
  Q_OBJECT
public:
  RingCallChannel(bool incoming, Connection* connection, QString peer, uint targetHandle, QString callID);
  Tp::BaseChannelPtr baseChannel();

  void onAccept(Tp::DBusError*);
  void onHangup(uint reason, const QString &detailedReason, const QString &message, Tp::DBusError* error);
  void onMuteStateChanged(const Tp::LocalMuteState &state, Tp::DBusError *error);
  void onHoldStateChanged(const Tp::LocalHoldState &state, const Tp::LocalHoldStateReason &reason, Tp::DBusError *error);

Q_SIGNALS:
void hangupComplete(bool);
void answerComplete(bool);

private Q_SLOTS:
  void onAnswerComplete(bool success);
  void onHangupComplete(bool success);
  void setCallState(const QString &state);

private:
  bool mIncoming;
  Connection* mConnection;
  QString mPeer;
  uint mTargetHandle;
  QString mCallID;
  Tp::BaseChannelPtr mBaseChannel;
  Tp::BaseChannelCallTypePtr mCallChannel;
  Tp::BaseChannelHoldInterfacePtr mHoldIface;
  Tp::BaseCallMuteInterfacePtr mMuteIface;
  Tp::BaseCallContentPtr mCallContent;
};

}


#endif
