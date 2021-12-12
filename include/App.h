/**
 * @file App.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Class for loading project view initialization
 */
#pragma once
#include <QApplication>
#include <MainWindow.h>
#include <optional>

class App : public QObject
{
public:
	App(int& xargc, char** xargv);
public:
	int Start();
private:
	QApplication app;
	std::optional<MainWindow> wnd;
private:
	static constexpr std::string_view AppVer = "0.2.1";
	static constexpr std::string_view AppTheme = "Fusion";
	static constexpr std::string_view AppName = "CATrees";
};