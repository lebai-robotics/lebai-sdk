# include "jsonbase.hh"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace lebai
{


std::string JSONBase::ToJSONString() const
{
	rapidjson::StringBuffer ss;
	rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use writer for condensed formatting
	if (Serialize(&writer))
	{
		return std::string(ss.GetString());
	}
	return "";
}

// std::string JSONBase::ToJSONRpcReqString(const std::string & method, int id) const
// {
// 	rapidjson::StringBuffer ss;
// 	rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use Writer for condensed formatting
// 	writer.StartObject();
// 	writer.String("jsonrpc");
// 	writer.String("2.0");
// 	writer.String("method");
// 	writer.String(method.c_str());
// 	writer.String("params");
// 	Serialize(&writer);
// 	writer.String("id");
// 	writer.Int(id);
// 	writer.EndObject();
// 	return std::string(ss.GetString());
// }

bool JSONBase::FromJSONString(const std::string& s)
{
	rapidjson::Document doc;
	if (!InitDocument(s, doc))
		return false;

	Deserialize(doc);

	return true;
}

// bool JSONBase::FromJSONRpcRespString(const std::string &s)
// {
// 	rapidjson::Document doc;
// 	if (!InitDocument(s, doc))
// 		return false;

// 	if (doc.HasMember("error"))
// 	{
// 		// Error code and message.
// 		int error_id = doc["error"]["code"].GetInt();
// 		std::string error_msg = doc["error"]["message"].GetString();
// 		std::cout << "Error: " << error_id << " " << error_msg << std::endl;
// 		return false;
// 	}

// 	if (doc.HasMember("result"))
// 	{
// 		Deserialize(doc["result"]);
// 	}

// 	return true;
// }

// const char * JSONBase::ToString() const
// {
// 	return ToJSONString().c_str();
// }

bool JSONBase::InitDocument(const std::string& s, rapidjson::Document& doc)
{
	if (s.empty())
		return false;

	std::string validJson(s);

	return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
}
}