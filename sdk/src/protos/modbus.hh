#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>


namespace lebai
{
	namespace modbus
    {
        class SetCoilsRequest : public JSONBase
        {
        public:
            void set_device(std::string device);
            std::string device() const;
            std::string *mutable_device();

            void set_pin(std::string pin);
            std::string pin() const;
            std::string *mutable_pin();

            void set_values(std::vector<bool> values);
            std::vector<bool> values() const;
            std::vector<bool> *mutable_values();

		protected:
            std::string device_;
            std::string pin_;
            std::vector<bool> values_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}