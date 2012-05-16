#include "precomp.h"
#include "type.h"

Type::Type(int32_t dataSize) : sizeOfMember(sizeof(void*)*3+dataSize * (sizeof(void*)/4)), firstMember(0), lastMember(0) {
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


void Type::insert(void *m, void *insertionPoint) {
	if (insertionPoint == 0) return;
	if (m == insertionPoint) return;

	void * oldBefore = getBefore(m);
	void * oldAfter = getAfter(m);

	// Forget the place of m
	if (oldBefore != 0) {
		setAfter(oldBefore, oldAfter);
	}
	if (oldAfter != 0) {
		setBefore(oldAfter, oldBefore);
	}

	// Check for validity of firstMember and lastMember after m has been "popped out".
	if (m == firstMember) {
		firstMember = oldAfter;
	}
	else if (m == lastMember) {
		lastMember = oldBefore;
	}

	// Now there should be no more links to m left. It's out of the flow.
	// We can now freely put it back into the place it should go to.

	if (insertionPoint == firstMember) {
		// We wanted to insert to the first position
		setBefore(m, 0);
		setAfter(m, insertionPoint);
		setBefore(insertionPoint, m);
		firstMember = m;
	}
	else if (insertionPoint == lastMember) {
		// We wanted to insert after the last one
		setBefore(m, insertionPoint);
		setAfter(m, 0);
		setAfter(insertionPoint, m);
		lastMember = m;
	}
	else {
		// We wanted to insert between two members, after the given insertion point.
		void * afterInsertionPoint = getAfter(insertionPoint);

		setBefore(m, insertionPoint);
		setAfter(m, afterInsertionPoint);
		setAfter(insertionPoint, m);
		setBefore(afterInsertionPoint, m);
	}

}

