#include "client.h"
#include "connector.h"
#include "easy_base_type.h"
#include "login.pb.h"
#include "msg.h"
#include "easy_time.h"
#include "easy_byte_buffer.h"

Client* Client::inst_ = easy_null; 

Client::Client()
{
	connector_login_ = easy_null;
	connector_proxy_ = easy_null;
	proxy_port_ = 0;
	login_port_ = 0;
}

Client::~Client()
{

}

Client* Client::instance()
{
	if (!inst_)
	{
		inst_ = new Client();
	}
	return inst_;
}

void Client::destroy()
{
	if(inst_)
	{
		delete inst_;
		inst_ = easy_null;
	}
}

easy_bool Client::connect_login(const easy_char* __host, easy_uint32 __port)
{
	if (!__host)
	{
		return false;
	}
	login_host_ = __host;
	login_port_ = __port;
	connector_login_ = new Connector(__host,login_port_);
	return true;
}

easy_bool Client::connect_proxy( const easy_char* __host, easy_uint32 __port )
{
	if (!__host)
	{
		return false;
	}
	proxy_host_ = __host;
	proxy_port_ = __port;
	connector_proxy_ = new Connector(__host,__port);
	return true;
}

void Client::send_login_msg()
{
	easy_uint16 __length = 0;
	easy_int32 __pakcet_id = MSG_C2L_LOGIN;
	login::c2l_login __c2l_login;
	__c2l_login.set_msg_id(__pakcet_id);
	__c2l_login.set_user_name("lee");
	__c2l_login.set_user_pwd("lee");
	std::string __string_login;
	__c2l_login.SerializeToString(&__string_login);
	__length = __string_login.length();
	connector_login_->write((const easy_char*)&__length,sizeof(easy_uint16));
	connector_login_->write(__string_login);
}

void Client::send_test_msg()
{
	static std::string __random_string[] = 
	{
		"[0x000085e4][T]AdvertisingIndentitifer: '', IdentifierForVendor: '', DeviceName: 'PC', ModelName: 'x86', SystemName: '', SystemVersion: '', HardwareID: '74d435046509'",
		"nice to meet you!",
		"It is the tears of the earth that keep here smiles in bloom.",
		"The mighty desert is burning for the love of a blade of grass who shakes her head and laughs and flies away.",
		"If you shed tears when you miss the sun, you also miss the stars.",
		"Her wishful face haunts my dreams like the rain at night.",
		"Once we dreamt that we were strangers.We wake up to find that we  were dear to each other.",
		"Sorrow is hushed into peace in my heart like the evening among the silent trees.",
		"Some unseen fingers, like an idle breeze, are playing upon my heart the music of the ripples.",
		"Listen, my heart, to the whispers of the world with which it makes love to you.",
		"Do not seat your love upon a precipice because it is high.",
		"I sit at my window this morning where the world like a passer-by stops for a moment, nods to me and goes.",
		"There little thoughts are the rustle of leaves; they have their whisper of joy in my mind.",
		"What you are you do not see, what you see is your shadow.",
		"My wishes are fools, they shout across thy song, my Master.Let me but listen.",
		"They throw their shadows before them who carry their lantern on their back.",
		"That I exist is  a perpetual surprise which is life.",
		"We, the rustling leaves, have a voice that answers the storms,but who are you so silent?I am a mere flower.",
		"Do not blame your food because you have no appetite.",
		"Success is not final, failure is not fatal: it is the courage to continue that counts.",
		"I cannot tell why this heart languishes in silence.It is for small needs it never asks, or knows or remembers.",
		"The bird wishes it were a cloud.The cloud wishes it were a bird."
	};

	static int __random_string_size = 22;
	srand(easy::EasyTime::get_cur_sys_time());
	int __random_index = rand()%__random_string_size;
	std::string __context = __random_string[__random_index];
	unsigned short __length = __context.size();
	static const easy_int32 __head_size = sizeof(easy_uint16);
#if 0
	static const int __data_length = 256;
	unsigned char __data[__data_length] = {};
	memcpy(__data,&__length,__head_size);
	memcpy(__data + __head_size,__context.c_str(),__length);
	connector_proxy_->write((char*)__data,__length + __head_size);
#else
	easy::EasyByteBuffer	__byte_buffer;
	__byte_buffer << __length;
	__byte_buffer << __context;
	connector_proxy_->write((char*)__byte_buffer.contents(),__byte_buffer.size());
#endif
}
