#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

#ifndef DISABLE_ENET
void EnetInterface::enetHostCreate(CBEnchanted *cb)
{
	enet_uint32 outgoingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	enet_uint32 incomingBandwidth = static_cast<enet_uint32>(cb->popValue().getInt());
	size_t channelLimit = static_cast<size_t>(cb->popValue().getInt());
	size_t peerCount = static_cast<size_t>(cb->popValue().getInt());
	int32_t addressTypeId = cb->popValue().getInt();

	ENetAddress* address = NULL;
	ENetAddress addressFromType;
	if (addressTypeId != 0) {
		addressFromType = typeToENetAddress(addressTypeId, cb);
		address = &addressFromType;
	}

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
	int32_t addressTypeId = cb->popValue().getInt();

	ENetAddress* address = NULL;
	ENetAddress addressFromType;
	if (addressTypeId != 0) {
		addressFromType = typeToENetAddress(addressTypeId, cb);
		address = &addressFromType;
	}

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

	ENetEvent* event = NULL;
	ENetEvent eventFromType;
	if (eventTypeId != 0) {
		eventFromType = typeToENetEvent(eventTypeId, cb);
		event = &eventFromType;
	}

	int32_t retVal = enet_host_check_events(host, event);
	ENetEventToType(event, eventTypeId, cb);

	cb->pushValue(retVal);
}

void EnetInterface::enetHostService(CBEnchanted *cb)
{
	enet_uint32 timeout = cb->popValue().getInt();
	int32_t eventTypeId = cb->popValue().getInt();
	ENetHost* host = getHost(cb->popValue().getInt(), cb);

	ENetEvent* event = NULL;
	ENetEvent eventFromType;
	if (eventTypeId != 0) {
		eventFromType = typeToENetEvent(eventTypeId, cb);
		event = &eventFromType;
	}

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
#endif // DISABLE_ENET
