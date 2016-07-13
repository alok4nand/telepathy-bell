#include <TelepathyQt/BaseConnection>
#include <TelepathyQt/Constants>
#include <TelepathyQt/RequestableChannelClassSpec>
#include <TelepathyQt/RequestableChannelClassSpecList>
#include <TelepathyQt/Types>
#include <TelepathyQt/ProtocolParameterList>
#include <TelepathyQt/Utils>

#include "protocol.hpp"
#include "connection.hpp"
#include "common.hpp"
#include "parameters.hpp"

using namespace Bell;

Protocol::Protocol(const QDBusConnection &dbusConnection, const QString &name)
: Tp::BaseProtocol(dbusConnection, name)
{
  setParameters(Tp::ProtocolParameterList()
   << Tp::ProtocolParameter(QLatin1String("Username"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired)
   << Tp::ProtocolParameter(QLatin1String("RingID"), QDBusSignature(QLatin1String("s")), Tp::ConnMgrParamFlagRequired | Tp::ConnMgrParamFlagHasDefault, "Enter \"ring:\" to create a new Ring account")
   << Tp::ProtocolParameter(QLatin1String("AccountID"), QDBusSignature(QLatin1String("s")),0)
   << Tp::ProtocolParameter(QLatin1String("Hostname"), QDBusSignature(QLatin1String("s")),Tp::ConnMgrParamFlagHasDefault, "bootstrap.ring.cx")
 );

  setRequestableChannelClasses(Tp::RequestableChannelClassSpecList()
  << Tp::RequestableChannelClassSpec::textChat()
  << Tp::RequestableChannelClassSpec::audioCall()
  << Tp::RequestableChannelClassSpec::videoCall());

  setEnglishName(QLatin1String("Ring"));
  setIconName(QLatin1String("im-ring"));
  setVCardField(QLatin1String("x-ring"));

  setCreateConnectionCallback(Tp::memFun(this, &Protocol::createConnection));
  setIdentifyAccountCallback(Tp::memFun(this, &Protocol::identifyAccount));
  setNormalizeContactCallback(Tp::memFun(this, &Protocol::normalizeContact));

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
   Tp::BaseConnectionPtr newConnection = Tp::BaseConnection::create<Bell::Connection>(QLatin1String("bell"), name(), parameters);
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
