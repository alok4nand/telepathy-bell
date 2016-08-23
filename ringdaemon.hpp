/*****************************************************************************
  Header file for ringdaemon.cpp
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

#ifndef BELL_RINGDAEMON_H
#define BELL_RINGDAEMON_H

#include <QtDBus/QtDBus>

namespace Bell{
  class RingDaemon {
  public:
    RingDaemon();
    ~RingDaemon();
    QDBusInterface* DBusInterface(){return mDBusInterface;};
    QDBusInterface* ConfigurationManagerInterface(){return mConfigurationManagerInterface;};
    QDBusInterface* CallManagerInterface(){return mCallManagerInterface;};
    QDBusInterface* InstanceInterface(){return mInstanceInterface;};
    QDBusInterface* PresenceManagerInterface(){return mPresenceManagerInterface;};
    QDBusInterface* VideoManagerInterface(){return mVideoManagerInterface;};
  private:
    void initializeInterfaces();
    int getProcessID();
    int activateService();
    void deactivateService();
  private:
    int mRingProcessID = 0;
    QDBusInterface *mDBusInterface;
    QDBusInterface *mConfigurationManagerInterface;
    QDBusInterface *mCallManagerInterface;
    QDBusInterface *mInstanceInterface;
    QDBusInterface *mPresenceManagerInterface;
    QDBusInterface *mVideoManagerInterface;
 };
}

#endif
