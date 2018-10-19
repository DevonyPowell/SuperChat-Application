#ifndef _CHATROOM_DATA_H
#define _CHATROOM_DATA_H

#include "DDSEntityManager.h"
#include "ccpp_SuperChat.h"

using namespace DDS;
using namespace SuperChat;

typedef std::vector<chatroom> chatroom_list_t;

class chatroom_data
{
  private:
  	DDSEntityManager em;
  	chatroomDataWriter_var Writer;
  	chatroomDataReader_var Reader;
  
  public:
  	void send ( chatroom messageInstance );
  	void recv ( chatroom_list_t* chatroomList );
  	chatroom_data (char *topic_name);
  	~chatroom_data ();
};
#endif 


