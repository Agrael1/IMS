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
	killTimer(timerId);
	core.reset();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
	core->scene.Start();
}
