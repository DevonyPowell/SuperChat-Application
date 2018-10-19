#include "chatroom_data.h"


void chatroom_data::send ( chatroom messageInstance )
{
	ReturnCode_t status = Writer->write(messageInstance, DDS::HANDLE_NIL);
	checkStatus(status, "Writer::write");
}

void chatroom_data::recv ( chatroom_list_t* chatroomList )
{
	// if any data is available, it returns a list
	assert ( chatroomList );
	chatroomSeq msgList;
	SampleInfoSeq infoSeq;
	ReturnCode_t status =  - 1;
	status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
	                     ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
	checkStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++)
	{
		chatroomList->push_back ( msgList[j] );
	}
	status = Reader->return_loan ( msgList, infoSeq );
	checkStatus(status, "MsgDataReader::return_loan");
}

chatroom_data::chatroom_data (char *topic_name)
{
	// participant
	em.createParticipant ("");

	// create types
	chatroomTypeSupport_var T = new chatroomTypeSupport ();
	em.registerType ( T.in() );

	// create Topic
	em.createTopic ( topic_name );

	// create Publisher
	em.createPublisher ();

	// create dataWriter
	em.createWriter ( false );
	DataWriter_var dwriter = em.getWriter ();
	Writer = chatroomDataWriter::_narrow ( dwriter.in() );

	//create Subscriber
	em.createSubscriber ();

	// create DataReader
	em.createReader ();
	DataReader_var dreader = em.getReader ();
	Reader = chatroomDataReader::_narrow( dreader.in () );
	checkHandle( Reader.in (), "MsgDataReader::_narrow" );
}

chatroom_data::~chatroom_data ()
{
	// Remove the DataWriters.
	em.deleteWriter ();

	// Remove the DataReaders
	em.deleteReader ();

	// Remove the Publisher.
	em.deletePublisher ();

	// Remove the Subscriber.
	em.deleteSubscriber ();

	// Remove the Topics.
	em.deleteTopic ();

	// Remove Participant.
	em.deleteParticipant ();
}