/*****************************************************************************
   This clss was written for managing ring daemon.
   Might be useful in future.
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
