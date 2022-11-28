
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include "jsonbase.hh"
#include "db.hh"

namespace lebai {
namespace posture {

/**
 * @brief Data to represent a joint array data.
 *
 */
class JointPose : public JSONBase {
 public:
  /**
   * @brief Set the joint array data.
   * @param joints The joint array data.
   */
  void set_joint(const std::vector<double>& joints);
  /**
   * @brief Return the const joint array object.
   * @return Immutable joint array data.
   */
  const std::vector<double>& joint() const;
  /**
   * @brief Return the joint array reference.
   *
   * @return Mutable joint array data.
   */
  std::vector<double>* mutable_joint();

 protected:
  std::vector<double> joint_; /*!< Joint array data */

  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class Position : public JSONBase {
 public:
  void set_x(double x);
  double x() const;
  double* mutable_x();
  void set_y(double y);
  double y() const;
  double* mutable_y();
  void set_z(double z);
  double z() const;
  double* mutable_z();

 protected:
  double x_ = 0.0; /*!< cartesian translation position data x */
  double y_ = 0.0; /*!< cartesian translation position data y */
  double z_ = 0.0; /*!< cartesian translation position data z */

  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class Quaternion : public JSONBase {
 public:
  // w, i, j, k
  void set_w(double w);
  double w() const;
  double* mutable_w();
  void set_i(double i);
  double i() const;
  double* mutable_i();
  void set_j(double j);
  double j() const;
  double* mutable_j();
  void set_k(double k);
  double k() const;
  double* mutable_k();

 protected:
  double w_ = 1.0; /*!< Quaternion data w */
  double i_ = 0.0; /*!< Quaternion data i */
  double j_ = 0.0; /*!< Quaternion data j */
  double k_ = 0.0; /*!< Quaternion data k */

  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};
class RotationMatrix : public JSONBase {
 public:
  void set_data(const std::array<double, 9>& data);
  const std::array<double, 9>& data() const;
  std::array<double, 9>* mutable_data();

 protected:
  std::array<double, 9> data_ = {{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};; /*!< Rotation matrix data */
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};
class Rotation : public JSONBase {
 public:
  Rotation();
  Rotation(const Rotation& other);
  Rotation& operator=(const Rotation& other);
  void set_rotation_matrix(const RotationMatrix& rotation_matrix);
  const RotationMatrix* rotation_matrix() const;
  RotationMatrix* mutable_rotation_matrix();
  void set_quaternion(const Quaternion& quaternion);
  const Quaternion* quaternion() const;
  Quaternion* mutable_quaternion();
  void set_euler_zyx(const Position& euler);
  const Position* euler_zyx() const;
  Position* mutable_euler_zyx();

 protected:
  std::unique_ptr<RotationMatrix> matrix_ = nullptr;
  std::unique_ptr<Quaternion> quaternion_ = nullptr;
  std::unique_ptr<Position> euler_zyx_ = nullptr;

 public:
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class JointFrame : public JSONBase {
 public:
  enum Kind {
    BASE = 0,
    CURRENT = 1,
    LAST = 11,
    CUSTOM = 99,
  };
  // Data
  void set_kind(Kind kind);
  Kind kind() const;
  Kind* mutable_kind();
  void set_joints(const JointPose& joints);
  const JointPose& joints() const;
  JointPose* mutable_joints();

 protected:
  Kind kind_;
  JointPose joints_;

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class CartesianFrame : public JSONBase {
 public:
  enum Kind {
    BASE = 0,
    FLANGE = 1,
    TCP = 2,
    LAST_FLANGE = 11,
    LAST_TCP = 12,
    CUSTOM = 99,
  };
  void set_position_kind(Kind kind);
  Kind position_kind() const;
  Kind* mutable_position_kind();
  void set_position(const Position& position);
  const Position& position() const;
  Position* mutable_position();
  void set_rotation_kind(Kind kind);
  Kind rotation_kind() const;
  Kind* mutable_rotation_kind();
  void set_rotation(const Rotation& rotation);
  const Rotation& rotation() const;
  Rotation* mutable_rotation();

 protected:
  Kind position_kind_ = Kind::BASE;
  Position position_;
  Kind rotation_kind_ = Kind::BASE;
  Rotation rotation_;

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class CartesianPose : public JSONBase {
 public:
  void set_position(const Position& position);
  const Position& position() const;
  Position* mutable_position();
  void set_rotation(const Rotation& rotation);
  const Rotation& rotation() const;
  Rotation* mutable_rotation();

 protected:
  Position position_;
  Rotation rotation_;

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

// // JointTargetPose
// class JointTargetPose : public JSONBase {
//  public:
//   // Data
//   void set_base(const JointFrame& base);
//   const JointFrame& base() const;
//   JointFrame* mutable_base();
//   void set_delta(const JointPose& joints);
//   const JointPose& delta() const;
//   JointPose* mutable_delta();

//  protected:
//   JointFrame base_;
//   JointPose delta_;

//  public:
//   // These methods are used to serialize and deserialize the class.
//   // They will not be wrapped in the SDK.
//   virtual bool Deserialize(const rapidjson::Value& obj);
//   virtual bool Serialize(
//       rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
//   virtual bool IsNullJSONData() const;
// };

// class CartesianTargetPose : public JSONBase {
//  public:
//   void set_base(const CartesianFrame& base);
//   const CartesianFrame& base() const;
//   CartesianFrame* mutable_base();
//   void set_delta(const CartesianPose& delta);
//   const CartesianPose& delta() const;
//   CartesianPose* mutable_delta();

//  protected:
//   CartesianFrame base_;
//   CartesianPose delta_;

//  public:
//   // These methods are used to serialize and deserialize the class.
//   // They will not be wrapped in the SDK.
//   virtual bool Deserialize(const rapidjson::Value& obj);
//   virtual bool Serialize(
//       rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
//   virtual bool IsNullJSONData() const;
// };
// Pose
class Pose : public JSONBase {
 public:
  enum Kind {
    CARTESIAN = 0,
    JOINT = 1,
  };
  Pose& operator=(const Pose& other);
  void set_kind(Kind kind);
  Kind kind() const;

  void set_joint(const JointPose& joint);
  JointPose* mutable_joint();  
  
  void set_cart(const CartesianPose& cartesian_pose);
  CartesianPose* mutable_cart();
  
  void set_cart_frame_index(const db::LoadRequest& cart_frame_index);
  db::LoadRequest* mutable_cart_frame_index();
  
  void set_cart_frame(const CartesianFrame& cart_frame);
  CartesianFrame* mutable_cart_frame();



 protected:
  Kind kind_;
  std::unique_ptr<JointPose> joint_ = nullptr;
  std::unique_ptr<CartesianPose> cart_ = nullptr;
  std::unique_ptr<db::LoadRequest> cart_frame_index_ = nullptr;
  std::unique_ptr<CartesianFrame> cart_frame_ = nullptr;
  

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};
class PoseRequest : public JSONBase {
 public:
  // Data
  void set_pose(const Pose& pose);
  const Pose& pose() const;
  Pose* mutable_pose();

 protected:
  Pose pose_;

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

class GetInverseKinRequest : public JSONBase {
public:
  // Data
  // pose
  void set_pose(const Pose& pose);
  const Pose& pose() const;
  Pose* mutable_pose();

  // refer
  void set_refer(const JointPose& refer);
  const JointPose * refer() const;
  JointPose * mutable_refer();

 protected:
  Pose pose_;
  std::unique_ptr<JointPose> refer_;


 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;  
};

class GetPoseMultiplyRequest : public JSONBase {
 public:
  // Data
  // base
  void set_base(const Pose& pose);
  const Pose& base() const;
  Pose* mutable_base();

  // target
  void set_target(const Pose& refer);
  const Pose & target() const;
  Pose* mutable_target();

 protected:
  Pose base_;
  Pose target_;

 public:
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
  virtual bool Deserialize(const rapidjson::Value& obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
  virtual bool IsNullJSONData() const;
};

}  // namespace posture
}  // namespace lebai