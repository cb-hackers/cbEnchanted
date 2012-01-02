#ifndef PRECOMP_H
#define PRECOMP_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
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
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795028841971693993751
#endif

using namespace std;
using namespace boost::filesystem;

using boost::any;
using boost::any_cast;
using boost::bind;
using boost::function;
using boost::lexical_cast;

#endif
