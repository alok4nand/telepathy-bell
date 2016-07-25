#ifndef BELL_PARAMETERS_H
#define BELL_PARAMETERS_H

#include <TelepathyQt/ProtocolParameterList>
#include <QVariant>
#include <QMap>
#include <iterator>
#include <QList>

#include "ringdaemon.hpp"

typedef QMap<QString, QString> MapStringString;
Q_DECLARE_METATYPE(MapStringString)

namespace Bell{

class Parameters{
public:
// Store the parameters from the account settings panel.
Parameters(const QVariantMap& parameters);
void createNewRingAccount();
// Get the list of parameters that should appear in the account settings panel.
static Tp::ProtocolParameterList getParameterList();
QVariantMap value();
private:
  RingDaemon mRingDaemonInterface;
  const QVariantMap mParameters;
  QString mUsername;
  QString mAccount;
  QString mRingID;
  QString mAccountID;
  QString mHostname;
};
}

#endif
