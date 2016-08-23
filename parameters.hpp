/*****************************************************************************
  Header file for parameters.cpp
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

#ifndef BELL_PARAMETERS_H
#define BELL_PARAMETERS_H

#include <TelepathyQt/ProtocolParameterList>
#include <QVariant>
#include <QMap>
#include <iterator>
#include <QList>

#include "ringdaemon.hpp"

typedef QMap<QString, QString> MapStringString;
Q_DECLARE_METATYPE(MapStringString)

namespace Bell{

class Parameters{
public:
// Store the parameters from the account settings panel.
static Tp::ProtocolParameterList getParameterList();
Parameters(const QVariantMap& parameters);
const QVariantMap value();
void setAccountIDviaRingID();
void setRingIDandAccountID();
void updateParameters(QString identifyAccount);

private:
  void createNewRingAccount();
  void setRingIDviaAccountID();
private:
  const QVariantMap mParameters;
  QString mUsername;
  QString mAccount;
  QString mRingID;
  QString mAccountID;
  QString mHostname;
  QDBusInterface mConfigurationManagerInterface;
};
}

#endif
