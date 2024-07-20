cmake_minimum_required(VERSION 3.16)
project(CatPaw VERSION 1.0.0)

set(QT_MIN_VERSION "6.6")
set(KF_MIN_VERSION "6.4")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEGitCommitHooks)
include(KDEClangFormat)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
        Core
        Gui
        DBus
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
        Config
        ConfigWidgets
        CoreAddons
        WindowSystem
        I18n
        KCMUtils
        XmlGui
)

find_package(KWin REQUIRED)
find_package(KWinDBusInterface CONFIG REQUIRED)

add_subdirectory(src)

feature_summary(WHAT ALL)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)