#pragma once
#include <QGraphicsScene>

class Core;

class FlowScene : public QGraphicsScene
{
public:
	FlowScene();
	~FlowScene();
public:
	void Start();
	bool StartFor(size_t time);
	void SendStats()const;
	void Refresh();
private:
	std::unique_ptr<Core> c;
};