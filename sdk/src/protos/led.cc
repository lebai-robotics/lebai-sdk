#include <string>
#include "led.hh"


namespace lebai
{
  namespace led
  {
    void LedData::set_mode(LedMode mode)
    {
      mode_ = mode;
    }
    LedMode LedData::mode() const
    {
      return mode_;
    }
    LedMode * LedData::mutable_mode()
    {
      return &mode_;
    }

    // set_speed
    void LedData::set_speed(LedSpeed speed)
    {
      speed_ = speed;
    }
    LedSpeed LedData::speed() const
    {
      return speed_;
    }
    LedSpeed * LedData::mutable_speed()
    {
      return &speed_;
    }
    void LedData::set_colors(const std::vector<LedColor> & colors)
    {
      colors_ = colors;
    }
    const std::vector<LedColor> & LedData::colors() const
    {
      return colors_;
    }
    std::vector<LedColor> * LedData::mutable_colors()
    {
      return &colors_;
    }
    // Deserialize
    bool LedData::Deserialize(const rapidjson::Value& obj)
    {
      if (obj.HasMember("mode"))
      {
        std::string mode_str = std::string(obj["mode"].GetString());
        if(mode_str == "HOLD_LED")
        {
          mode_ = HOLD_LED;
        }
        else if(mode_str == "CLOSE_LED")
        {
          mode_ = CLOSE_LED;
        }
        else if(mode_str == "OPEN_LED")
        {
          mode_ = OPEN_LED;
        }
        else if(mode_str == "BREATH")
        {
          mode_ = BREATH;
        }
        else if(mode_str == "FOUR")
        {
          mode_ = FOUR;
        }
        else if(mode_str == "WATER")
        {
          mode_ = WATER;
        }
        else if(mode_str == "BLINK")
        {
          mode_ = BLINK;
        }
      }
      if (obj.HasMember("speed"))
      {
        std::string speed_str = std::string(obj["speed"].GetString());
        if(speed_str == "HOLD_LED_SPEED")
        {
          speed_ = HOLD_LED_SPEED;
        }
        else if(speed_str == "FAST")
        {
          speed_ = FAST;
        }
        else if(speed_str == "NORMAL")
        {
          speed_ = NORMAL;
        }
        else if(speed_str == "SLOW")
        {
          speed_ = SLOW;
        }
      }
      if (obj.HasMember("colors"))
      {
        const rapidjson::Value& colors = obj["colors"];
        for (rapidjson::SizeType i = 0; i < colors.Size(); i++)
        {
          std::string color_str = std::string(colors[i].GetString());
          if(color_str == "RED")
          {
            colors_.push_back(RED);
          }
          else if(color_str == "GREEN")
          {
            colors_.push_back(GREEN);
          }
          else if(color_str == "BLUE")
          {
            colors_.push_back(BLUE);
          }
          else if(color_str == "PINK")
          {
            colors_.push_back(PINK);
          }
          else if(color_str == "YELLOW")
          {
            colors_.push_back(YELLOW);
          }
          else if(color_str == "CYAN")
          {
            colors_.push_back(CYAN);
          }
          else if(color_str == "GRAY")
          {
            colors_.push_back(GRAY);
          }
          else if(color_str == "BROWN")
          {
            colors_.push_back(BROWN);
          }
          else if(color_str == "ORANGE")
          {
            colors_.push_back(ORANGE);
          }
          else if(color_str == "GOLD")
          {
            colors_.push_back(GOLD);
          }
          else if(color_str == "INDIGO")
          {
            colors_.push_back(INDIGO);
          }
          else if(color_str == "LIGHT_SKY_BLUE")
          {
            colors_.push_back(LIGHT_SKY_BLUE);
          }
          else if(color_str == "DARK_VIOLET")
          {
            colors_.push_back(DARK_VIOLET);
          }
          else if(color_str == "CHOCOLATE")
          {
            colors_.push_back(CHOCOLATE);
          }
          else if(color_str == "LIGHT_RED")
          {
            colors_.push_back(LIGHT_RED);
          }
          else if(color_str == "WHITE")
          {
            colors_.push_back(WHITE);
          }
        }
      }
      return true;
    }
    // Serialize
    bool LedData::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("mode");
      writer->Int(mode_);
      writer->String("speed");
      writer->Int(speed_);
      writer->String("colors");
      writer->StartArray();
      for (auto& color : colors_)
      {
        writer->Int(color);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }

    bool LedData::IsNullJSONData() const
    {
      return false;
    }   
  }
}