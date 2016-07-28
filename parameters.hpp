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
static Tp::ProtocolParameterList getParameterList();
Parameters(const QVariantMap& parameters);
const QVariantMap value();
void setAccountIDviaRingID();
void setRingIDandAccountID();
void updateParameters(QString identifyAccount);

private:
  void createNewRingAccount();
  void setRingIDviaAccountID();
private:
  const QVariantMap mParameters;
  QString mUsername;
  QString mAccount;
  QString mRingID;
  QString mAccountID;
  QString mHostname;
  QDBusInterface mConfigurationManagerInterface;
};
}

#endif
