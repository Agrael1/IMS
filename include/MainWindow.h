#pragma once
#include <QMainWindow>
#include <Scene.h>
#include <View.h>
#include <optional>
#include <QMenu>

class MainWindow : public QMainWindow
{
	struct Core
	{
		Core():view(&scene){}
		FlowScene scene;
		FlowView view;
	};
public:
	MainWindow(int32_t width, int32_t height);
public:
	void OnStart();
	void closeEvent(QCloseEvent* event)override;
	void timerEvent(QTimerEvent* event);
private:
	QAction start;
	std::optional<Core> core;
	int timerId = 0;
	bool runs = true;
};