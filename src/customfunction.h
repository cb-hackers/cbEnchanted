#ifndef CBCUSTOMFUNCTION_H
#define CBCUSTOMFUNCTION_H
#include "precomp.h"
class CBEnchanted;

/** @defgroup customfunctions Custom functions and commands
  */


class CustomFunction
{
	public:
		typedef void (*CustomFunc) (CBEnchanted *);

		CustomFunction(CustomFunc func,int32_t group,int32_t id) :
			function(func), groupId(group), funcId(id) { }
		CustomFunction() : function(0), groupId(0), funcId(0) {}
		inline void call(CBEnchanted *cb) const { (*function)(cb); }
		bool isNull() const {return function == 0;}
		bool operator > (const CustomFunction &o) const;
		bool operator < (const CustomFunction &o) const;
		bool operator == (const CustomFunction &o);
		bool operator != (const CustomFunction &o);
		CustomFunc getFuncPtr() const { return function; }
		void setFuncPtr(CustomFunc func) { function = func; }
		int32_t getHandle() const { return handle; }
		void setHandle( int32_t h ) { handle = h; }
		int32_t getGroup() const { return groupId; }
		void setGroupId(int32_t g) { groupId = g; }
		int32_t getFuncId() const {return funcId; }
		void setFuncId(int32_t f) { funcId = f; }

	private:
		int32_t groupId;
		int32_t funcId;
		int32_t handle;
		CustomFunc function;
};


#endif // CBCUSTOMFUNCTION_H
