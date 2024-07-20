#######################################
# Effect

set(catpaw_SOURCES
    main.cpp
    catpaw.cpp
)

kconfig_add_kcfg_files(catpaw_SOURCES
    catpawconfig.kcfgc
)

kcoreaddons_add_plugin(catpaw SOURCES ${catpaw_SOURCES} INSTALL_NAMESPACE "kwin/effects/plugins")
target_compile_definitions(catpaw PRIVATE -DTRANSLATION_DOMAIN=\"catpaw\")

target_link_libraries(catpaw PRIVATE
    KWin::kwin
    KF6::ConfigGui
    KF6::I18n
)

##########################
## configurtion dialog
##########################
qt_add_dbus_interface(dbus_SRCS ${KWIN_EFFECTS_INTERFACE} Effects_interface)

set(catpaw_config_SRCS catpaw_config.cpp ${dbus_SRCS})
ki18n_wrap_ui(catpaw_config_SRCS catpaw_config.ui)
kconfig_add_kcfg_files(catpaw_config_SRCS catpawconfig.kcfgc)

kcoreaddons_add_plugin(catpaw_config INSTALL_NAMESPACE "kwin/effects/configs" SOURCES ${catpaw_config_SRCS})
target_compile_definitions(catpaw_config PRIVATE -DTRANSLATION_DOMAIN=\"catpaw\")

target_link_libraries(catpaw_config
    Qt::DBus
    KF6::KCMUtils
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
)