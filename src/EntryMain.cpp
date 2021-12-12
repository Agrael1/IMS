#include "Grid.h"

int main()
{
	Grid g(map);
	g.Write();
	while(g.Update()) ;
	return 0;
}