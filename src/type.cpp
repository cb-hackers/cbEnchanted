#include "precomp.h"
#include "type.h"

Type::Type(int32_t fields) : sizeOfMember((fields + 3) * sizeof(void *)), firstMember(0), lastMember(0) {
	dummyFirst[0] = dummyFirst[1] = 0;
}

void *Type::newMember() {
	void *m = new char[sizeOfMember];
	memset(m, 0, sizeOfMember);

	if (lastMember) {
		setAfter(lastMember, m);
	}

	setBefore(m, lastMember);
	setAfter(m, 0);
	setType(m, this);

	lastMember = m;
	if (firstMember == 0) {
		firstMember = m;
		setAfter(dummyFirst, firstMember);
	}

	return m;
}

void *Type::deleteMember(void *m) {
	void * before = getBefore(m);
	void * after = getAfter(m);
	
	if (before) { //Not first member
		setAfter(before, after);
		if (after == 0) {
			lastMember = before;
		}
		else {
			setBefore(after, before);
		}
		return before;
	}
	else {
		firstMember = after;
		setAfter(dummyFirst, firstMember);
		if (after) {
			setBefore(after, 0);
		}
		else {
			lastMember = 0;
		}
		return dummyFirst;
	}
	delete[] (void**)m;
}

