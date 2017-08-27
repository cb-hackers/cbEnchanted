#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "customfunctionhandler.h"
#include "customfunctiondefines.h"
#include "customfunctions.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

#ifndef DISABLE_ENET
int32_t EnetInterface::enetSocketCounter = 0;
std::unordered_map<int32_t, ENetSocket> EnetInterface::enetSockets;

int32_t EnetInterface::enetSocketSetCounter = 0;
std::unordered_map<int32_t, ENetSocketSet*> EnetInterface::enetSocketSets;

int32_t EnetInterface::enetPacketCounter = 0;
std::unordered_map<int32_t, ENetPacket*> EnetInterface::enetPackets;

int32_t EnetInterface::enetHostCounter = 0;
std::unordered_map<int32_t, ENetHost*> EnetInterface::enetHosts;

int32_t EnetInterface::enetPeerCounter = 0;
std::unordered_map<int32_t, ENetPeer*> EnetInterface::enetPeers;

EnetInterface::EnetInterface()
{
	cb = CBEnchanted::instance();

	CustomFunction func;
	func.setGroupId(CBE_ENET_FUNCTION_GROUP);

	CustomFunctionHandler *handler = cb->getCustomFunctionHandler();

	func.setFuncPtr(&EnetInterface::enetInitialize);
	func.setFuncId(1);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetDeinitialize);
	func.setFuncId(2);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetLinkedVersion);
	func.setFuncId(3);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetTimeGet);
	func.setFuncId(4);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetTimeSet);
	func.setFuncId(5);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketCreate);
	func.setFuncId(6);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketBind);
	func.setFuncId(7);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketGetAddress);
	func.setFuncId(8);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketListen);
	func.setFuncId(10);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketAccept);
	func.setFuncId(11);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketConnect);
	func.setFuncId(12);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketSend);
	func.setFuncId(13);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketReceive);
	func.setFuncId(14);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketWait);
	func.setFuncId(15);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketSetOption);
	func.setFuncId(16);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketGetOption);
	func.setFuncId(17);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketShutdown);
	func.setFuncId(18);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketDestroy);
	func.setFuncId(19);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetSocketSetSelect);
	func.setFuncId(20);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetAddressSetHostIp);
	func.setFuncId(21);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetAddressSetHost);
	func.setFuncId(22);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetAddressGetHostIp);
	func.setFuncId(23);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetAddressGetHost);
	func.setFuncId(24);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPacketCreate);
	func.setFuncId(25);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPacketDestroy);
	func.setFuncId(26);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPacketResize);
	func.setFuncId(27);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetCrc32);
	func.setFuncId(28);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostCreate);
	func.setFuncId(29);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostDestroy);
	func.setFuncId(30);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostConnect);
	func.setFuncId(31);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostCheckEvents);
	func.setFuncId(32);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostService);
	func.setFuncId(33);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostFlush);
	func.setFuncId(34);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostBroadcast);
	func.setFuncId(35);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostCompress);
	func.setFuncId(36);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostCompressWithRangeCoder);
	func.setFuncId(37);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostChannelLimit);
	func.setFuncId(38);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostBandwidthLimit);
	func.setFuncId(39);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostBandwidthThrottle);
	func.setFuncId(40);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetHostRandomSeed);
	func.setFuncId(41);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerSend);
	func.setFuncId(42);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerReceive);
	func.setFuncId(43);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerPing);
	func.setFuncId(44);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerPingInterval);
	func.setFuncId(45);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerTimeout);
	func.setFuncId(46);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerReset);
	func.setFuncId(47);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerDisconnect);
	func.setFuncId(48);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerDisconnectNow);
	func.setFuncId(49);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerDisconnectLater);
	func.setFuncId(50);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerThrottleConfigure);
	func.setFuncId(51);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerThrottle);
	func.setFuncId(52);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerResetQueues);
	func.setFuncId(53);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerSetupOutgoingCommand);
	func.setFuncId(54);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerQueueOutgoingCommand);
	func.setFuncId(55);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerQueueIncomingCommand);
	func.setFuncId(56);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerQueueAcknowledgement);
	func.setFuncId(57);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerDispatchIncomingUnreliableCommands);
	func.setFuncId(58);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerDispatchIncomingReliableCommands);
	func.setFuncId(59);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerOnConnect);
	func.setFuncId(60);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetPeerOnDisconnect);
	func.setFuncId(61);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetRangeCoderCreate);
	func.setFuncId(62);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetRangeCoderDestroy);
	func.setFuncId(63);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetRangeCoderCompress);
	func.setFuncId(64);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetRangeCoderDecompress);
	func.setFuncId(65);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::enetProtocolCommandSize);
	func.setFuncId(66);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::ENETSOCKETSETEMPTY);
	func.setFuncId(67);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::ENETSOCKETSETADD);
	func.setFuncId(68);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::ENETSOCKETSETREMOVE);
	func.setFuncId(69);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::ENETSOCKETSETCHECK);
	func.setFuncId(70);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::createSocketSet);
	func.setFuncId(71);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::deleteSocketSet);
	func.setFuncId(72);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::getEnetPacketFlags);
	func.setFuncId(73);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::getEnetPacketData);
	func.setFuncId(74);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::setEnetPacketData);
	func.setFuncId(75);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::getEnetPacketDataLength);
	func.setFuncId(76);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::getEnetPeerAddress);
	func.setFuncId(77);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::setEnetPeerData);
	func.setFuncId(78);
	handler->addBody(func);

	func.setFuncPtr(&EnetInterface::getEnetPeerData);
	func.setFuncId(79);
	handler->addBody(func);
}

EnetInterface::~EnetInterface()
{

}

void EnetInterface::enetInitialize(CBEnchanted *cb)
{
	cb->pushValue(enet_initialize());
}

void EnetInterface::enetDeinitialize(CBEnchanted *cb)
{
	enet_deinitialize();
	cb->pushValue(0);
}

void EnetInterface::enetLinkedVersion(CBEnchanted *cb)
{
	cb->pushValue(static_cast<int32_t>(enet_linked_version()));
}

void EnetInterface::enetTimeGet(CBEnchanted *cb)
{
	cb->pushValue(static_cast<int32_t>(enet_time_get()));
}

void EnetInterface::enetTimeSet(CBEnchanted *cb)
{
	enet_time_set(cb->popValue().getInt());
	cb->pushValue(0);
}

void EnetInterface::enetProtocolCommandSize(CBEnchanted *cb)
{
	enet_uint8 commandNumber = static_cast<enet_uint8>(cb->popValue().getInt());
	cb->pushValue(static_cast<int32_t>(enet_protocol_command_size(commandNumber)));
}

ENetSocket EnetInterface::getSocket(int32_t id, CBEnchanted *cb, bool errors)
{
	if (enetSockets.count(id) == 0) {
		if (errors) cb->errors->createFatalError("Could not find socket with ID " + std::to_string(id) + "!");
		return ENET_SOCKET_NULL;
	}
	return enetSockets[id];
}

ENetSocketSet* EnetInterface::getSocketSet(int32_t id)
{
	if (enetSocketSets.count(id) == 0) {
		return NULL;
	}
	return enetSocketSets[id];
}

ENetPacket* EnetInterface::getPacket(int32_t id, CBEnchanted *cb, bool errors)
{
	if (enetPackets.count(id) == 0) {
		if (errors) cb->errors->createFatalError("Could not find packet with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetPackets[id];
}

ENetHost *EnetInterface::getHost(int32_t id, CBEnchanted *cb, bool errors)
{
	if (enetHosts.count(id) == 0) {
		if (errors) cb->errors->createFatalError("Could not find host with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetHosts[id];
}

ENetPeer *EnetInterface::getPeer(int32_t id, CBEnchanted *cb, bool errors)
{
	if (enetPeers.count(id) == 0) {
		if (errors) cb->errors->createFatalError("Could not find peer with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetPeers[id];
}

ENetAddress EnetInterface::typeToENetAddress(int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	ENetAddress address;
	address.port = static_cast<enet_uint16>(Type::getMembersType(typePtr)->getShortField(typePtr, 0));
	address.host = static_cast<enet_uint32>(Type::getMembersType(typePtr)->getIntField(typePtr, 2));

	return address;
}

void EnetInterface::ENetAddressToType(const ENetAddress *address, int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	Type::getMembersType(typePtr)->setField(typePtr, 0, static_cast<int16_t>(address->port));
	Type::getMembersType(typePtr)->setField(typePtr, 2, static_cast<int32_t>(address->host));
}

ENetBuffer EnetInterface::typeToENetBuffer(int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	ENetBuffer buffer;
	buffer.dataLength = Type::getMembersType(typePtr)->getIntField(typePtr, 0);

	uint8_t *memblock = cb->memInterface->getMemblock(Type::getMembersType(typePtr)->getIntField(typePtr, 4));
	memcpy(buffer.data, memblock + 4, buffer.dataLength);

	return buffer;
}

void EnetInterface::ENetBufferToType(const ENetBuffer *buffer, int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	Type::getMembersType(typePtr)->setField(typePtr, 0, static_cast<int32_t>(buffer->dataLength));
	uint8_t *memblock = cb->memInterface->getMemblock(Type::getMembersType(typePtr)->getIntField(typePtr, 4));
	memcpy(memblock + 4, buffer->data, buffer->dataLength);
}

ENetEvent EnetInterface::typeToENetEvent(int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	ENetEvent event;
	event.type = static_cast<ENetEventType>(Type::getMembersType(typePtr)->getByteField(typePtr, 0));
	event.peer = getPeer(Type::getMembersType(typePtr)->getIntField(typePtr, 1), cb, false);
	event.channelID = static_cast<enet_uint8>(Type::getMembersType(typePtr)->getByteField(typePtr, 5));
	event.data = static_cast<enet_uint32>(Type::getMembersType(typePtr)->getIntField(typePtr, 6));
	event.packet = getPacket(Type::getMembersType(typePtr)->getIntField(typePtr, 10), cb, false);

	return event;
}

void EnetInterface::ENetEventToType(const ENetEvent *event, int32_t typeId, CBEnchanted *cb)
{
	void* typePtr = cb->getTypePtr(typeId);

	Type::getMembersType(typePtr)->setField(typePtr, 0, static_cast<int32_t>(event->type));

	const int32_t peerID = findPeerID(event->peer);
	if (peerID != 0) {
		Type::getMembersType(typePtr)->setField(typePtr, 1, peerID);
	} else {
		++enetPeerCounter;
		enetPeers[enetPeerCounter] = event->peer;
		Type::getMembersType(typePtr)->setField(typePtr, 1, enetPeerCounter);
	}

	Type::getMembersType(typePtr)->setField(typePtr, 5, static_cast<uint8_t>(event->channelID));
	Type::getMembersType(typePtr)->setField(typePtr, 6, static_cast<int32_t>(event->data));

	const int32_t packetID = findPacketID(event->packet);
	if (packetID != 0) {
		Type::getMembersType(typePtr)->setField(typePtr, 10, packetID);
	} else {
		++enetPacketCounter;
		enetPackets[enetPacketCounter] = event->packet;
		Type::getMembersType(typePtr)->setField(typePtr, 10, enetPacketCounter);
	}
}

int32_t EnetInterface::findPacketID(const ENetPacket *packet)
{
	for (auto IT = enetPackets.begin(); IT != enetPackets.end(); ++IT) {
		if (IT->second == packet) {
			return IT->first;
		}
	}
	return 0;
}

int32_t EnetInterface::findPeerID(const ENetPeer *peer)
{
	for (auto IT = enetPeers.begin(); IT != enetPeers.end(); ++IT) {
		if (IT->second == peer) {
			return IT->first;
		}
	}
	return 0;
}
#endif // DISABLE_ENET
