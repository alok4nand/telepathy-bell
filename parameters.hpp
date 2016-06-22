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
const std::string getAlias(){ return mAlias; }
const std::string getHostname(){ return mHostname;}

private:
  const QVariantMap mParameters;
  std::string mAlias;
  std::string mHostname;
};
}



#endif
