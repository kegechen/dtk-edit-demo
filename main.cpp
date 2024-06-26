/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <DApplication>
#include <LogManager.h>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSettings>
#include <DGuiApplicationHelper>

#include "mainwindow.h"

DCORE_USE_NAMESPACE
DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    const QString organizationName("deepin") ;
    const QString applicationName("dtk-edit-demo");
    QSettings qsettings(organizationName, applicationName);
    bool colorConfig = qsettings.value("Base.AppSettings.UseInactiveColorGroup/value", true).toBool();
    bool useInactiveColorGroup = DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::UseInactiveColorGroup);
    if (colorConfig ^ useInactiveColorGroup) {
        DGuiApplicationHelper::setAttribute(DGuiApplicationHelper::UseInactiveColorGroup, colorConfig);
    }

    bool saveConfig = qsettings.value("Base.AppSettings.DontSaveApplicationTheme/value", false).toBool();
    bool dontSaveApplicationTheme = DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::DontSaveApplicationTheme);
    if (saveConfig ^ dontSaveApplicationTheme) {
        DGuiApplicationHelper::setAttribute(DGuiApplicationHelper::DontSaveApplicationTheme, saveConfig);
    }

    DApplication app(argc, argv);
    app.setOrganizationName(organizationName);
    app.setApplicationName(applicationName);

    // aboutdialog info
    app.setApplicationVersion(DTK_VERSION_STR);
    app.setApplicationDescription("this is a dtk edit demo");
    app.setProductIcon(QIcon::fromTheme("deepin-editor"));
    // icon on dock
    app.setWindowIcon(QIcon::fromTheme("deepin-editor"));

    // Single instance
    bool enableSingleApp = qsettings.value("Base.AppSettings.SingleApp/value", true).toBool();
    if (enableSingleApp && !app.setSingleInstance("dtk-edit-demo-key")) {
        qWarning() << "app is already running！";
        return -1;
    }

    // Console log
    DLogManager::registerConsoleAppender();
    // File log
    DLogManager::registerFileAppender();
    qDebug() << "log file path:" << DLogManager::getlogFilePath();

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    if (!parser.positionalArguments().isEmpty())
        mainWin.loadFile(parser.positionalArguments().first());
    mainWin.show();
    return app.exec();
}
//! [0]
