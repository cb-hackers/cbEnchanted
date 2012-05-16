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
	this->thisMember = m;

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


void Type::insertAt(void *insertionPoint) {
	if (insertionPoint == 0) return;
	void *m = this->thisMember;

	void * oldBefore = getBefore(m);
	void * oldAfter = getAfter(m);

	void * beforeInsertionPoint = getBefore(insertionPoint);

	if (lastMember == m) {
		// This was the last member
		cout << "Inserting the last member" << endl;
		lastMember = oldBefore;
	}
	else {
		cout << "Inserting type member that isn't the last member" << endl;
		setAfter(oldBefore, oldAfter);
	}

	if (beforeInsertionPoint) {
		// The point to be inserted isn't the first point, set after the given one
		cout << " -> Inserted to middle" << endl;
		void * afterInsertionPoint = getAfter(insertionPoint);
		setAfter(m, afterInsertionPoint);
		setBefore(m, insertionPoint);
		setAfter(insertionPoint, m);
		setBefore(afterInsertionPoint, m);
	}
	else {
		// This was to be set as the first one.
		cout << " -> Inserted to be the first member" << endl;
		firstMember = m;
		setBefore(m, 0);
		setAfter(m, insertionPoint);
		setAfter(oldBefore, oldAfter);
	}
}

