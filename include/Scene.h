#pragma once
#include <QGraphicsScene>
#include <thread>

class Core;
class QThread;

class FlowScene : public QGraphicsScene
{
public:
	FlowScene();
	~FlowScene();
public:
	void Start();
private:
	std::unique_ptr<Core> c;
	std::jthread thread;
	std::atomic_bool launch = false;
};