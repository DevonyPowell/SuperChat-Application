#ifndef _USER_DATA_H
#define _USER_DATA_H

#include "DDSEntityManager.h"
#include "ccpp_SuperChat.h"

using namespace DDS;
using namespace SuperChat;

typedef std::vector<user> user_list_t;

class user_data
{
  private:
  	DDSEntityManager em;
  	userDataWriter_var Writer;
  	userDataReader_var Reader;

  public:
  	void send ( user userInstance );
  	void recv ( user_list_t* userList );
  	user_data (char *topic_name);
  	~user_data ();
};

#endif