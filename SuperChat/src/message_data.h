#ifndef _MESSAGE_DATA_H
#define _MESSAGE_DATA_H

#include "DDSEntityManager.h"
#include "ccpp_SuperChat.h"

using namespace DDS;
using namespace SuperChat;

typedef std::vector<message> message_list_t;

class message_data
{
  private:
    DDSEntityManager em;
    messageDataWriter_var Writer;
    messageDataReader_var Reader;

  public:
    void send ( message messageInstance );
    void recv ( message_list_t* messageList );
    message_data (char *topic_name);
    ~message_data ();
};

#endif 

