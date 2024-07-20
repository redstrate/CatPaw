/*
    SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "catpaw_config.h"

#include "catpawconfig.h"

#include "Effects_interface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS(CatPawEffectConfig)

CatPawEffectConfig::CatPawEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    m_ui.setupUi(widget());

    addConfig(CatPawConfig::self(), widget());
}

void CatPawEffectConfig::save()
{
    KCModule::save();
    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"), QStringLiteral("/Effects"), QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("catpaw"));
}

#include "catpaw_config.moc"

#include "moc_catpaw_config.cpp"
