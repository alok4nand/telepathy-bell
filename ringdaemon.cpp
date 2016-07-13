#include "ringdaemon.hpp"

using namespace Bell;

RingDaemon::RingDaemon()
{
  qDebug() << Q_FUNC_INFO;
  mDBusInterface = new QDBusInterface("org.freedesktop.DBus","/org/freedesktop/DBus","org.freedesktop.DBus");
  if(RingDaemon::getProcessID() == 0 ){
    mRingProcessID = RingDaemon::activateService();
  }
  else{
    mRingProcessID = RingDaemon::getProcessID();
  }
  RingDaemon::initializeInterfaces();
}

RingDaemon::~RingDaemon()
{
RingDaemon::deactivateService();
delete(mDBusInterface);
delete(mConfigurationManagerInterface);
delete(mCallManagerInterface);
delete(mInstanceInterface);
delete(mPresenceManagerInterface);
delete(mVideoManagerInterface);
}

void RingDaemon::initializeInterfaces()
{
  qDebug() << Q_FUNC_INFO;
  // Initialize Ring Daemon's Configuration Manager Interface
  mConfigurationManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager");
  // Initialize Ring Daemon's Call Manager Interface
  mCallManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring/CallManager","cx.ring.Ring.CallManager");
  // Initialize Ring Daemon's Instance Interface
  mInstanceInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring/Instance","cx.ring.Ring.Instance");
  // Initialize Ring Daemon's Presence Manager Interface
  mPresenceManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring/PresenceManager","cx.ring.Ring.PresenceManager");
  // Initialize Ring Daemon's Video Manager Interface
  mVideoManagerInterface = new QDBusInterface("cx.ring.Ring","/cx/ring/Ring/VideoManager","cx.ring.Ring.VideoManager");
}

int RingDaemon::getProcessID(){
  qDebug() << Q_FUNC_INFO;
  QDBusReply<uint> reply = mDBusInterface->call("GetConnectionUnixProcessID","cx.ring.Ring");
  if(reply.isValid()){
      qDebug() << reply;
      return reply;
      }
  return 0;
}

int RingDaemon::activateService()
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
