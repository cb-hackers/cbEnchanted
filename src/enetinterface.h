#ifndef ENETINTERFACE_H
#define ENETINTERFACE_H

#include "precomp.h"

class CBEnchanted;

class EnetInterface {
	public:
		EnetInterface();
		~EnetInterface();

		static void enetInitialize(CBEnchanted *cb);
		static void enetDeinitialize(CBEnchanted *cb);
		static void enetLinkedVersion(CBEnchanted *cb);
		static void enetTimeGet(CBEnchanted *cb);
		static void enetTimeSet(CBEnchanted *cb);

		static void enetSocketCreate(CBEnchanted *cb);
		static void enetSocketBind(CBEnchanted *cb);
		static void enetSocketGetAddress(CBEnchanted *cb);
		static void enetSocketGetPort(CBEnchanted *cb);
		static void enetSocketListen(CBEnchanted *cb);
		static void enetSocketAccept(CBEnchanted *cb);
		static void enetSocketConnect(CBEnchanted *cb);
		static void enetSocketSend(CBEnchanted *cb);
		static void enetSocketReceive(CBEnchanted *cb);
		static void enetSocketWait(CBEnchanted *cb);
		static void enetSocketSetOption(CBEnchanted *cb);
		static void enetSocketGetOption(CBEnchanted *cb);
		static void enetSocketShutdown(CBEnchanted *cb);
		static void enetSocketDestroy(CBEnchanted *cb);
		static void enetSocketsetSelect(CBEnchanted *cb);

		static void enetAddressSetHostIp(CBEnchanted *cb);
		static void enetAddressSetHost(CBEnchanted *cb);
		static void enetAddressGetHostIp(CBEnchanted *cb);
		static void enetAddressGetHost(CBEnchanted *cb);
		static void enetPacketCreate(CBEnchanted *cb);
		static void enetPacketDestroy(CBEnchanted *cb);
		static void enetPacketResize (CBEnchanted *cb);
		static void enetCrc32(CBEnchanted *cb);

		static void enetHostCreate(CBEnchanted *cb);
		static void enetHostDestroy(CBEnchanted *cb);
		static void enetHostConnect(CBEnchanted *cb);
		static void enetHostCheckEvents(CBEnchanted *cb);
		static void enetHostService(CBEnchanted *cb);
		static void enetHostFlush(CBEnchanted *cb);
		static void enetHostBroadcast(CBEnchanted *cb);
		static void enetHostCompress(CBEnchanted *cb);
		static void enetHostCompressWithRangeCoder(CBEnchanted *cb);
		static void enetHostChannelLimit(CBEnchanted *cb);
		static void enetHostBandwidthLimit(CBEnchanted *cb);
		static void enetHostBandwidthThrottle(CBEnchanted *cb);
		static void enetHostRandomSeed(CBEnchanted *cb);

		static void enetPeerSend(CBEnchanted *cb);
		static void enetPeerReceive(CBEnchanted *cb);
		static void enetPeerPing(CBEnchanted *cb);
		static void enetPeerPingInterval(CBEnchanted *cb);
		static void enetPeerTimeout(CBEnchanted *cb);
		static void enetPeerReset(CBEnchanted *cb);
		static void enetPeerDisconnect(CBEnchanted *cb);
		static void enetPeerDisconnectNow(CBEnchanted *cb);
		static void enetPeerDisconnectLater(CBEnchanted *cb);
		static void enetPeerThrottleConfigure(CBEnchanted *cb);
		static void enetPeerThrottle(CBEnchanted *cb);
		static void enetPeerResetQueues(CBEnchanted *cb);
		static void enetPeerSetupOutgoingCommand(CBEnchanted *cb);
		static void enetPeerQueueOutgoingCommand(CBEnchanted *cb);
		static void enetPeerQueueIncomingCommand(CBEnchanted *cb);
		static void enetPeerQueueAcknowledgement(CBEnchanted *cb);
		static void enetPeerDispatchIncomingUnreliableCommands(CBEnchanted *cb);
		static void enetPeerDispatchIncomingReliableCommands(CBEnchanted *cb);
		static void enetPeerOnConnect(CBEnchanted *cb);
		static void enetPeerOnDisconnect(CBEnchanted *cb);

		static void enetRangeCoderCreate(CBEnchanted *cb);
		static void enetRangeCoderDestroy(CBEnchanted *cb);
		static void enetRangeCoderCompress(CBEnchanted *cb);
		static void enetRangeCoderDecompress(CBEnchanted *cb);

		static void enetProtocolCommandSize(CBEnchanted *cb);

		static ENetAddress typeToAddress(int32_t typeId, CBEnchanted *cb);

	private:
		CBEnchanted *cb;

		static int32_t enetSocketCounter;
		static std::unordered_map<int32_t, ENetSocket> enetSockets;
};

#endif // ENETINTERFACE_H
