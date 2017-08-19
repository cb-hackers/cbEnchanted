#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "customfunctionhandler.h"
#include "customfunctiondefines.h"
#include "customfunctions.h"
#include "type.h"
#include "errorsystem.h"

int EnetInterface::enetSocketCounter;
std::unordered_map<int32_t, ENetSocket> EnetInterface::enetSockets;

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

	func.setFuncPtr(&EnetInterface::enetSocketGetPort);
	func.setFuncId(9);
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

	func.setFuncPtr(&EnetInterface::enetSocketsetSelect);
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
	++enetSocketCounter;
	enetSockets[enetSocketCounter] = enet_socket_create(static_cast<ENetSocketType>(socketType));
	cb->pushValue(enetSocketCounter);
}

void EnetInterface::enetSocketBind(CBEnchanted *cb)
{
	int32_t typeId = cb->popValue().getInt();
	int32_t socketId = cb->popValue().getInt();
	cb->pushValue(enet_socket_bind(enetSockets[socketId], &typeToAddress(typeId, cb)));
}

void EnetInterface::enetSocketGetAddress(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketGetPort(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketListen(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketAccept(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketConnect(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketSend(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketReceive(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketWait(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketSetOption(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketGetOption(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketShutdown(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketDestroy(CBEnchanted *cb)
{

}

void EnetInterface::enetSocketsetSelect(CBEnchanted *cb)
{

}

void EnetInterface::enetAddressSetHostIp(CBEnchanted *cb)
{

}

void EnetInterface::enetAddressSetHost(CBEnchanted *cb)
{

}

void EnetInterface::enetAddressGetHostIp(CBEnchanted *cb)
{

}

void EnetInterface::enetAddressGetHost(CBEnchanted *cb)
{

}

void EnetInterface::enetPacketCreate(CBEnchanted *cb)
{

}

void EnetInterface::enetPacketDestroy(CBEnchanted *cb)
{

}

void EnetInterface::enetPacketResize(CBEnchanted *cb)
{

}

void EnetInterface::enetCrc32(CBEnchanted *cb)
{

}

void EnetInterface::enetHostCreate(CBEnchanted *cb)
{

}

void EnetInterface::enetHostDestroy(CBEnchanted *cb)
{

}

void EnetInterface::enetHostConnect(CBEnchanted *cb)
{

}

void EnetInterface::enetHostCheckEvents(CBEnchanted *cb)
{

}

void EnetInterface::enetHostService(CBEnchanted *cb)
{

}

void EnetInterface::enetHostFlush(CBEnchanted *cb)
{

}

void EnetInterface::enetHostBroadcast(CBEnchanted *cb)
{

}

void EnetInterface::enetHostCompress(CBEnchanted *cb)
{

}

void EnetInterface::enetHostCompressWithRangeCoder(CBEnchanted *cb)
{

}

void EnetInterface::enetHostChannelLimit(CBEnchanted *cb)
{

}

void EnetInterface::enetHostBandwidthLimit(CBEnchanted *cb)
{

}

void EnetInterface::enetHostBandwidthThrottle(CBEnchanted *cb)
{

}

void EnetInterface::enetHostRandomSeed(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerSend(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerReceive(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerPing(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerPingInterval(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerTimeout(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerReset(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerDisconnect(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerDisconnectNow(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerDisconnectLater(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerThrottleConfigure(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerThrottle(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerResetQueues(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerSetupOutgoingCommand(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerQueueOutgoingCommand(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerQueueIncomingCommand(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerQueueAcknowledgement(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerDispatchIncomingUnreliableCommands(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerDispatchIncomingReliableCommands(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerOnConnect(CBEnchanted *cb)
{

}

void EnetInterface::enetPeerOnDisconnect(CBEnchanted *cb)
{

}

void EnetInterface::enetRangeCoderCreate(CBEnchanted *cb)
{

}

void EnetInterface::enetRangeCoderDestroy(CBEnchanted *cb)
{

}

void EnetInterface::enetRangeCoderCompress(CBEnchanted *cb)
{

}

void EnetInterface::enetRangeCoderDecompress(CBEnchanted *cb)
{

}

void EnetInterface::enetProtocolCommandSize(CBEnchanted *cb)
{

}

ENetAddress EnetInterface::typeToAddress(int32_t typeId, CBEnchanted *cb)
{
	Type *type = cb->getType(typeId);
	void * typePtr = cb->getTypePointerVariable(cb->getCustomFunctionHandler()->getLatestHandle());

	ENetAddress address;
	address.port = type->getShortField(typePtr, 0);

	string IPStr = type->getStringField(typePtr, 2).getStdString();
	if(enet_address_set_host_ip(&address, IPStr.c_str()) < 0) {
		cb->errors->createFatalError("Failed to convert string " + IPStr + " to IP address");
	}

	return address;
}
