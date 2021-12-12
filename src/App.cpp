/**
 * @file App.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Class for loading project view initialization
 */
#include <App.h>
#include <QStyleFactory>
#include <QIcon>
#include <QPalette>

 /**
  * @brief Construct a new App:: App object
  *
  * @param argc argument count
  * @param argv argument list
  */
App::App(int& xargc, char** xargv)
    :app(xargc, xargv)
{
    // Basic setup of the application
    QCoreApplication::setApplicationName(AppName.data());
    QCoreApplication::setApplicationVersion(AppVer.data());
    app.setStyle(QStyleFactory::create(AppTheme.data()));
    app.setWindowIcon(QIcon{ ":/tlr.ico" });

    // Style definition (darkmode)
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);

    // Event type for opening projects
    wnd.emplace(1280,1024);
    wnd->show();
}

/**
 * @brief Starts the application
 *
 * @return int return code of the application
 */
int App::Start()
{
    return app.exec();
}

