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
  private:
    void init();
    uint getProcessID();
    uint activateService();
    void deactivateService();
  private:
    uint mRingProcessID = 0;
    QDBusInterface *mDBusInterface;
    QDBusInterface *mConfigurationManagerInterface;
    QDBusInterface *mCallManagerInterface;
    QDBusInterface *mInstanceInterface;
 };
}

#endif
