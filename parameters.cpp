#include "parameters.hpp"
#include <QDBusMetaType>
#include <QMap>
#include <iterator>
#include <QDBusPendingCall>
#include <QDBusPendingReply>

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

Parameters::Parameters(const QVariantMap& parameters): mParameters(parameters)
{
  qDebug() << Q_FUNC_INFO;
  mUsername = parameters[QLatin1String("Username")].toString();
  mAccount = parameters[QLatin1String("Account")].toString();
  mRingID = parameters[QLatin1String("RingID")].toString();
  mAccountID = parameters[QLatin1String("AccountID")].toString();
  mHostname = parameters[QLatin1String("Hostname")].toString();
}

void Parameters::createNewRingAccount()
{
  qDebug() << Q_FUNC_INFO;

  qDBusRegisterMetaType<MapStringString>();
  QDBusReply<MapStringString> reply = mRingDaemonInterface.ConfigurationManagerInterface()->call("getAccountTemplate","RING");
  if(!reply.isValid())
  {
  qDebug() << "Error in getting Account Template";
  // QDBusError mError = reply.error();
  // qDebug() << mError.message();
  // qDebug() << mError.name();
  }
  MapStringString mAccountDetails = reply.value();
  mAccountDetails["Account.alias"]  = mUsername;
  mAccountDetails["Account.displayName"] = mAccount;
  mAccountDetails["Account.enable"] = "false";
  mAccountDetails["Account.hasCustomUserAgent"] = "telepathy-bell";
  mAccountDetails["Account.hostname"] = mHostname;
  // QMap<QString,QString>::iterator iter;
  // for(iter = mAccountDetails.begin(); iter != mAccountDetails.end(); ++iter)
  // {
  //       qDebug() << iter.key() << iter.value();
  // }
  QList<QVariant> argumentList;
  argumentList << QVariant::fromValue(mAccountDetails);

 QDBusPendingCall mPendingCallReply = mRingDaemonInterface.ConfigurationManagerInterface()->asyncCallWithArgumentList("addAccount",argumentList);
 QDBusPendingReply<QString> mPendingReply(mPendingCallReply);
 if(mPendingReply.isError()){
   qDebug() << "Error in getting new Account ID ";
 }
 mAccountID = mPendingReply.value();
}

QVariantMap Parameters::value()
{
  QVariantMap modifiedParameters;
  modifiedParameters[QLatin1String("Username")] = mUsername;
  modifiedParameters[QLatin1String("Account")] = mAccount;
  modifiedParameters[QLatin1String("RingID")] = mRingID;
  modifiedParameters[QLatin1String("AccountID")] = mAccountID;
  modifiedParameters[QLatin1String("Hostname")] = mHostname;
  // for(QVariantMap::const_iterator iter = modifiedParameters.begin(); iter != modifiedParameters.end(); ++iter) {
  //      qDebug() << iter.key() << iter.value();
  //     }
  return modifiedParameters;
}
