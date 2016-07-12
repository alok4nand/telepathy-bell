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
