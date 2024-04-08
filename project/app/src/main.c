#include "main.h"
int main(void)
{
	int size;
	char *calling = "https://api.translink.ca/rttiapi/v1/stops/55713/estimates?apikey=JoKWW8MHpsoc04lKVKnA&count=2";
	transStruct *a = ReadFromTransAPI(&size, calling);
	freeTransStruct(size, a);
	return 1;
}