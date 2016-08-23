/*****************************************************************************
  Deals with all the provided and needed parameters.
  If the user has an existing account with RingID,
  it fills in the user's AccountID.
  The user can also create a new account by entering "ring:"
  This class deals with all the above parameter functions and updating them.
******************************************************************************
*  Copyright (C) 2016 Alok Anand
*
*  Author: Alok Anand <alok4nand@gmail.com>
*
*  Telepathy-bell is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.
*
*  Telepathy-bell is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
*/

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
  << Tp::ProtocolParameter(QLatin1String("account"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
  << Tp::ProtocolParameter(QLatin1String("RingID"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired | Tp::ConnMgrParamFlagHasDefault, "Enter \"ring:\" to create a new Ring account")
  << Tp::ProtocolParameter(QLatin1String("AccountID"), QDBusSignature(QLatin1String("s")),0)
  << Tp::ProtocolParameter(QLatin1String("Hostname"), QDBusSignature(QLatin1String("s")),Tp::ConnMgrParamFlagHasDefault, "bootstrap.ring.cx");
   return params;
}

Parameters::Parameters(const QVariantMap& parameters): mParameters(parameters),
mConfigurationManagerInterface("cx.ring.Ring","/cx/ring/Ring/ConfigurationManager","cx.ring.Ring.ConfigurationManager")
{
  qDebug() << Q_FUNC_INFO;
  mUsername = parameters[QLatin1String("Username")].toString();
  mAccount = parameters[QLatin1String("account")].toString();
  mRingID = parameters[QLatin1String("RingID")].toString();
  mAccountID = parameters[QLatin1String("AccountID")].toString();
  mHostname = parameters[QLatin1String("Hostname")].toString();
}

void Parameters::setRingIDandAccountID()
{
  qDebug() << Q_FUNC_INFO;
  Parameters::createNewRingAccount();
  Parameters::setRingIDviaAccountID();
}

void Parameters::createNewRingAccount()
{

  qDBusRegisterMetaType<MapStringString>();
  QDBusReply<MapStringString> reply = mConfigurationManagerInterface.call("getAccountTemplate","RING");
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

 QDBusPendingCall mPendingCallReply = mConfigurationManagerInterface.asyncCallWithArgumentList("addAccount",argumentList);
 QDBusPendingReply<QString> mPendingReply(mPendingCallReply);
 if(mPendingReply.isError()){
   qDebug() << "Error in getting new Account ID ";
 }
 mAccountID = mPendingReply.value();
}

void Parameters::setRingIDviaAccountID()
{
  qDebug() << Q_FUNC_INFO;
  qDBusRegisterMetaType<MapStringString>();
  QDBusReply<MapStringString> reply = mConfigurationManagerInterface.call("getAccountDetails",mAccountID);
  if(!reply.isValid())
  {
    qDebug() << "Error in getting Account details";
    QDBusError mError = reply.error();
    qDebug() << mError.message();
    qDebug() << mError.name();
  }
  MapStringString mAccountDetails = reply.value();
  mRingID = mAccountDetails["Account.username"];
  qDebug() << "Value of RingID is : " << mRingID;
}

void Parameters::setAccountIDviaRingID()
{
  qDebug() << Q_FUNC_INFO;
  QDBusReply<QStringList> reply = mConfigurationManagerInterface.call("getAccountList");
  if(!reply.isValid())
  {
    qDebug() << "Error in getting Account List";
    // QDBusError mError = reply.error();
    // qDebug() << mError.message();
    // qDebug() << mError.name();
  }
  QString str;
  qDBusRegisterMetaType<MapStringString>();
  foreach(str,reply.value())
  {
  QDBusReply<MapStringString> mAccountDetails = mConfigurationManagerInterface.call("getAccountDetails",str);
  if(!mAccountDetails.isValid())
    {
    qDebug() << "Error in getting Account details";
    // QDBusError mError = reply.error();
    // qDebug() << mError.message();
    // qDebug() << mError.name();
    }
    MapStringString details = mAccountDetails.value();
    if(mUsername == details["Account.alias"] && mRingID == details["Account.username"]){
      mAccountID = str;
      qDebug() << mAccountID;
    }
  }
}

const QVariantMap Parameters::value()
{
  QVariantMap modifiedParameters;
  modifiedParameters[QLatin1String("Username")] = mUsername;
  modifiedParameters[QLatin1String("account")] = mAccount;
  modifiedParameters[QLatin1String("RingID")] = mRingID;
  modifiedParameters[QLatin1String("AccountID")] = mAccountID;
  modifiedParameters[QLatin1String("Hostname")] = mHostname;
  // for(QVariantMap::const_iterator iter = modifiedParameters.begin(); iter != modifiedParameters.end(); ++iter) {
  //      qDebug() << iter.key() << iter.value();
  //     }
  return modifiedParameters;
}

void Parameters::updateParameters(QString identifyAccount){
  qDebug() << Q_FUNC_INFO << identifyAccount;
  // FIXME better account path recognition.
  QString objPath = "/org/freedesktop/Telepathy/Account/bell/Ring/" + identifyAccount + "0";
  qDebug() << objPath;
  QDBusInterface mAccountManager("org.freedesktop.Telepathy.AccountManager",objPath,"org.freedesktop.Telepathy.Account");
  QVariantMap set;
  set[QLatin1String("RingID")] = mRingID;
  set[QLatin1String("AccountID")] = mAccountID;
  QStringList unset;
  QDBusReply<QStringList> reconnect_required = mAccountManager.call("UpdateParameters",set,unset);
  if(!reconnect_required.isValid())
  {
    qDebug() << "Error in Updating Account Parameters";
    QDBusError mError = reconnect_required.error();
    qDebug() << mError.message();
    qDebug() << mError.name();
  }
  else{
    qDebug() << "Updates Account Parameters Sucessfully , reconnect required ?:";
  }
}
