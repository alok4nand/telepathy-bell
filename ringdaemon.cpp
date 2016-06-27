#include "ringdaemon.hpp"

using namespace Bell;

RingDaemon::RingDaemon()
{
  qDebug() << Q_FUNC_INFO;
  RingDaemon::init();
if(RingDaemon::getProcessID() == 0 ){
   mRingProcessID = RingDaemon::activateService();
}
else{
  mRingProcessID = RingDaemon::getProcessID();
}
}

RingDaemon::~RingDaemon()
{
RingDaemon::deactivateService();
}

void RingDaemon::init()
{
  qDebug() << Q_FUNC_INFO;
  // Initialize Interface to DBus
  mDBusInterface = new QDBusInterface("org.freedesktop.DBus","/org/freedesktop/DBus","org.freedesktop.DBus");
  // Initialize Ring Daemon's Configuration Manager Interface
  mConfigurationManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring","cx.ring.Ring.ConfigurationManager");
  // Initialize Ring Daemon's Call Manager Interface
  mCallManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring","cx.ring.Ring.CallManager");
  // Initialize Ring Daemon's Instance Interface
  mInstanceInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring","cx.ring.Ring.Instance");
}

uint RingDaemon::getProcessID(){
  qDebug() << Q_FUNC_INFO;
  QDBusReply<uint> reply = mDBusInterface->call("GetConnectionUnixProcessID","cx.ring.Ring");
  if(reply.isValid()){
      qDebug() << reply;
      return reply;
      }
  return 0;
}

uint RingDaemon::activateService()
{
  qDebug() << Q_FUNC_INFO;
  uint arg = 0;
  mDBusInterface->call("StartServiceByName","cx.ring.Ring",arg);
  return RingDaemon::getProcessID();
}

void RingDaemon::deactivateService()
{
  qDebug() << Q_FUNC_INFO;
  mInstanceInterface->call("Unregister",mRingProcessID);
}
