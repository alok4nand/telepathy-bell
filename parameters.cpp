#include "parameters.hpp"

using namespace Bell;

Tp::ProtocolParameterList Parameters::getParameterList()
{
   Tp::ProtocolParameterList params;

   params << Tp::ProtocolParameter(QLatin1String("Username"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
  << Tp::ProtocolParameter(QLatin1String("Account"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
  << Tp::ProtocolParameter(QLatin1String("RingID"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired | Tp::ConnMgrParamFlagHasDefault, "Enter \"ring:\" to create a new Ring account")
  << Tp::ProtocolParameter(QLatin1String("AccountID"), QDBusSignature(QLatin1String("s")),0)
  << Tp::ProtocolParameter(QLatin1String("Hostname"), QDBusSignature(QLatin1String("s")),Tp::ConnMgrParamFlagHasDefault, "bootstrap.ring.cx");
   return params;
}

using namespace Bell;

Parameters::Parameters(const QVariantMap& parameters)
: mParameters(parameters)
{
  mUsername = getString("Username");
  mUsername = getString("Account");
  mUsername = getString("RingID");
  mUsername = getString("AccountID");
  mHostname = getString("Hostname");
}


const char*
Parameters::getString(const char* parameter)
{
   return (const char *)mParameters.value(QLatin1String(parameter)).toString().toUtf8().constData();
}
