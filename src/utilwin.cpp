#include "util.h"
#include <time.h>
int64_t mtimer() {
	return clock() * 1000 / CLOCKS_PER_SEC;
}
