//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: SuperChat.h
//  Source: idl/SuperChat.idl
//  Generated: Sun Apr 23 20:59:25 2017
//  OpenSplice V6.4.140407OSS
//  
//******************************************************************
#ifndef _SUPERCHAT_H_
#define _SUPERCHAT_H_

#include "sacpp_mapping.h"
#include "sacpp_DDS_DCPS.h"


namespace SuperChat
{
   struct message;
   struct chatroom;
   struct user;
   const DDS::ULong MESSAGE_SIZE_MAX = (DDS::ULong) 144UL;
   const DDS::ULong CHATROOM_NAME_MAX = (DDS::ULong) 25UL;
   const DDS::ULong NICK_SIZE_MAX = (DDS::ULong) 8UL;
   const DDS::ULong CHATROOMS_MAX = (DDS::ULong) 10UL;
   const DDS::Float USER_UPDATE_PERIOD = (DDS::Float) 2.5;

   struct message
   {
         typedef DDS::Char _message_slice;
         typedef DDS::Char _message[144];
         typedef _message _message_out;
         static _message_slice * _message_alloc ();
         static void _message_free (_message_slice *);
         static void _message_copy (_message_slice* to, const _message_slice* from);
         static _message_slice *_message_dup (const _message_slice* from);

         struct _message_uniq_ {};
         typedef DDS_DCPS_FArray_var< _message, _message_slice, struct _message_uniq_> _message_var;
         typedef DDS_DCPS_Array_forany< _message, _message_slice, struct _message_uniq_> _message_forany;
         _message message;
         DDS::ULongLong uuid;
         DDS::ULongLong cksum;
         DDS::ULong chatroom_idx;
   };

   typedef DDS_DCPSStruct_var < message> message_var;
   typedef message&message_out;

   struct chatroom
   {
         typedef DDS::Char _chatroom_name_slice;
         typedef DDS::Char _chatroom_name[25];
         typedef _chatroom_name _chatroom_name_out;
         static _chatroom_name_slice * _chatroom_name_alloc ();
         static void _chatroom_name_free (_chatroom_name_slice *);
         static void _chatroom_name_copy (_chatroom_name_slice* to, const _chatroom_name_slice* from);
         static _chatroom_name_slice *_chatroom_name_dup (const _chatroom_name_slice* from);

         struct _chatroom_name_uniq_ {};
         typedef DDS_DCPS_FArray_var< _chatroom_name, _chatroom_name_slice, struct _chatroom_name_uniq_> _chatroom_name_var;
         typedef DDS_DCPS_Array_forany< _chatroom_name, _chatroom_name_slice, struct _chatroom_name_uniq_> _chatroom_name_forany;
         DDS::ULong chatroom_idx;
         _chatroom_name chatroom_name;
   };

   typedef DDS_DCPSStruct_var < chatroom> chatroom_var;
   typedef chatroom&chatroom_out;

   struct user
   {
         typedef DDS::Char _nick_slice;
         typedef DDS::Char _nick[8];
         typedef _nick _nick_out;
         static _nick_slice * _nick_alloc ();
         static void _nick_free (_nick_slice *);
         static void _nick_copy (_nick_slice* to, const _nick_slice* from);
         static _nick_slice *_nick_dup (const _nick_slice* from);

         struct _nick_uniq_ {};
         typedef DDS_DCPS_FArray_var< _nick, _nick_slice, struct _nick_uniq_> _nick_var;
         typedef DDS_DCPS_Array_forany< _nick, _nick_slice, struct _nick_uniq_> _nick_forany;
         _nick nick;
         DDS::ULongLong uuid;
         DDS::ULong chatroom_idx;
   };

   typedef DDS_DCPSStruct_var < user> user_var;
   typedef user&user_out;
}
template <>
SuperChat::message::_message_slice* DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::alloc ();
template <>
void DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::copy (SuperChat::message::_message_slice *to, const SuperChat::message::_message_slice* from);
template <>
void DDS_DCPS_ArrayHelper < SuperChat::message::_message, SuperChat::message::_message_slice, SuperChat::message::_message_uniq_>::free (SuperChat::message::_message_slice *ptr);
template <>
SuperChat::chatroom::_chatroom_name_slice* DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::alloc ();
template <>
void DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::copy (SuperChat::chatroom::_chatroom_name_slice *to, const SuperChat::chatroom::_chatroom_name_slice* from);
template <>
void DDS_DCPS_ArrayHelper < SuperChat::chatroom::_chatroom_name, SuperChat::chatroom::_chatroom_name_slice, SuperChat::chatroom::_chatroom_name_uniq_>::free (SuperChat::chatroom::_chatroom_name_slice *ptr);
template <>
SuperChat::user::_nick_slice* DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::alloc ();
template <>
void DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::copy (SuperChat::user::_nick_slice *to, const SuperChat::user::_nick_slice* from);
template <>
void DDS_DCPS_ArrayHelper < SuperChat::user::_nick, SuperChat::user::_nick_slice, SuperChat::user::_nick_uniq_>::free (SuperChat::user::_nick_slice *ptr);




#endif 