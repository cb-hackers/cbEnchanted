#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

#ifndef DISABLE_ENET
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

void EnetInterface::getEnetPeerAddress(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);

	ENetAddressToType(&peer->address, typeId, cb);
	cb->pushValue(0);
}

void EnetInterface::setEnetPeerData(CBEnchanted *cb)
{
	int32_t data = cb->popValue().getInt();
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);
	if (peer->data != NULL) delete peer->data;
	peer->data = new int32_t;
	*(int32_t*)peer->data = data;
	cb->pushValue(0);
}

void EnetInterface::getEnetPeerData(CBEnchanted *cb)
{
	ENetPeer* peer = getPeer(cb->popValue().getInt(), cb);
	cb->pushValue(*(int32_t*)peer->data);
}
#endif // DISABLE_ENET
