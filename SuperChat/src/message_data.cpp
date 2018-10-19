#include "message_data.h"

void message_data::send ( message messageInstance )
{
	ReturnCode_t status = Writer->write(messageInstance, DDS::HANDLE_NIL);
	checkStatus(status, "Writer::write");
}

void message_data::recv ( message_list_t* messageList )
{
	// if any data is available, it returns a list
	assert ( messageList );
	messageSeq msgList;
	SampleInfoSeq infoSeq;
	ReturnCode_t status =  - 1;
	status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
	                     ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
	checkStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++)
	{
	messageList->push_back ( msgList[j] );
	}
	status = Reader->return_loan ( msgList, infoSeq );
	checkStatus(status, "MsgDataReader::return_loan");
}

message_data::message_data (char *topic_name)
{
	// participant
	em.createParticipant ("");

	// create types
	messageTypeSupport_var T = new messageTypeSupport ();
	em.registerType ( T.in() );

	// create Topic
	em.createTopic ( topic_name );

	// create Publisher
	em.createPublisher ();

	// create dataWriter
	em.createWriter ( false );
	DataWriter_var dwriter = em.getWriter ();
	Writer = messageDataWriter::_narrow ( dwriter.in() );

	//create Subscriber
	em.createSubscriber ();

	// create DataReader
	em.createReader ();
	DataReader_var dreader = em.getReader ();
	Reader = messageDataReader::_narrow( dreader.in () );
	checkHandle( Reader.in (), "MsgDataReader::_narrow" );
}

message_data::~message_data ()
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