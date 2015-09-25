#include "network/ComPacket.h"

namespace       Network
{

ComPacket::ComPacket()
{
    clear();
}

ComPacket::~ComPacket()
{

}

void        ComPacket::init()
{
  struct      s_ComPacketHeader header;
  this->getPacketHeader() = header;
  // std::cout << "ComPacket init:: magic = " << (char)this->getPacketHeader().magic << " -- packetsize == " << this->getPacketHeader().packetSize << " -- version == " << this->getPacketHeader().version << " -- header size == " << this->getPacketHeader().headerSize << std::endl;
}

}
