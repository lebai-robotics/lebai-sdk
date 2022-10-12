#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>


namespace lebai
{
	namespace led
	{
    enum LedMode
    {
      HOLD_LED = 0,
      CLOSE_LED	= 1,
      OPEN_LED = 2,
      BREATH = 3,
      FOUR = 4,
      WATER = 5,
      BLINK = 6,
    };
    enum LedSpeed
    {
      HOLD_LED_SPEED = 0,
      FAST = 1,
      NORMAL = 2,
      SLOW = 3,
    };
    enum LedColor
    {
      RED = 0,
      GREEN = 1,
      BLUE = 2,
      PINK = 3,
      YELLOW = 4,
      CYAN = 5,
      GRAY = 6,
      BROWN = 7,
      ORANGE = 8,
      GOLD = 9,
      INDIGO = 10,
      LIGHT_SKY_BLUE = 11,
      DARK_VIOLET = 12,
      CHOCOLATE	= 13,
      LIGHT_RED = 14,
      WHITE = 15,
    };

		class LedData : public JSONBase
		{
		public:
      void set_mode(LedMode mode);
      LedMode mode() const;
      LedMode * mutable_mode();

      void set_speed(LedSpeed speed);
      LedSpeed speed() const;
      LedSpeed * mutable_speed();

      void set_colors(const std::vector<LedColor> & colors);
      const std::vector<LedColor> & colors() const;
      std::vector<LedColor> * mutable_colors();

		protected:
      LedMode	 mode_;	  
      LedSpeed speed_;
      std::vector<LedColor> colors_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};
	}
}