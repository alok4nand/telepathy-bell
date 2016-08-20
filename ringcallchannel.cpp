#include "ringcallchannel.hpp"

#include <QtDebug>

using namespace Bell;

RingCallChannel::RingCallChannel(bool incoming, Connection* connection, QString peer, uint targetHandle, QString callID)
: mIncoming(incoming),
  mConnection(connection),
  mPeer(peer),
  mTargetHandle(targetHandle),
  mCallID(callID)
{
  mBaseChannel = Tp::BaseChannel::create(mConnection, TP_QT_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact, targetHandle);
  mBaseChannel->setTargetID(peer);

  Tp::BaseChannelCallTypePtr callType = Tp::BaseChannelCallType::create(mBaseChannel.data(),
                                                                         true,
                                                                         Tp::StreamTransportTypeUnknown,
                                                                         true,
                                                                         false, "audio","");
  mBaseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast(callType));

  mHoldIface = Tp::BaseChannelHoldInterface::create();
  mHoldIface->setSetHoldStateCallback(Tp::memFun(this,&RingCallChannel::onHoldStateChanged));

  mMuteIface = Tp::BaseCallMuteInterface::create();
  mMuteIface->setSetMuteStateCallback(Tp::memFun(this,&RingCallChannel::onMuteStateChanged));

  mCallChannel = Tp::BaseChannelCallTypePtr::dynamicCast(mBaseChannel->interface(TP_QT_IFACE_CHANNEL_TYPE_CALL));
  mCallChannel->setHangupCallback(Tp::memFun(this,&RingCallChannel::onHangup));
  mCallChannel->setAcceptCallback(Tp::memFun(this,&RingCallChannel::onAccept));

  connect(this, SIGNAL(hangupComplete(bool)), this, SLOT(onHangupComplete(bool)));
  connect(this, SIGNAL(answerComplete(bool)), this, SLOT(onAnswerComplete(bool)));
}

Tp::BaseChannelPtr RingCallChannel::baseChannel()
{
  qDebug() << Q_FUNC_INFO;
  return mBaseChannel;
}

void RingCallChannel::onAccept(Tp::DBusError*)
{
  qDebug() << Q_FUNC_INFO;
  mConnection->mCallManagerInterface.call("accept",mCallID);
}

void RingCallChannel::onHangup(uint reason, const QString &detailedReason, const QString &message, Tp::DBusError* error)
{
  qDebug() << Q_FUNC_INFO;
  reason = 2;
  mConnection->mCallManagerInterface.call("hangup",mCallID);

}

void RingCallChannel::onAnswerComplete(bool success)
{
  qDebug() << Q_FUNC_INFO;
}

void RingCallChannel::onHangupComplete(bool success)
{
  qDebug() << Q_FUNC_INFO;
}

void
RingCallChannel::onHoldStateChanged(const Tp::LocalHoldState &state, const Tp::LocalHoldStateReason &reason, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;
}

void
RingCallChannel::onMuteStateChanged(const Tp::LocalMuteState &state, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO;

}

void
RingCallChannel::setCallState(const QString &state)
{
  qDebug() << Q_FUNC_INFO;

}
