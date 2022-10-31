#pragma once

#include "jsonbase.hh"
#include <memory>
#include <vector>

namespace lebai
{
    namespace safety
    {
        class CollisionTorqueDiff : public JSONBase
        {
        public:
            void set_diffs(std::vector<double> diffs);
            std::vector<double> diffs();
            std::vector<double> * mutable_diffs();

        protected:
            std::vector<double> diffs_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}