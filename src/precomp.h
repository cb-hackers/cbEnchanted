#ifndef PRECOMP_H
#define PRECOMP_H

#ifdef WIN32
#include <Windows.h>
#endif
#include <map>
#include <stdint.h>
#include <vector>
#include <cassert>
#include <set>
#include <cstdio>
#include <fstream>
#include <stack>
#include <list>
#include <cstdlib>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/dynamic_bitset.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/transformations.h>

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795028841971693993751
#endif

#define UNUSED(v) (v);

using namespace std;

using boost::function;
using boost::lexical_cast;
using boost::dynamic_bitset;

#ifdef FORCEINLINE
	#undef FORCEINLINE
#endif
#ifdef VC_USE_FORCEINLINE
	#define FORCEINLINE __forceinline
#else
	#define FORCEINLINE inline
#endif

#ifdef BUILD_64_BIT
	#undef BUILD_64_BIT
#endif
#ifdef BUILD_32_BIT
	#undef BUILD_32_BIT
#endif
#define BUILD_32_BIT sizeof(void*) == 4
#define BUILD_64_BIT sizeof(void*) == 8



#endif
