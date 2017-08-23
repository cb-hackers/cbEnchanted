#include "precomp.h"
#include "cbenchanted.h"
#include "enetinterface.h"
#include "type.h"
#include "errorsystem.h"
#include "meminterface.h"

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
	const void* memblockData = (void*)(cb->memInterface->getMemblock(cb->popValue().getInt()) + 4);

	ENetPacket* packet = enet_packet_create(memblockData, dataLength, flags);
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

void EnetInterface::getEnetPacketFlags(CBEnchanted *cb)
{
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);
	cb->pushValue(static_cast<int32_t>(packet->flags));
}

void EnetInterface::getEnetPacketData(CBEnchanted *cb)
{
	void* memblockData = (void*)(cb->memInterface->getMemblock(cb->popValue().getInt()) + 4);
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);
	memcpy(memblockData, (void*)packet->data, packet->dataLength);
	cb->pushValue(0);
}

void EnetInterface::setEnetPacketData(CBEnchanted *cb)
{
	const uint8_t* memblock = cb->memInterface->getMemblock(cb->popValue().getInt());
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);

	const size_t dataLength = static_cast<size_t>(*(int32_t*)memblock);

	uint8_t* data = new uint8_t[dataLength];
	memcpy(data, memblock + 4, dataLength);

	if (packet->data != NULL) enet_free(packet->data);
	packet->data = data;
	packet->dataLength = dataLength;
	cb->pushValue(0);
}

void EnetInterface::getEnetPacketDataLength(CBEnchanted *cb)
{
	ENetPacket* packet = getPacket(cb->popValue().getInt(), cb);
	cb->pushValue(static_cast<int32_t>(packet->dataLength));
}
