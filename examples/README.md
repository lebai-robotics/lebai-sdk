# Examples

This directory keeps small examples for the public SDK surfaces across supported
languages.

## Robot

- C++: `robot/robot.cc`
- Python: `robot/robot.py`
- C#: `robot/RobotExample.cs`
- Java: `robot/RobotExample.java`

## Gripper

- C++: `gripper/gripper.cc`
- Python: `gripper/gripper.py`
- C#: `gripper/GripperExample.cs`
- Java: `gripper/GripperExample.java`

The gripper examples use direct RS485/Modbus RTU and require a real serial port.
They print usage and exit if no serial port argument is supplied.

## Discovery

- C++: `discovery/discovery.cc`
- Python: `discovery/discovery.py`
- C#: `discovery/DiscoveryExample.cs`
- Java: `discovery/DiscoveryExample.java`

LuaRobot and older motion/threading snippets are intentionally not kept as
public examples in v2.x. Use the Robot examples as the maintained starting
point.
