#include <QCoreApplication>

#include <TelepathyQt/BaseConnectionManager>
#include <TelepathyQt/Constants>
#include <TelepathyQt/Debug>

#include "protocol.hpp"

using namespace Bell;
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QLatin1String("telepathy-bell"));

    Tp::registerTypes();
    Tp::enableDebug(true);
    Tp::enableWarnings(true);

    Tp::BaseProtocolPtr proto = Tp::BaseProtocol::create<Protocol>(QLatin1String("Ring"));
    Tp::BaseConnectionManagerPtr cm = Tp::BaseConnectionManager::create(QLatin1String("bell"));

    cm->addProtocol(proto);
    cm->registerObject();

    return app.exec();
}
