#include "parameters.hpp"

Tp::ProtocolParameterList Parameters::getParameterList()
{
   Tp::ProtocolParameterList params;

   params << Tp::ProtocolParameter(QLatin1String("Alias"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired
    << Tp::ProtocolParameter(QLatin1String("Hostname"), QDBusSignature(QLatin1String("s")), 0, "bootstrap.ring.cx")
   ;

   return params;
}

using namespace Bell;

Parameters::Parameters(const QVariantMap& parameters)
: mParameters(parameters)
{
  mAlias = getString("Alias");
  mHostname = getString("Hostname");
}


const char*
Parameters::getString(const char* parameter)
{
   return (const char *)mParameters.value(QLatin1String(parameter)).toString().toUtf8().constData();
}
