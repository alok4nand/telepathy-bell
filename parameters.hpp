#ifndef BELL_PARAMETERS_H
#define BELL_PARAMETERS_H

#include <QVariant>
#include <TelepathyQt/ProtocolParameterList>

#include <string>
// insert ring headers.
namespace Bell{

class Parameters{
public:
// Store the parameters from the account settings panel.
Parameters(const QVariantMap& parameters);
// Get the list of parameters that should appear in the account settings panel.
static Tp::ProtocolParameterList getParameterList();

protected:
const char* getString(const char* parameter);
const std::string getUsername(){ return mUsername; }
const std::string getAccount() {return mAccount;}
const std::string getRingID() {return mRingID;}
const std::string getAccountID() {return mAccountID;}
const std::string getHostname(){ return mHostname;}

private:
  const QVariantMap mParameters;
  std::string mUsername;
  std::string mAccount;
  std::string mRingID;
  std::string mAccountID;
  std::string mHostname;
};
}



#endif
