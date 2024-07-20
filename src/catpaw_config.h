/*
    SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <kcmodule.h>

#include "ui_catpaw_config.h"

class CatPawEffectConfig : public KCModule
{
    Q_OBJECT
public:
    explicit CatPawEffectConfig(QObject *parent, const KPluginMetaData &data);

    void save() override;

private:
    Ui::CatPawEffectConfigForm m_ui;
};
