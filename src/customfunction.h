#ifndef CBCUSTOMFUNCTION_H
#define CBCUSTOMFUNCTION_H
#include "precomp.h"
class CBEnchanted;

class CustomFunction
{
	public:
		typedef void (*CustomFunc) (void);

		CustomFunction(CustomFunc func,int32_t group,int32_t id) :
			function(func), groupId(group), funcId(id) { }
		CustomFunction() : function(0), groupId(0), funcId(0) {}

		void call() const { (*function)(); }
		bool isNull() const {return function == 0;}
		bool operator > (const CustomFunction &o);
		bool operator < (const CustomFunction &o);
		bool operator == (const CustomFunction &o);
		void setParams(const vector<int32_t> &p) { params = p; }
		const vector<int32_t> &getParams() const { return params; }
		int32_t getHandle() const { return handle; }
		void setHandle( int32_t h ) { handle = h; }
	private:
		vector<int32_t> params;
		int32_t groupId;
		int32_t funcId;
		int32_t handle;
		CustomFunc function;
};


#endif // CBCUSTOMFUNCTION_H
