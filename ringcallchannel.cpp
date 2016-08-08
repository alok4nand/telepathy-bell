#include "ringcallchannel.hpp"

#include <QtDebug>

using namespace Bell;

RingCallChannel::RingCallChannel(bool incoming, Connection* connection, QString peer, uint targetHandle)
: mIncoming(incoming),
  mConnection(connection),
  mPeer(peer),
  mTargetHandle(targetHandle)
{
  // mBaseChannel = Tp::BaseChannel::create(mConnection, TP_QT_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact, targetHandle);
  // mBaseChannel->setTargetID(peer);
  //
  // Tp::BaseChannelCallTypePtr callType = Tp::BaseChannelCallType::create(mBaseChannel.data(),
  //                                                                        true,
  //                                                                        Tp::StreamTransportTypeUnknown,
  //                                                                        true,
  //                                                                        false, "audio","");
  // mBaseChannel->plugInterface(Tp::AbstractChannelInterfacePtr::dynamicCast(callType));
  //
  // mHoldIface = Tp::BaseChannelHoldInterface::create();
  // mHoldIface->setSetHoldStateCallback(Tp::memFun(this,&SipCallChannel::onHoldStateChanged));
  //
  // mMuteIface = Tp::BaseCallMuteInterface::create();
  // mMuteIface->setSetMuteStateCallback(Tp::memFun(this,&SipCallChannel::onMuteStateChanged));
  //
  // mCallChannel = Tp::BaseChannelCallTypePtr::dynamicCast(mBaseChannel->interface(TP_QT_IFACE_CHANNEL_TYPE_CALL));
  // mCallChannel->setHangupCallback(Tp::memFun(this,&SipCallChannel::onHangup));
  // mCallChannel->setAcceptCallback(Tp::memFun(this,&SipCallChannel::onAccept));

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
}

void RingCallChannel::onHangup(uint reason, const QString &detailedReason, const QString &message, Tp::DBusError* error)
{
  qDebug() << Q_FUNC_INFO;
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
