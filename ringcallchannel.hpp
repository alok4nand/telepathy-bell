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
  RingCallChannel(bool incoming, Connection* connection, QString peer, uint targetHandle);
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
  Tp::BaseChannelPtr mBaseChannel;
  Tp::BaseChannelCallTypePtr mCallChannel;
  Tp::BaseChannelHoldInterfacePtr mHoldIface;
  Tp::BaseCallMuteInterfacePtr mMuteIface;
  Tp::BaseCallContentPtr mCallContent;
};

}


#endif
