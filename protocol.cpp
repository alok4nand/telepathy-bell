/*****************************************************************************
  Creates protocol object for which Connection Manager can create
  Connection.
  It is also manages the parameters passed on to Connection
  object for creating a connection.
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

#include <TelepathyQt/BaseConnection>
#include <TelepathyQt/Constants>
#include <TelepathyQt/RequestableChannelClassSpec>
#include <TelepathyQt/RequestableChannelClassSpecList>
#include <TelepathyQt/Types>
#include <TelepathyQt/ProtocolParameterList>
#include <TelepathyQt/Utils>
#include <QtDebug>

#include "protocol.hpp"
#include "connection.hpp"
#include "common.hpp"
#include "parameters.hpp"

using namespace Bell;

Protocol::Protocol(const QDBusConnection &dbusConnection, const QString &name)
: Tp::BaseProtocol(dbusConnection, name)
{
  qDebug() << Q_FUNC_INFO;

  setRequestableChannelClasses(Tp::RequestableChannelClassSpecList()
  << Tp::RequestableChannelClassSpec::textChat()
  << Tp::RequestableChannelClassSpec::audioCall()
  << Tp::RequestableChannelClassSpec::videoCall());

  setEnglishName(QLatin1String("Ring"));
  setIconName(QLatin1String("im-Ring"));
  setVCardField(QLatin1String("x-ring"));

  setCreateConnectionCallback(Tp::memFun(this, &Protocol::createConnection));
  setIdentifyAccountCallback(Tp::memFun(this, &Protocol::identifyAccount));
  setNormalizeContactCallback(Tp::memFun(this, &Protocol::normalizeContact));
  setParameters(Parameters::getParameterList());

  mAddressingInterface = Tp::BaseProtocolAddressingInterface::create();
  mAddressingInterface->setAddressableVCardFields(QStringList() << QLatin1String("x-ring"));
  mAddressingInterface->setAddressableUriSchemes(QStringList() << QLatin1String("ring"));
  mAddressingInterface->setNormalizeVCardAddressCallback(Tp::memFun(this, &Protocol::normalizeVCardAddress));
  mAddressingInterface->setNormalizeContactUriCallback(Tp::memFun(this, &Protocol::normalizeContactUri));
  plugInterface(Tp::AbstractProtocolInterfacePtr::dynamicCast(mAddressingInterface));

}

Protocol::~Protocol()
{

}

Tp::BaseConnectionPtr Protocol::createConnection(const QVariantMap &parameters, Tp::DBusError *error)
{
  qDebug() << Q_FUNC_INFO << parameters;
  Q_UNUSED(error);
  QDBusReply<void> reply = QDBusConnection::sessionBus().interface()->startService("cx.ring.Ring");
  if(!reply.isValid()){
    qDebug() << "Error in starting Ring Daemon Service";
    // QDBusError mError = reply.error();
    // qDebug() << mError.message();
    // qDebug() << mError.name();
  }
  Parameters mParameters(parameters);
  QString iName = Protocol::identifyAccount(parameters,error);
   if (parameters[QLatin1String("RingID")].toString() == "ring:")
   {
      qDebug() << "Creating a New Ring Account";
      mParameters.setRingIDandAccountID();
      mParameters.updateParameters(iName);
   }
   if (!parameters.contains("AccountID"))
   {
      qDebug() << "Adding account ID information";
      mParameters.setAccountIDviaRingID();
      mParameters.updateParameters(iName);
   }

  Tp::BaseConnectionPtr newConnection = Tp::BaseConnection::create<Bell::Connection>(QLatin1String("bell"), name(), mParameters.value());
  return newConnection;
}

QString Protocol::identifyAccount(const QVariantMap &parameters, Tp::DBusError *error)
{
   return Tp::escapeAsIdentifier(parameters[QLatin1String("account")].toString());
}

QString Protocol::normalizeContact(const QString &contactId, Tp::DBusError *error)
{
   // FIXME
   return contactId;
}

QString Protocol::normalizeVCardAddress(const QString &vcardField, const QString vcardAddress,
        Tp::DBusError *error)
{
   // FIXME
   error->set(QLatin1String("NormalizeVCardAddress.Error.Test"), QLatin1String(""));
   return QString();
}

QString Protocol::normalizeContactUri(const QString &uri, Tp::DBusError *error)
{
   // FIXME
   error->set(QLatin1String("NormalizeContactUri.Error.Test"), QLatin1String(""));
   return QString();
}
