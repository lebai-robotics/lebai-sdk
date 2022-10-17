
#pragma once

#include "jsonbase.hh"
#include <memory>
#include <string>
#include <vector>

namespace lebai
{
    namespace signal
    {
        class SetSignalRequest : public JSONBase
		{
		public:
            void set_key(unsigned int key);
            unsigned int key() const;
            unsigned int * mutable_key();

            void set_value(int value);
            int value() const;
            int * mutable_value();

		protected:
            unsigned int key_;	  
            int value_;
            // These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

        class GetSignalRequest : public JSONBase
		{
		public:
            void set_key(unsigned int key);
            unsigned int key() const;
            unsigned int * mutable_key();

		protected:
            unsigned int key_;	  
            int value_;
            // These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

        class GetSignalResponse : public JSONBase
		{
		public:
            void set_value(int value);
            int value() const;
            int * mutable_value();

		protected:  
            int value_;
            // These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};
    }
}