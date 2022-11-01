#include "posture.hh"

#include <cmath>
#include <string>

namespace lebai
{
  namespace posture
  {
    void JointPose::set_joint(const std::vector<double> &joint) { joint_ = joint; }
    const std::vector<double> &JointPose::joint() const { return joint_; }
    std::vector<double> *JointPose::mutable_joint() { return &joint_; }
    bool JointPose::Deserialize(const rapidjson::Value &obj)
    {
      joint_.clear();
      for (auto iter = obj["joint"].GetArray().Begin();
           iter != obj["joint"].GetArray().End(); iter++)
      {
        joint_.push_back(iter->GetDouble());
      }
      return true;
    }

    bool JointPose::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("joint");
      writer->StartArray();
      for (auto it = joint_.begin(); it != joint_.end(); ++it)
      {
        writer->Double(*it);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    bool JointPose::IsNullJSONData() const { return joint_.empty(); }

    void Position::set_x(double x) { x_ = x; }
    double Position::x() const { return x_; }
    double *Position::mutable_x() { return &x_; }
    void Position::set_y(double y) { y_ = y; }
    double Position::y() const { return y_; }
    double *Position::mutable_y() { return &y_; }
    void Position::set_z(double z) { z_ = z; }
    double Position::z() const { return z_; }
    double *Position::mutable_z() { return &z_; }
    bool Position::Deserialize(const rapidjson::Value &obj)
    {
      x_ = obj["x"].GetDouble();
      y_ = obj["y"].GetDouble();
      z_ = obj["z"].GetDouble();
      return true;
    }
    bool Position::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("x");
      writer->Double(x_);
      writer->String("y");
      writer->Double(y_);
      writer->String("z");
      writer->Double(z_);
      writer->EndObject();
      return true;
    }
    bool Position::IsNullJSONData() const
    {
      return std::abs(x_) < 1e-6 && std::abs(y_) < 1e-6 && std::abs(z_) < 1e-6;
    }

    void Quaternion::set_w(double w) { w_ = w; }
    double Quaternion::w() const { return w_; }
    double *Quaternion::mutable_w() { return &w_; }
    void Quaternion::set_i(double i) { i_ = i; }
    double Quaternion::i() const { return i_; }
    double *Quaternion::mutable_i() { return &i_; }
    void Quaternion::set_j(double j) { j_ = j; }
    double Quaternion::j() const { return j_; }
    double *Quaternion::mutable_j() { return &j_; }
    void Quaternion::set_k(double k) { k_ = k; }
    double Quaternion::k() const { return k_; }
    double *Quaternion::mutable_k() { return &k_; }
    bool Quaternion::Deserialize(const rapidjson::Value &obj)
    {
      w_ = obj["w"].GetDouble();
      i_ = obj["i"].GetDouble();
      j_ = obj["j"].GetDouble();
      k_ = obj["k"].GetDouble();
      return true;
    }
    bool Quaternion::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("w");
      writer->Double(w_);
      writer->String("i");
      writer->Double(i_);
      writer->String("j");
      writer->Double(j_);
      writer->String("k");
      writer->Double(k_);
      writer->EndObject();
      return true;
    }
    bool Quaternion::IsNullJSONData() const
    {
      return std::abs(w_ - 1.0) < 1e-6 && std::abs(i_) < 1e-6 &&
             std::abs(j_) < 1e-6 && std::abs(k_) < 1e-6;
    }

    void RotationMatrix::set_data(const std::array<double, 9> &data)
    {
      data_ = data;
    }
    const std::array<double, 9> &RotationMatrix::data() const { return data_; }
    std::array<double, 9> *RotationMatrix::mutable_data() { return &data_; }
    bool RotationMatrix::Deserialize(const rapidjson::Value &obj)
    {
      data_[0] = obj["m11"].GetDouble();
      data_[1] = obj["m12"].GetDouble();
      data_[2] = obj["m13"].GetDouble();
      data_[3] = obj["m21"].GetDouble();
      data_[4] = obj["m22"].GetDouble();
      data_[5] = obj["m23"].GetDouble();
      data_[6] = obj["m31"].GetDouble();
      data_[7] = obj["m32"].GetDouble();
      data_[8] = obj["m33"].GetDouble();
      return true;
    }
    bool RotationMatrix::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("m11");
      writer->Double(data_[0]);
      writer->String("m12");
      writer->Double(data_[1]);
      writer->String("m13");
      writer->Double(data_[2]);
      writer->String("m21");
      writer->Double(data_[3]);
      writer->String("m22");
      writer->Double(data_[4]);
      writer->String("m23");
      writer->Double(data_[5]);
      writer->String("m31");
      writer->Double(data_[6]);
      writer->String("m32");
      writer->Double(data_[7]);
      writer->String("m33");
      writer->Double(data_[8]);
      writer->EndObject();
      return true;
    }
    bool RotationMatrix::IsNullJSONData() const
    {
      return std::abs(data_[0] - 1) < 1e-6 && std::abs(data_[1]) < 1e-6 &&
             std::abs(data_[2]) < 1e-6 && std::abs(data_[3]) < 1e-6 &&
             std::abs(data_[4] - 1) < 1e-6 && std::abs(data_[5]) < 1e-6 &&
             std::abs(data_[6]) < 1e-6 && std::abs(data_[7]) < 1e-6 &&
             std::abs(data_[8] - 1) < 1e-6;
    }
    Rotation::Rotation() {}
    Rotation::Rotation(const Rotation &other)
    {
      if (other.matrix_)
      {
        matrix_ = std::make_unique<RotationMatrix>();
        *matrix_ = *other.matrix_;
      }
      else
      {
        matrix_.reset();
      }
      if (other.quaternion_)
      {
        quaternion_ = std::make_unique<Quaternion>();
        *quaternion_ = *other.quaternion_;
      }
      else
      {
        quaternion_.reset();
      }
      if (other.euler_zyx_)
      {
        euler_zyx_ = std::make_unique<Position>();
        *euler_zyx_ = *other.euler_zyx_;
      }
      else
      {
        euler_zyx_.reset();
      }
    }
    Rotation &Rotation::operator=(const Rotation &other)
    {
      if (other.matrix_)
      {
        matrix_ = std::make_unique<RotationMatrix>();
        *matrix_ = *other.matrix_;
      }
      else
      {
        matrix_.reset();
      }
      if (other.quaternion_)
      {
        quaternion_ = std::make_unique<Quaternion>();
        *quaternion_ = *other.quaternion_;
      }
      else
      {
        quaternion_.reset();
      }
      if (other.euler_zyx_)
      {
        euler_zyx_ = std::make_unique<Position>();
        *euler_zyx_ = *other.euler_zyx_;
      }
      else
      {
        euler_zyx_.reset();
      }
      return *this;
    }
    void Rotation::set_rotation_matrix(const RotationMatrix &matrix)
    {
      if (!matrix_)
      {
        matrix_ = std::make_unique<RotationMatrix>();
      }
      *matrix_ = matrix;
    }
    const RotationMatrix *Rotation::rotation_matrix() const
    {
      return matrix_.get();
    }
    RotationMatrix *Rotation::mutable_rotation_matrix()
    {
      if (!matrix_)
      {
        euler_zyx_.reset();
        quaternion_.reset();
        matrix_ = std::make_unique<RotationMatrix>();
      }
      return matrix_.get();
    }

    void Rotation::set_quaternion(const Quaternion &quaternion)
    {
      if (!quaternion_)
      {
        quaternion_ = std::make_unique<Quaternion>();
      }
      *quaternion_ = quaternion;
    }
    const Quaternion *Rotation::quaternion() const { return quaternion_.get(); }
    Quaternion *Rotation::mutable_quaternion()
    {
      if (!quaternion_)
      {
        quaternion_ = std::make_unique<Quaternion>();
      }
      return quaternion_.get();
    }
    void Rotation::set_euler_zyx(const Position &euler_zyx)
    {
      if (!euler_zyx_)
      {
        euler_zyx_ = std::make_unique<Position>();
      }
      *euler_zyx_ = euler_zyx;
    }
    const Position *Rotation::euler_zyx() const { return euler_zyx_.get(); }
    Position *Rotation::mutable_euler_zyx()
    {
      if (!euler_zyx_)
      {
        euler_zyx_ = std::make_unique<Position>();
      }
      return euler_zyx_.get();
    }
    bool Rotation::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("matrix"))
      {
        matrix_ = std::make_unique<RotationMatrix>();
        matrix_->Deserialize(obj["matrix"]);
      }
      else
      {
        matrix_.reset();
      }
      if (obj.HasMember("quaternion"))
      {
        quaternion_ = std::make_unique<Quaternion>();
        quaternion_->Deserialize(obj["quaternion"]);
      }
      else
      {
        quaternion_.reset();
      }
      if (obj.HasMember("euler_zyx"))
      {
        euler_zyx_ = std::make_unique<Position>();
        euler_zyx_->Deserialize(obj["euler_zyx"]);
      }
      else
      {
        euler_zyx_.reset();
      }
      return true;
    }
    bool Rotation::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (matrix_ != nullptr)
      {
        writer->String("matrix");
        matrix_->Serialize(writer);
      }
      if (quaternion_ != nullptr)
      {
        writer->String("quaternion");
        quaternion_->Serialize(writer);
      }
      if (euler_zyx_ != nullptr)
      {
        writer->String("euler_zyx");
        euler_zyx_->Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool Rotation::IsNullJSONData() const
    {
      if (matrix_ && !matrix_->IsNullJSONData())
      {
        return false;
      }
      if (quaternion_ && !quaternion_->IsNullJSONData())
      {
        return false;
      }
      if (euler_zyx_ && !euler_zyx_->IsNullJSONData())
      {
        return false;
      }
      return true;
    }

    void JointFrame::set_kind(Kind kind) { kind_ = kind; }
    JointFrame::Kind JointFrame::kind() const { return kind_; }
    JointFrame::Kind *JointFrame::mutable_kind() { return &kind_; }
    void JointFrame::set_joints(const JointPose &joints) { joints_ = joints; }
    const JointPose &JointFrame::joints() const { return joints_; }
    JointPose *JointFrame::mutable_joints() { return &joints_; }

    bool JointFrame::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("kind"))
      {
        kind_ = static_cast<Kind>(obj["kind"].GetInt());
      }
      if (obj.HasMember("joints"))
      {
        joints_.Deserialize(obj["joints"]);
      }
      return true;
    }

    bool JointFrame::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (kind_ != Kind::BASE)
      {
        writer->String("kind");
        writer->Int(static_cast<int>(kind_));
      }
      if (joints_.joint().size() > 0)
      {
        writer->String("joints");
        joints_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool JointFrame::IsNullJSONData() const
    {
      return joints_.IsNullJSONData() && kind_ == Kind::BASE;
    }

    void CartesianFrame::set_position_kind(Kind position_kind)
    {
      position_kind_ = position_kind;
    }
    CartesianFrame::Kind CartesianFrame::position_kind() const
    {
      return position_kind_;
    }
    CartesianFrame::Kind *CartesianFrame::mutable_position_kind()
    {
      return &position_kind_;
    }
    void CartesianFrame::set_position(const Position &position)
    {
      position_ = position;
    }
    const Position &CartesianFrame::position() const { return position_; }
    Position *CartesianFrame::mutable_position() { return &position_; }
    void CartesianFrame::set_rotation_kind(Kind rotation_kind)
    {
      rotation_kind_ = rotation_kind;
    }
    CartesianFrame::Kind CartesianFrame::rotation_kind() const
    {
      return rotation_kind_;
    }
    CartesianFrame::Kind *CartesianFrame::mutable_rotation_kind()
    {
      return &rotation_kind_;
    }
    void CartesianFrame::set_rotation(const Rotation &rotation)
    {
      rotation_ = rotation;
    }
    const Rotation &CartesianFrame::rotation() const { return rotation_; }
    Rotation *CartesianFrame::mutable_rotation() { return &rotation_; }
    bool CartesianFrame::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("position_kind"))
      {
        position_kind_ = static_cast<Kind>(obj["position_kind"].GetInt());
      }
      if (obj.HasMember("position"))
      {
        position_.Deserialize(obj["position"]);
      }
      if (obj.HasMember("rotation_kind"))
      {
        rotation_kind_ = static_cast<Kind>(obj["rotation_kind"].GetInt());
      }
      if (obj.HasMember("rotation"))
      {
        rotation_.Deserialize(obj["rotation"]);
      }
      return true;
    }
    bool CartesianFrame::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (position_kind_ != Kind::BASE)
      {
        writer->String("position_kind");
        writer->Int(static_cast<int>(position_kind_));
      }
      if (!position_.IsNullJSONData())
      {
        writer->String("position");
        position_.Serialize(writer);
      }
      if (rotation_kind_ != Kind::BASE)
      {
        writer->String("rotation_kind");
        writer->Int(static_cast<int>(rotation_kind_));
      }
      if (!rotation_.IsNullJSONData())
      {
        writer->String("rotation");
        rotation_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool CartesianFrame::IsNullJSONData() const
    {
      return position_.IsNullJSONData() && rotation_.IsNullJSONData() &&
             position_kind_ == Kind::BASE && rotation_kind_ == Kind::BASE;
    }

    void CartesianPose::set_position(const Position &position)
    {
      position_ = position;
    }
    const Position &CartesianPose::position() const { return position_; }
    Position *CartesianPose::mutable_position() { return &position_; }
    void CartesianPose::set_rotation(const Rotation &rotation)
    {
      rotation_ = rotation;
    }
    const Rotation &CartesianPose::rotation() const { return rotation_; }
    Rotation *CartesianPose::mutable_rotation() { return &rotation_; }
    bool CartesianPose::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("position"))
      {
        position_.Deserialize(obj["position"]);
      }
      if (obj.HasMember("rotation"))
      {
        rotation_.Deserialize(obj["rotation"]);
      }
      return true;
    }
    bool CartesianPose::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!position_.IsNullJSONData())
      {
        writer->String("position");
        position_.Serialize(writer);
      }
      if (!rotation_.IsNullJSONData())
      {
        writer->String("rotation");
        rotation_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool CartesianPose::IsNullJSONData() const
    {
      return position_.IsNullJSONData() && rotation_.IsNullJSONData();
    }

    // JointTargetPose begin
    void JointTargetPose::set_base(const JointFrame &base) { base_ = base; }
    const JointFrame &JointTargetPose::base() const { return base_; }
    JointFrame *JointTargetPose::mutable_base() { return &base_; }
    void JointTargetPose::set_delta(const JointPose &joints) { delta_ = joints; }
    const JointPose &JointTargetPose::delta() const { return delta_; }
    JointPose *JointTargetPose::mutable_delta() { return &delta_; }
    bool JointTargetPose::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("base"))
      {
        base_.Deserialize(obj["base"]);
      }
      if (obj.HasMember("delta"))
      {
        delta_.Deserialize(obj["delta"]);
      }
      return true;
    }
    bool JointTargetPose::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!base_.IsNullJSONData())
      {
        writer->String("base");
        base_.Serialize(writer);
      }
      if (!delta_.IsNullJSONData())
      {
        writer->String("delta");
        delta_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool JointTargetPose::IsNullJSONData() const
    {
      return base_.IsNullJSONData() && delta_.IsNullJSONData();
    }
    // JointTargetPose end

    // CartesianTargetPose begin
    void CartesianTargetPose::set_base(const CartesianFrame &base) { base_ = base; }
    const CartesianFrame &CartesianTargetPose::base() const { return base_; }
    CartesianFrame *CartesianTargetPose::mutable_base() { return &base_; }
    void CartesianTargetPose::set_delta(const CartesianPose &delta)
    {
      delta_ = delta;
    }
    const CartesianPose &CartesianTargetPose::delta() const { return delta_; }
    CartesianPose *CartesianTargetPose::mutable_delta() { return &delta_; }
    bool CartesianTargetPose::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("base"))
      {
        base_.Deserialize(obj["base"]);
      }
      if (obj.HasMember("delta"))
      {
        delta_.Deserialize(obj["delta"]);
      }
      return true;
    }
    bool CartesianTargetPose::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!base_.IsNullJSONData())
      {
        writer->String("base");
        base_.Serialize(writer);
      }
      if (!delta_.IsNullJSONData())
      {
        writer->String("delta");
        delta_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool CartesianTargetPose::IsNullJSONData() const
    {
      return base_.IsNullJSONData() && delta_.IsNullJSONData();
    }

    // Pose begin
    Pose &Pose::operator=(const Pose &other)
    {
      joint_.reset();
      cart_.reset();
      if (other.joint_)
      {
        joint_ = std::make_unique<JointTargetPose>();
        *joint_ = *other.joint_;
      }
      else if (other.cart_)
      {
        cart_ = std::make_unique<CartesianTargetPose>();
        *cart_ = *other.cart_;
      }
      return *this;
    }
    void Pose::set_joint(const JointTargetPose &joint)
    {
      cart_.reset();
      if (!joint_)
      {
        joint_ = std::make_unique<JointTargetPose>();
      }
      *joint_ = joint;
      // joint_->set_base(joint.base());
      // joint_->set_delta(joint.delta());
    }
    const JointTargetPose *Pose::joint() const { return joint_.get(); }
    JointTargetPose *Pose::mutable_joint()
    {
      if (!joint_)
      {
        cart_.reset();
        joint_ = std::make_unique<JointTargetPose>();
      }
      return joint_.get();
    }

    void Pose::set_cart(const CartesianTargetPose &cart)
    {
      joint_.reset();
      if (!cart_)
      {
        cart_ = std::make_unique<CartesianTargetPose>();
      }
      *cart_ = cart;
    }
    const CartesianTargetPose *Pose::cart() const { return cart_.get(); }
    CartesianTargetPose *Pose::mutable_cart()
    {
      if (!cart_)
      {
        joint_.reset();
        cart_ = std::make_unique<CartesianTargetPose>();
      }
      return cart_.get();
    }

    bool Pose::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("joint"))
      {
        if (!joint_)
        {
          joint_ = std::make_unique<JointTargetPose>();
        }
        cart_.reset();
        joint_->Deserialize(obj["joint"]);
      }
      else if (obj.HasMember("cart"))
      {
        if (!cart_)
        {
          cart_ = std::make_unique<CartesianTargetPose>();
        }
        joint_.reset();
        cart_->Deserialize(obj["cart"]);
      }

      return true;
    }

    bool Pose::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (joint_ && !joint_->IsNullJSONData())
      {
        writer->String("joint");
        joint_->Serialize(writer);
      }
      else if (cart_ && !cart_->IsNullJSONData())
      {
        writer->String("cart");
        cart_->Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool Pose::IsNullJSONData() const
    {
      if (!joint_ && !cart_)
      {
        return true;
      }
      if (joint_)
      {
        return joint_->IsNullJSONData();
      }
      else if (cart_)
      {
        return cart_->IsNullJSONData();
      }
      return true;
    }

    void PoseRequest::set_pose(const Pose &pose) { pose_ = pose; }
    const Pose &PoseRequest::pose() const { return pose_; }
    Pose *PoseRequest::mutable_pose() { return &pose_; }
    bool PoseRequest::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("pose"))
      {
        pose_.Deserialize(obj["pose"]);
      }
      return true;
    }
    bool PoseRequest::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!pose_.IsNullJSONData())
      {
        writer->String("pose");
        pose_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool PoseRequest::IsNullJSONData() const { return pose_.IsNullJSONData(); }
    
    void GetInverseKinRequest::set_pose(const Pose &pose) { pose_ = pose; }
    const Pose &GetInverseKinRequest::pose() const { return pose_; }
    Pose *GetInverseKinRequest::mutable_pose() { return &pose_; }

    void GetInverseKinRequest::set_refer(const JointPose& refer)
    {
      if(!refer_)
      {
        refer_ = std::make_unique<JointPose>();
      }
      *refer_ = refer;
    }
    const JointPose* GetInverseKinRequest::refer() const {
      if(!refer_)
      {
        return nullptr;
      }
      return refer_.get(); 
    }
    JointPose* GetInverseKinRequest::mutable_refer() {
      if(!refer_)
      {
        refer_ = std::make_unique<JointPose>();
      }
      return refer_.get();
    }
    bool GetInverseKinRequest::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("pose"))
      {
        pose_.Deserialize(obj["pose"]);
      }
      if (obj.HasMember("refer"))
      {
        if(!refer_)
        {
          refer_ = std::make_unique<JointPose>();
        }
        refer_->Deserialize(obj["refer"]);
      }
      return true;
    }
    bool GetInverseKinRequest::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!pose_.IsNullJSONData())
      {
        writer->String("pose");
        pose_.Serialize(writer);
      }
      if(refer_ && !refer_->IsNullJSONData())
      {
        writer->String("refer");
        refer_->Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool GetInverseKinRequest::IsNullJSONData() const { return false; }

    void GetPoseMultiplyRequest::set_base(const Pose &base) { base_ = base; }
    const Pose &GetPoseMultiplyRequest::base() const { return base_; }
    Pose *GetPoseMultiplyRequest::mutable_base() { return &base_; }

    void GetPoseMultiplyRequest::set_target(const Pose &target){ target_ = target; }
    const Pose &GetPoseMultiplyRequest::target() const { return target_; }
    Pose *GetPoseMultiplyRequest::mutable_target() { return &target_; }

    bool GetPoseMultiplyRequest::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("base"))
      {
        base_.Deserialize(obj["base"]);
      }
      if (obj.HasMember("target"))
      {
        target_.Deserialize(obj["target"]);
      }
      return true;
    }
    bool GetPoseMultiplyRequest::Serialize(
        rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      if (!base_.IsNullJSONData())
      {
        writer->String("base");
        base_.Serialize(writer);
      }
      if (!target_.IsNullJSONData())
      {
        writer->String("target");
        target_.Serialize(writer);
      }
      writer->EndObject();
      return true;
    }
    bool GetPoseMultiplyRequest::IsNullJSONData() const { return false; }
  } // namespace posture

} // namespace lebai