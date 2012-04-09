#ifndef TYPE_H
#define TYPE_H

#include "precomp.h"
#include "isstring.h"

class Type {
	public:
		Type(int32_t fields);

		void *newMember();
		void *deleteMember(void *m);

		inline static Type *getMembersType(const void *m) { return ((Type**)m)[2]; }
		inline static int32_t &getIntField(const void *m, int32_t field) {
			return *(int32_t*)(((void**)m) + 3 + field);
		}
		inline static float getFloatField(const void *m, int32_t field) {
			return *(float*)(((void**)m) + 3 + field);
		}
		inline static ISString &getStringField(const void *m, int32_t field) {
			return *(ISString*)(((void**)m) + 3 + field);
		}
		inline static void setField(void *m,int32_t field, int32_t value) {
			*(int32_t*)(((void**)m) + 3 + field) = value;
		}
		inline static void setField(void *m,int32_t field, float value) {
			*(float*)(((void**)m) + 3 + field) = value;
		}
		inline static void setField(void *m,int32_t field, const ISString &value) {
			*(ISString*)(((void**)m) + 3 + field) = value;
		}
		inline static void *getBefore(void *m) {
			if (m == 0) return 0;
			return ((void**)m)[0];
		}
		inline static void *getAfter(void *m) {
			if (m == 0) return 0;
			return ((void**)m)[1];
		}
		inline void setBefore(void *m,void *v) {
			((void**)m)[0] = v;
		}
		inline void setAfter(void *m,void *v) {
			((void**)m)[1] = v;
		}

		void *getFirst(){return firstMember;}
		void *getLast(){return lastMember;}
	private:
		inline static void setType(void *m,Type *t) {
			((Type**)m)[2] = t;
		}

		void *dummyFirst[2];
		int32_t sizeOfMember;
		void *firstMember;
		void *lastMember;
};

#endif
