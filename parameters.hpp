#ifndef BELL_PARAMETERS_H
#define BELL_PARAMETERS_H

#include <TelepathyQt/ProtocolParameterList>
#include <QVariant>
// insert ring headers.
namespace Bell{

class Parameters{
public:
// Store the parameters from the account settings panel.
Parameters(const QVariantMap& parameters);
// Get the list of parameters that should appear in the account settings panel.
static Tp::ProtocolParameterList getParameterList();

private:
  const QVariantMap mParameters;
  QString mUsername;
  QString mAccount;
  QString mRingID;
  QString mAccountID;
  QString mHostname;
};
}

#endif
