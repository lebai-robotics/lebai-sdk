#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>

namespace lebai
{
	namespace io
	{

		enum IoDevice
		{
			ROBOT = 0,
			FLANGE = 1,
			EXTRA = 2,
			SHOULDER = 3,
			FLANGE_BTN = 4,
		};
		enum IoPinKind
		{
			DI = 0,
			DO = 1,
			AI = 2,
			AO = 3,
		};
		/**
		 * @brief IO related Data .
		 *
		 */
		class GetDioPinRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

		protected:
			IoDevice device_;
			unsigned int pin_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};
		class GetDioPinResponse : public JSONBase
		{
		public:
			void set_value(unsigned int pin);
			unsigned int value() const;
			unsigned int *mutable_value();

		protected:
			unsigned int value_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class SetDoPinRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_value(unsigned int pin);
			unsigned int value() const;
			unsigned int *mutable_value();

		protected:
			IoDevice device_;
			unsigned int pin_;
			unsigned int value_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class GetAioPinRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

		protected:
			IoDevice device_;
			unsigned int pin_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class GetAioPinResponse : public JSONBase
		{
		public:
			void set_value(double pin);
			double value() const;
			double *mutable_value();

		protected:
			double value_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class SetAoPinRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_value(double pin);
			double value() const;
			double *mutable_value();

		protected:
			IoDevice device_;
			unsigned int pin_;
			double value_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class GetDioPinsRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_count(unsigned int count);
			unsigned int count() const;
			unsigned int *mutable_count();
		protected:
			IoDevice device_;
			unsigned int pin_;
			unsigned int count_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};
		class GetDioPinsResponse : public JSONBase
		{
		public:
			void set_values(std::vector<unsigned int> pin);
			std::vector<unsigned int> values() const;
			std::vector<unsigned int> *mutable_values();

		protected:
			std::vector<unsigned int> values_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class SetDoPinsRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_values(std::vector<unsigned int> pin);
			std::vector<unsigned int> values() const;
			std::vector<unsigned int> *mutable_values();

		protected:
			IoDevice device_;
			unsigned int pin_;
			std::vector<unsigned int> values_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class GetAioPinsRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_count(unsigned int count);
			unsigned int count() const;
			unsigned int *mutable_count();
		protected:
			IoDevice device_;
			unsigned int pin_;
			unsigned int count_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class GetAioPinsResponse : public JSONBase
		{
		public:
			void set_values(std::vector<double> values);
			std::vector<double> values() const;
			std::vector<double> *mutable_values();

		protected:
			std::vector<double> values_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		class SetAoPinsRequest : public JSONBase
		{
		public:
			void set_device(IoDevice device);
			IoDevice device() const;
			IoDevice *mutable_device();

			void set_pin(unsigned int pin);
			unsigned int pin() const;
			unsigned int *mutable_pin();

			void set_values(std::vector<double> pin);
			std::vector<double> values() const;
			std::vector<double> *mutable_values();

		protected:
			IoDevice device_;
			unsigned int pin_;
			std::vector<double> values_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};
	}
}