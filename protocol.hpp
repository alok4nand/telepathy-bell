#ifndef BELL_PROTOCOL_H
#define BELL_PROTOCOL_H

#include <TelepathyQt/BaseProtocol>

namespace Bell{

class Protocol : public Tp::BaseProtocol
{
    Q_OBJECT

public:
    Protocol(const QDBusConnection &dbusConnection, const QString &name);
   ~Protocol();

private:
    Tp::BaseConnectionPtr createConnection(const QVariantMap &parameters, Tp::DBusError *error);
    QString identifyAccount(const QVariantMap &parameters, Tp::DBusError *error);
    QString normalizeContact(const QString &contactId, Tp::DBusError *error);

    // Proto.I.Addressing
    QString normalizeVCardAddress(const QString &vCardField, const QString vCardAddress,
            Tp::DBusError *error);
    QString normalizeContactUri(const QString &uri, Tp::DBusError *error);

private:
    Tp::BaseProtocolAddressingInterfacePtr mAddressingInterface;
};

}
#endif
