#include "parameters.hpp"

using namespace Bell;

Tp::ProtocolParameterList Parameters::getParameterList()
{
  qDebug() << Q_FUNC_INFO;
   Tp::ProtocolParameterList params;

   params << Tp::ProtocolParameter(QLatin1String("Username"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
  << Tp::ProtocolParameter(QLatin1String("Account"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
  << Tp::ProtocolParameter(QLatin1String("RingID"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired | Tp::ConnMgrParamFlagHasDefault, "Enter \"ring:\" to create a new Ring account")
  << Tp::ProtocolParameter(QLatin1String("AccountID"), QDBusSignature(QLatin1String("s")),0)
  << Tp::ProtocolParameter(QLatin1String("Hostname"), QDBusSignature(QLatin1String("s")),Tp::ConnMgrParamFlagHasDefault, "bootstrap.ring.cx");
   return params;
}

Parameters::Parameters(const QVariantMap& parameters): mParameters(parameters)
{
  qDebug() << Q_FUNC_INFO;
  mUsername = parameters[QLatin1String("Username")].toString();
  mAccount = parameters[QLatin1String("Account")].toString();
  mRingID = parameters[QLatin1String("RingID")].toString();
  mAccountID = parameters[QLatin1String("AccountID")].toString();
  mHostname = parameters[QLatin1String("Hostname")].toString();
}
