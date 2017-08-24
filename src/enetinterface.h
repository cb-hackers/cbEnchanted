#ifndef ENETINTERFACE_H
#define ENETINTERFACE_H

#include "precomp.h"

#define CBE_ENET_NULL 0

#if ENET_VERSION < 66317
	#define enet_address_set_host_ip(address, string) enet_address_set_host(address, string)
#endif

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
		static void enetSocketSetSelect(CBEnchanted *cb);

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

		static void ENETSOCKETSETEMPTY(CBEnchanted *cb);
		static void ENETSOCKETSETADD(CBEnchanted *cb);
		static void ENETSOCKETSETREMOVE(CBEnchanted *cb);
		static void ENETSOCKETSETCHECK(CBEnchanted *cb);

		static void createSocketSet(CBEnchanted *cb);
		static void deleteSocketSet(CBEnchanted *cb);

		static void getEnetPacketFlags(CBEnchanted *cb);
		static void getEnetPacketData(CBEnchanted *cb);
		static void setEnetPacketData(CBEnchanted *cb);
		static void getEnetPacketDataLength(CBEnchanted *cb);

		static void getEnetPeerAddress(CBEnchanted *cb);
		static void setEnetPeerData(CBEnchanted *cb);
		static void getEnetPeerData(CBEnchanted *cb);

		static ENetSocket getSocket(int32_t id, CBEnchanted *cb, bool errors = true);
		static ENetSocketSet* getSocketSet(int32_t id);
		static ENetPacket* getPacket(int32_t id, CBEnchanted *cb, bool errors = true);
		static ENetHost* getHost(int32_t id, CBEnchanted *cb, bool errors = true);
		static ENetPeer* getPeer(int32_t id, CBEnchanted *cb, bool errors = true);
	private:
		CBEnchanted *cb;

		static int32_t enetSocketCounter;
		static std::unordered_map<int32_t, ENetSocket> enetSockets;

		static int32_t enetSocketSetCounter;
		static std::unordered_map<int32_t, ENetSocketSet*> enetSocketSets;

		static int32_t enetPacketCounter;
		static std::unordered_map<int32_t, ENetPacket*> enetPackets;

		static int32_t enetHostCounter;
		static std::unordered_map<int32_t, ENetHost*> enetHosts;

		static int32_t enetPeerCounter;
		static std::unordered_map<int32_t, ENetPeer*> enetPeers;

		static ENetAddress typeToENetAddress(int32_t typeId, CBEnchanted *cb);
		static void ENetAddressToType(const ENetAddress * address, int32_t typeId, CBEnchanted *cb);

		static ENetBuffer typeToENetBuffer(int32_t typeId, CBEnchanted *cb);
		static void ENetBufferToType(const ENetBuffer * buffer, int32_t typeId, CBEnchanted *cb);

		static ENetEvent typeToENetEvent(int32_t typeId, CBEnchanted *cb);
		static void ENetEventToType(const ENetEvent * event, int32_t typeId, CBEnchanted *cb);

		static int32_t findPacketID(const ENetPacket* packet);
		static int32_t findPeerID(const ENetPeer* peer);
};

#endif // ENETINTERFACE_H
