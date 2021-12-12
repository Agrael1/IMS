#include "Cell.h"

std::function<void()> Cell::OnTreePlant;
std::function<void(Cell::Reason)> Cell::OnTreeDies;