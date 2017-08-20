#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "customfunctionhandler.h"
#include "customfunctiondefines.h"
#include "customfunctions.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

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
}

EnetInterface::~EnetInterface()
{

}

void EnetInterface::enetInitialize(CBEnchanted *cb)
{
	enet_initialize();
	cb->pushValue(0);
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

	cb->pushValue(enet_socket_bind(socket, &typeToENetAddress(typeId, cb)));
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

	cb->pushValue(enet_socket_connect(socket, &typeToENetAddress(typeId, cb)));
}

void EnetInterface::enetSocketSend(CBEnchanted *cb)
{
	int32_t size = cb->popValue().getInt();
	int32_t typeId2 = cb->popValue().getInt();
	int32_t typeId1 = cb->popValue().getInt();
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_socket_send(socket, &typeToENetAddress(typeId1, cb), &typeToENetBuffer(typeId2, cb), size));
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

void EnetInterface::enetAddressSetHostIp(CBEnchanted *cb)
{
	string hostName = cb->popValue().toString().getUtf8Encoded();
	int32_t typeId = cb->popValue().getInt();

	ENetAddress address;
	int32_t retVal = enet_address_set_host_ip(&address, hostName.c_str());
	ENetAddressToType(&address, typeId, cb);
	cb->pushValue(retVal);
}

void EnetInterface::enetAddressSetHost(CBEnchanted *cb)
{
	string hostName = cb->popValue().toString().getUtf8Encoded();
	int32_t typeId = cb->popValue().getInt();

	ENetAddress address;
	int32_t retVal = enet_address_set_host(&address, hostName.c_str());
	ENetAddressToType(&address, typeId, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetAddressGetHostIp(CBEnchanted *cb)
{
	int32_t nameLength = static_cast<int32_t>(cb->popValue().getInt());
	int32_t typeId = cb->popValue().getInt();

	const ENetAddress address = typeToENetAddress(typeId, cb);
	std::string IPStr(nameLength, 0);
	if(enet_address_get_host_ip(&address, &IPStr[0], 46) < 0) {
		cb->pushValue(std::string(""));
	} else {
		cb->pushValue(IPStr);
	}
}

void EnetInterface::enetAddressGetHost(CBEnchanted *cb)
{
	int32_t nameLength = static_cast<int32_t>(cb->popValue().getInt());
	int32_t typeId = cb->popValue().getInt();

	const ENetAddress address = typeToENetAddress(typeId, cb);
	std::string IPStr(nameLength, 0);
	if(enet_address_get_host(&address, &IPStr[0], 46) < 0) {
		cb->pushValue(std::string(""));
	} else {
		cb->pushValue(IPStr);
	}
}

void EnetInterface::enetPacketCreate(CBEnchanted *cb)
{
	int32_t flags = cb->popValue().getInt();
	size_t dataLength = static_cast<size_t>(cb->popValue().getInt());
	const void* data = (const void*)(cb->memInterface->getMemblock(cb->popValue().getInt()) + 4);
	ENetPacket* packet = enet_packet_create(data, dataLength, flags);
	if (packet == NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		++enetPacketCounter;
		enetPackets[enetPacketCounter] = packet;
		cb->pushValue(enetPacketCounter);
	}
}

void EnetInterface::enetPacketDestroy(CBEnchanted *cb)
{
	int32_t packetId = cb->popValue().getInt();

	enet_packet_destroy(getPacket(packetId, cb));
	enetPackets.erase(packetId);

	cb->pushValue(0);
}

void EnetInterface::enetPacketResize(CBEnchanted *cb)
{
	size_t dataLength = static_cast<size_t>(cb->popValue().getInt());
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);

	cb->pushValue(enet_packet_resize(packet, dataLength));
}

void EnetInterface::enetCrc32(CBEnchanted *cb)
{
	size_t dataLength = static_cast<size_t>(cb->popValue().getInt());
	int32_t typeId = cb->popValue().getInt();

	const ENetBuffer buffer = typeToENetBuffer(typeId, cb);
	const int32_t retVal = static_cast<int32_t>(enet_crc32(&buffer, dataLength));

	cb->pushValue(retVal);
}

void EnetInterface::enetHostCreate(CBEnchanted *cb)
{
	enet_uint32 outgoingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 incomingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	size_t channelLimit = static_cast<size_t>(cb->popValue().getInt());
	size_t peerCount = static_cast<size_t>(cb->popValue().getInt());
	const ENetAddress* address = &typeToENetAddress(cb->popValue().getInt(), cb);

	ENetHost* host = enet_host_create(address, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);

	if (host == NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		++enetHostCounter;
		enetHosts[enetHostCounter] = host;
		cb->pushValue(enetHostCounter);
	}
}

void EnetInterface::enetHostDestroy(CBEnchanted *cb)
{
	int32_t hostId = cb->popValue().getInt();

	enet_host_destroy(getHost(hostId, cb));
	enetHosts.erase(hostId);

	cb->pushValue(0);
}

void EnetInterface::enetHostConnect(CBEnchanted *cb)
{
	enet_uint32 data = static_cast<enet_uint32>(cb->popValue().getInt());
	size_t channelCount = static_cast<size_t>(cb->popValue().getInt());
	const ENetAddress* address = &typeToENetAddress(cb->popValue().getInt(), cb);
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	ENetPeer* retPeer = enet_host_connect(host, address, channelCount, data);

	if (retPeer == NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		++enetPeerCounter;
		enetPeers[enetPeerCounter] = retPeer;
		cb->pushValue(enetPeerCounter);
	}
}

void EnetInterface::enetHostCheckEvents(CBEnchanted *cb)
{
	int32_t eventTypeId = cb->popValue().getInt();
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	ENetEvent* event = &typeToENetEvent(eventTypeId, cb);
	int32_t retVal = enet_host_check_events(host, event);
	ENetEventToType(event, eventTypeId, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetHostService(CBEnchanted *cb)
{
	enet_uint32 timeout = cb->popValue().getInt();
	int32_t eventTypeId = cb->popValue().getInt();
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	ENetEvent* event = &typeToENetEvent(eventTypeId, cb);
	int32_t retVal = enet_host_service(host, event, timeout);
	ENetEventToType(event, eventTypeId, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetHostFlush(CBEnchanted *cb)
{
	enet_host_flush(getHost(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetHostBroadcast(CBEnchanted *cb)
{
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);
	enet_uint8 channelID = static_cast<enet_uint8>(cb->popValue().getInt());
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	enet_host_broadcast(host, channelID, packet);
	cb->pushValue(0);
}

void EnetInterface::enetHostCompress(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetHostCompressWithRangeCoder(CBEnchanted *cb)
{
	ENetHost* host = getHost(cb->popValue().getInt(), cb);
	cb->pushValue(enet_host_compress_with_range_coder(host));
}

void EnetInterface::enetHostChannelLimit(CBEnchanted *cb)
{
	size_t channelLimit = static_cast<size_t>(cb->popValue().getInt());
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	enet_host_channel_limit(host, channelLimit);
	cb->pushValue(0);
}

void EnetInterface::enetHostBandwidthLimit(CBEnchanted *cb)
{
	enet_uint32 outgoingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 incomingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	enet_host_bandwidth_limit(host, incomingBandwidth, outgoingBandwidth);
	cb->pushValue(0);
}

void EnetInterface::enetHostBandwidthThrottle(CBEnchanted *cb)
{
	enet_host_bandwidth_throttle(getHost(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetHostRandomSeed(CBEnchanted *cb)
{
	cb->pushValue(static_cast<int32_t>(enet_host_random_seed()));
}

void EnetInterface::enetPeerSend(CBEnchanted *cb)
{
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);
	enet_uint8 channelID = static_cast<enet_uint8>(cb->popValue().getInt());
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);

	cb->pushValue(enet_peer_send(peer, channelID, packet));
}

void EnetInterface::enetPeerReceive(CBEnchanted *cb)
{
	int32_t channelIDMemblockID = cb->popValue().getInt();
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);

	ENetPacket* packet = enet_peer_receive(peer, cb->memInterface->getMemblock(channelIDMemblockID) + 4);

	if (packet == NULL) {
		cb->pushValue(CBE_ENET_NULL);
	} else {
		++enetPacketCounter;
		enetPackets[enetPacketCounter] = packet;
		cb->pushValue(enetPacketCounter);
	}
}

void EnetInterface::enetPeerPing(CBEnchanted *cb)
{
	enet_peer_ping(getPeer(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetPeerPingInterval(CBEnchanted *cb)
{
	enet_uint32 pingInterval = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_peer_ping_interval(getPeer(cb->popValue().getInt(), cb), pingInterval);
	cb->pushValue(0);
}

void EnetInterface::enetPeerTimeout(CBEnchanted *cb)
{
	enet_uint32 timeoutMaximum = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 timeoutMinimum = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 timeoutLimit = static_cast<enet_uint32>(cb->popValue().getInt());
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);

	enet_peer_timeout(peer, timeoutLimit, timeoutMinimum, timeoutMaximum);
	cb->pushValue(0);
}

void EnetInterface::enetPeerReset(CBEnchanted *cb)
{
	enet_peer_reset(getPeer(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetPeerDisconnect(CBEnchanted *cb)
{
	enet_uint32 data = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_peer_disconnect(getPeer(cb->popValue().getInt(), cb), data);
	cb->pushValue(0);
}

void EnetInterface::enetPeerDisconnectNow(CBEnchanted *cb)
{
	enet_uint32 data = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_peer_disconnect_now(getPeer(cb->popValue().getInt(), cb), data);
	cb->pushValue(0);
}

void EnetInterface::enetPeerDisconnectLater(CBEnchanted *cb)
{
	enet_uint32 data = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_peer_disconnect_later(getPeer(cb->popValue().getInt(), cb), data);
	cb->pushValue(0);
}

void EnetInterface::enetPeerThrottleConfigure(CBEnchanted *cb)
{
	enet_uint32 deceleration = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 acceleration = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 interval = static_cast<enet_uint32>(cb->popValue().getInt());
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);

	enet_peer_throttle_configure(peer, interval, acceleration, deceleration);
	cb->pushValue(0);
}

void EnetInterface::enetPeerThrottle(CBEnchanted *cb)
{
	enet_uint32 rtt = static_cast<enet_uint32>(cb->popValue().getInt());
	cb->pushValue(enet_peer_throttle(getPeer(cb->popValue().getInt(), cb), rtt));
}

void EnetInterface::enetPeerResetQueues(CBEnchanted *cb)
{
	enet_peer_reset_queues(getPeer(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetPeerSetupOutgoingCommand(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerQueueOutgoingCommand(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerQueueIncomingCommand(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerQueueAcknowledgement(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerDispatchIncomingUnreliableCommands(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerDispatchIncomingReliableCommands(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetPeerOnConnect(CBEnchanted *cb)
{
	enet_peer_on_connect(getPeer(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetPeerOnDisconnect(CBEnchanted *cb)
{
	enet_peer_on_disconnect(getPeer(cb->popValue().getInt(), cb));
	cb->pushValue(0);
}

void EnetInterface::enetRangeCoderCreate(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetRangeCoderDestroy(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetRangeCoderCompress(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetRangeCoderDecompress(CBEnchanted *cb)
{
	STUB
}

void EnetInterface::enetProtocolCommandSize(CBEnchanted *cb)
{
	enet_uint8 commandNumber = static_cast<enet_uint8>(cb->popValue().getInt());
	cb->pushValue(static_cast<int32_t>(enet_protocol_command_size(commandNumber)));
}

void EnetInterface::ENETSOCKETSETEMPTY(CBEnchanted *cb)
{
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_EMPTY(socketSet);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETADD(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_ADD(socketSet, socket);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETREMOVE(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	ENET_SOCKETSET_REMOVE(socketSet, socket);
	cb->pushValue(0);
}

void EnetInterface::ENETSOCKETSETCHECK(CBEnchanted *cb)
{
	ENetSocket socket = getSocket(cb->popValue().getInt(), cb);
	ENetSocketSet* socketSet = getSocketSet(cb->popValue().getInt());
	cb->pushValue(ENET_SOCKETSET_CHECK(socketSet, socket));
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

ENetSocket EnetInterface::getSocket(int32_t id, CBEnchanted *cb)
{
	if (enetSockets.count(id) == 0) {
		cb->errors->createFatalError("Could not find socket with ID " + std::to_string(id) + "!");
		return 0;
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

ENetPacket* EnetInterface::getPacket(int32_t id, CBEnchanted *cb)
{
	if (enetPackets.count(id) == 0) {
		cb->errors->createFatalError("Could not find packet with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetPackets[id];
}

ENetHost *EnetInterface::getHost(int32_t id, CBEnchanted *cb)
{
	if (enetHosts.count(id) == 0) {
		cb->errors->createFatalError("Could not find host with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetHosts[id];
}

ENetPeer *EnetInterface::getPeer(int32_t id, CBEnchanted *cb)
{
	if (enetPeers.count(id) == 0) {
		cb->errors->createFatalError("Could not find peer with ID " + std::to_string(id) + "!");
		return NULL;
	}
	return enetPeers[id];
}

ENetAddress EnetInterface::typeToENetAddress(int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	ENetAddress address;
	address.port = type->getShortField(typePtr, 0);

	string IPStr = type->getStringField(typePtr, 2).getStdString();
	if(enet_address_set_host_ip(&address, IPStr.c_str()) < 0) {
		cb->errors->createFatalError("Failed to convert string " + IPStr + " to IP address.");
	}

	return address;
}

void EnetInterface::ENetAddressToType(const ENetAddress *address, int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	type->setField(typePtr, 0, address->port);
	char IPStr[46];
	if(enet_address_get_host_ip(address, IPStr, 46) < 0) {
		cb->errors->createFatalError("Failed to convert address " + std::to_string(address->host) + " to printable form.");
	}
	type->setField(typePtr, 2, ISString(IPStr));
}

ENetBuffer EnetInterface::typeToENetBuffer(int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	ENetBuffer buffer;
	buffer.dataLength = type->getIntField(typePtr, 0);

	uint8_t *memblock = cb->memInterface->getMemblock(type->getIntField(typePtr, 4));
	memcpy(buffer.data, memblock + 4, buffer.dataLength);

	return buffer;
}

void EnetInterface::ENetBufferToType(const ENetBuffer *buffer, int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	type->setField(typePtr, 0, static_cast<int32_t>(buffer->dataLength));
	uint8_t *memblock = cb->memInterface->getMemblock(type->getIntField(typePtr, 4));
	memcpy(memblock + 4, buffer->data, buffer->dataLength);
}

ENetEvent EnetInterface::typeToENetEvent(int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	ENetEvent event;
	event.type = static_cast<ENetEventType>(type->getByteField(typePtr, 0));
	event.peer = getPeer(type->getIntField(typePtr, 1), cb);
	event.channelID = static_cast<enet_uint8>(type->getByteField(typePtr, 5));
	event.data = static_cast<enet_uint32>(type->getIntField(typePtr, 6));
	event.packet = getPacket(type->getIntField(typePtr, 10), cb);

	return event;
}

void EnetInterface::ENetEventToType(const ENetEvent *event, int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	type->setField(typePtr, 0, static_cast<int32_t>(event->type));

	const int32_t peerID = findPeerID(event->peer);
	if (peerID != 0) {
		type->setField(typePtr, 1, peerID);
	} else {
		++enetPeerCounter;
		enetPeers[enetPeerCounter] = event->peer;
		type->setField(typePtr, 1, enetPeerCounter);
	}

	type->setField(typePtr, 5, static_cast<uint8_t>(event->channelID));
	type->setField(typePtr, 6, static_cast<int32_t>(event->data));

	const int32_t packetID = findPacketID(event->packet);
	if (packetID != 0) {
		type->setField(typePtr, 10, packetID);
	} else {
		++enetPacketCounter;
		enetPackets[enetPacketCounter] = event->packet;
		type->setField(typePtr, 10, enetPacketCounter);
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
