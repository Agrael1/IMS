#include <MainWindow.h>
#include <qmenubar.h>

MainWindow::MainWindow(int32_t width, int32_t height)
	:start("start")
{
	resize(width, height);
	core.emplace();

	menuBar()->addAction(&start);
	connect(&start, &QAction::triggered, [this]() { OnStart(); });

	setCentralWidget(&core->view);
}

void MainWindow::OnStart()
{
	timerId = startTimer(1);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if(timerId)
		killTimer(timerId);
	core.reset();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
	if (runs)
	{
		runs = core->scene.StartFor(100);
	}
	if (!runs)
	{
		core->scene.SendStats();
		killTimer(timerId);
		timerId = 0;
	}
}
