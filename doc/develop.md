# Develop文档

## 查看proto定义
lebai-sdk依赖的RPC定义可以通过[lebai-proto](https://lebai-robotics.github.io/lebai-proto/)查看。

## 如何实现一个RPC

本部分会给出一个根据[lebai-proto](https://lebai-robotics.github.io/lebai-proto/)文档添加数据结构和`RPC`实现的基本流程。

以下以`MoveJoint`关节空间运动为例说明如果添加RPC

### 添加数据结构

查阅[lebai-proto](https://lebai-robotics.github.io/lebai-proto/)可以看到`MoveJoint`定义如下

![image-20230109095057824](D:\work\lebai\rc\workspace\lebai-sdk\doc\develop.assets\image-20230109095057824.png)

可以看出该`RPC`的输入为`MoveRequest`，输出为`MotionIndex`

可以根据[lebai-proto](https://lebai-robotics.github.io/lebai-proto/)进一步查看该接口输入[motion.proto](https://lebai-robotics.github.io/lebai-proto/#motion.proto).

因此在项目`sdk\src\protos\motion.hh`中添加`MoveRequest`数据结构:

```

	class MoveRequest : public JSONBase
	{
	public:
		// pose_via_
		void set_pose_via(const posture::Pose & pose);
		const posture::Pose & pose_via() const;
		posture::Pose * mutable_pose_via();
		// pose_
		void set_pose(const posture::Pose & pose);
		const posture::Pose & pose() const;
		posture::Pose * mutable_pose();
		// param_
		void set_param(const MoveParam & param);
		const MoveParam & param() const;
		MoveParam * mutable_param();
		// rad_ 
		void set_rad(double rad);
		double rad() const;
		double * mutable_rad();

	protected:
		posture::Pose pose_via_;
		posture::Pose pose_;
		double rad_;
		MoveParam param_;
	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;		
	};
```

所有的`RPC`数据结构都应该继承自`JSONBase`并实现如下三个方法

```
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;	
```

`Deserialize`是将JSON数据转换为`MoveRequest`数据结构。

`Serialize`是将`MoveRequest`数据结构转换为JSON数据。

`IsNullJSONData`返回该数据是否为空。

在项目`sdk\src\protos\motion.cc`中添加`MoveRequest`的相关实现:

```
  // MoveRequest begin
  void MoveRequest::set_param(const MoveParam & param)
  {
    param_ = param;
  }	
  const MoveParam & MoveRequest::param() const
  {
    return param_;
  }
  MoveParam * MoveRequest::mutable_param()
  {
    return &param_;
  }

  void MoveRequest::set_pose(const posture::Pose & pose)
  {
    pose_ = pose;
  }
  const posture::Pose & MoveRequest::pose() const
  {
    return pose_;
  }
  posture::Pose * MoveRequest::mutable_pose()
  {
    return &pose_;
  }
  
  bool MoveRequest::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("param"))
    {
      param_.Deserialize(obj["param"]);
    }
    if(obj.HasMember("pose"))
    {
      pose_.Deserialize(obj["pose"]);
    }    
    return true;    
  }

  bool MoveRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    if(!param_.IsNullJSONData())
    {
      writer->String("param");
      param_.Serialize(writer);
    }
    if(!pose_.IsNullJSONData())
    {
      writer->String("pose");
      pose_.Serialize(writer);
    }
    writer->EndObject();
    return true;
  }
  bool MoveRequest::IsNullJSONData() const
  {
    if(param_.IsNullJSONData() && pose_.IsNullJSONData())
    {
      return true;
    }
    return false;
  }
  // MoveRequest end
```

可以看到`MoveRequest`中使用了一些嵌套的数据结构`posture::Pose`，在开发过程中可以复用这些数据结构。

同样的，还需要添加返回数据类型的实现，在项目`sdk\src\protos\motion.hh`中添加`MotionIndex`数据结构:

```
	class MotionIndex : public JSONBase
	{
	public:
		void set_id(unsigned int id);
		unsigned int id() const;
		unsigned int * mutable_id();
	protected:
		unsigned int id_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};
```

在项目`sdk\src\protos\motion.cc`中添加`MotionIndex`数据结构实现:

```

  void MotionIndex::set_id(unsigned int id)
  {
    id_ = id;
  }
  unsigned int MotionIndex::id() const
  {
    return id_;
  }
  unsigned int *MotionIndex::mutable_id()
  {
    return &id_;
  }

  bool MotionIndex::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("id"))
    {
      unsigned int id_int = (unsigned int)(obj["id"].GetUint());
      id_ = id_int;
    }
    return true;
  }
  bool MotionIndex::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    writer->Key("id");
    writer->Uint(id_);
    writer->EndObject();
    return true;    
  }
  bool MotionIndex::IsNullJSONData() const
  {
    return false;
  }
```

至此已经添加了相关的数据结构实现，可以开始实现`RPC`.

### 添加`RPC`实现

在`sdk/include/lebai/robot.hh`中根据API的设计添加接口

```
int movej(const std::vector<double> & joint_positions, double a, double v, double t, double r);
```

在`sdk/src/robot.cc`中添加对应的实现

```
int Robot::movej(const std::vector<double> & joint_positions, double a, double v, double t, double r)
{
  motion::MoveRequest move_req;
  move_req.mutable_param()->set_acc(a);
  move_req.mutable_param()->set_velocity(v);
  move_req.mutable_param()->set_time(t);
  move_req.mutable_param()->set_radius(r);
  motion::MotionIndex resp;
  for(auto && p :joint_positions)
  {
    move_req.mutable_pose()->mutable_joint()->mutable_joint()->push_back(p);
  }
  resp = impl_->moveJoint(move_req);
  return resp.id();
}
```

该实现首先生成根据函数入参生成`MoveRequest`数据结构，然后调用`JSONRPC`实现`impl_->moveJoint(move_req)`（后面我们会看到如何实现），最后获取`JSONRPC`的实现，然后生成返回数据结构`MotionIndex`并返回.

在`sdk/src/robot_impl.hh`中添加对应的`JSONRPC`函数定义。

```
motion::MotionIndex moveJoint(const motion::MoveRequest & req);
```

在`sdk/src/robot_impl.cc`中添加对应的`JSONRPC`函数实现。

```
  motion::MotionIndex Robot::RobotImpl::moveJoint(const motion::MoveRequest & req)
  {
    std::string resp;
    json_rpc_connector_->CallRpc("move_joint", req.ToJSONString(), &resp);
    motion::MotionIndex motion_resp;
    motion_resp.FromJSONString(resp);
    return motion_resp;
  }
```

该实现中，将`MoveReuqest`转换为`JSON`字符串，然后通过相应的网络通讯接口，然后获取返回字符串，再将其转换为`MotionIndex`并且返回。

至此，`MoveJoint`实现完成







