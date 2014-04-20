#include "action-ack.hh"

ActionAck::ActionAck(int player)
    : player_(player)
{
}

ActionAck::ActionAck()
    : player_(-1)
{
}

void ActionAck::handle_buffer(utils::Buffer& buf)
{
    buf.handle(player_);
}
