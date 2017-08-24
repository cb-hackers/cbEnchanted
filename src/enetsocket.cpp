#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

#ifndef DISABLE_ENET
void EnetInterface::enetSocketCreate(CBEnchanted *cb)
{
	int32_t socketType = cb->popValue().getInt();

	ENetSocket socket = enet_socket_create(static_cast<ENetSocketType>(socketType));
	if (socket == ENET_SOCKET_NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		++enetSocketCounter;
		enetSockets[enetSocketCounter] = socket;
		cb->pushValue(enetSocketCounter);
	}
}

void EnetInterface::enetSocketBind(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress* address = NULL;
	ENetAddress addressFromType;
	if (typeId != 0) {
		addressFromType = typeToENetAddress(typeId, cb);
		address = &addressFromType;
	}

	cb->pushValue(enet_socket_bind(socket, address));
}

void EnetInterface::enetSocketGetAddress(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress address;
	int32_t retVal = enet_socket_get_address(socket, &address);
	ENetAddressToType(&address, typeId, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetSocketListen(CBEnchanted *cb)
{
	int32_t backlog = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socket_listen(socket, backlog));
}

void EnetInterface::enetSocketAccept(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress address;
	ENetSocket retSocket = enet_socket_accept(socket, &address);
	if (retSocket == ENET_SOCKET_NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		ENetAddressToType(&address, typeId, cb);
		++enetSocketCounter;
		enetSockets[enetSocketCounter] = socket;
		cb->pushValue(enetSocketCounter);
	}
}

void EnetInterface::enetSocketConnect(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress* address = NULL;
	ENetAddress addressFromType;
	if (typeId != 0) {
		addressFromType = typeToENetAddress(typeId, cb);
		address = &addressFromType;
	}

	cb->pushValue(enet_socket_connect(socket, address));
}

void EnetInterface::enetSocketSend(CBEnchanted *cb)
{
	int32_t size = cb->popValue().getInt();
	int32_t typeId2 = cb->popValue().getInt();
	int32_t typeId1 = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress* address = NULL;
	ENetAddress addressFromType;
	if (typeId1 != 0) {
		addressFromType = typeToENetAddress(typeId1, cb);
		address = &addressFromType;
	}

	ENetBuffer* buffer = NULL;
	ENetBuffer bufferFromType;
	if (typeId2 != 0) {
		bufferFromType = typeToENetBuffer(typeId2, cb);
		buffer = &bufferFromType;
	}

	cb->pushValue(enet_socket_send(socket, address, buffer, size));
}

void EnetInterface::enetSocketReceive(CBEnchanted *cb)
{
	int32_t size = cb->popValue().getInt();
	int32_t typeId2 = cb->popValue().getInt();
	int32_t typeId1 = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	ENetAddress address;
	ENetBuffer buffer;
	int32_t retVal = enet_socket_receive(socket, &address, &buffer, size);
	ENetAddressToType(&address, typeId1, cb);
	ENetBufferToType(&buffer, typeId2, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetSocketWait(CBEnchanted *cb)
{
	int32_t timeout = cb->popValue().getInt();
	int32_t memblockId = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	uint8_t *conditionMemblock = cb->memInterface->getMemblock(memblockId);
	cb->pushValue(enet_socket_wait(socket, (enet_uint32 *)(conditionMemblock + 4), timeout));
}

void EnetInterface::enetSocketSetOption(CBEnchanted *cb)
{
	int32_t value = cb->popValue().getInt();
	int32_t socketOption = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socket_set_option(socket, static_cast<ENetSocketOption>(socketOption), value));
}

void EnetInterface::enetSocketGetOption(CBEnchanted *cb)
{
	uint8_t *valueMemblock = cb->memInterface->getMemblock(cb->popValue().getInt());
	int32_t socketOption = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socket_get_option(socket, static_cast<ENetSocketOption>(socketOption), (int *)(valueMemblock + 4)));
}

void EnetInterface::enetSocketShutdown(CBEnchanted *cb)
{
	int32_t socketShutdown = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socket_shutdown(socket, static_cast<ENetSocketShutdown>(socketShutdown)));
}

void EnetInterface::enetSocketDestroy(CBEnchanted *cb)
{
	int32_t socketId = cb->popValue().getInt();

	enet_socket_destroy(getSocket(socketId, cb));
	enetSockets.erase(socketId);

	cb->pushValue(0);
}

void EnetInterface::enetSocketSetSelect(CBEnchanted *cb)
{
	int32_t timeout = cb->popValue().getInt();
	ENetSocketSet* writeSet = getSocketSet(cb->popValue().getInt());
	ENetSocketSet* readSet = getSocketSet(cb->popValue().getInt());
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socketset_select(socket, readSet, writeSet, timeout));
}

void EnetInterface::ENETSOCKETSETEMPTY(CBEnchanted *cb)
{
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_EMPTY(*socketSet);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETADD(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_ADD(*socketSet, socket);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETREMOVE(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_REMOVE(*socketSet, socket);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETCHECK(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	cb->pushValue(ENET_SOCKETSET_CHECK(*socketSet, socket));
}

void EnetInterface::createSocketSet(CBEnchanted *cb)
{
	++enetSocketSetCounter;
	enetSocketSets[enetSocketSetCounter] = new ENetSocketSet();
	cb->pushValue(enetSocketSetCounter);
}

void EnetInterface::deleteSocketSet(CBEnchanted *cb)
{
	int32_t socketSetID = cb->popValue().getInt();
	ENetSocketSet* socketSet = getSocketSet(socketSetID);
	delete socketSet;
	enetSocketSets.erase(socketSetID);
	cb->pushValue(0);
}
#endif // DISABLE_ENET
