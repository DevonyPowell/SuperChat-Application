#include "user_data.h"

void user_data::send ( user userInstance )
{
	ReturnCode_t status = Writer->write(userInstance, DDS::HANDLE_NIL);
	checkStatus(status, "Writer::write");
}

void user_data::recv ( user_list_t* userList )
{
	// if any data is available, it returns a list
	assert ( userList );
	userSeq msgList;
	SampleInfoSeq infoSeq;
	ReturnCode_t status =  - 1;
	status = Reader->take ( msgList, infoSeq, LENGTH_UNLIMITED,
	                     ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE );
	checkStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++)
	{
		userList->push_back ( msgList[j] );
	}
	status = Reader->return_loan ( msgList, infoSeq );
	checkStatus(status, "MsgDataReader::return_loan");
}

user_data::user_data (char *topic_name)
{
	// participant
	em.createParticipant ("");

	// create types
	userTypeSupport_var T = new userTypeSupport ();
	em.registerType ( T.in() );

	// create Topic
	em.createTopic ( topic_name );

	// create Publisher
	em.createPublisher ();

	// create dataWriter
	em.createWriter ( false );
	DataWriter_var dwriter = em.getWriter ();
	Writer = userDataWriter::_narrow ( dwriter.in() );

	//create Subscriber
	em.createSubscriber ();

	// create DataReader
	em.createReader ();
	DataReader_var dreader = em.getReader ();
	Reader = userDataReader::_narrow( dreader.in () );
	checkHandle( Reader.in (), "MsgDataReader::_narrow" );
}

user_data::~user_data ()
{
	std::cout << "deconstructor" << '\n';
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