using System;
using System.Collections.Generic;

namespace lebai
{
    /// <summary>
    /// Idiomatic C# facade for the Lebai robot controller.
    /// </summary>
    public sealed class Robot : IDisposable
    {
        private readonly l_master.Robot native;

        public Robot(string ip, bool simulator)
        {
            native = new l_master.Robot(ip, simulator);
        }

        public Robot(string ip)
        {
            native = new l_master.Robot(ip);
        }

        public l_master.Robot Native
        {
            get { return native; }
        }

        public void Dispose()
        {
            native.Dispose();
        }

        public void StartSys()
        {
            native.start_sys();
        }

        public void StopSys()
        {
            native.stop_sys();
        }

        public void Stop()
        {
            native.stop();
        }

        public void EStop()
        {
            native.estop();
        }

        public void StartTeachMode()
        {
            native.start_teach_mode();
        }

        public void EndTeachMode()
        {
            native.end_teach_mode();
        }

        public void PauseMove()
        {
            native.pause_move();
        }

        public void ResumeMove()
        {
            native.resume_move();
        }

        public void Reboot()
        {
            native.reboot();
        }

        public void WaitMove()
        {
            native.wait_move();
        }

        public void WaitMove(uint id)
        {
            native.wait_move(id);
        }

        public int MoveJoint(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.move_joint(joints, a, v, t, r);
            }
        }

        public int MoveJ(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.movej(joints, a, v, t, r);
            }
        }

        public int MoveLinear(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.move_linear(joints, a, v, t, r);
            }
        }

        public int MoveL(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.movel(joints, a, v, t, r);
            }
        }

        public int MoveCircular(
            IEnumerable<double> jointVia,
            IEnumerable<double> joint,
            double rad,
            double a,
            double v,
            double t,
            double r)
        {
            using (var via = ToDoubleVector(jointVia))
            using (var target = ToDoubleVector(joint))
            {
                return native.move_circular(via, target, rad, a, v, t, r);
            }
        }

        public int MoveC(
            IEnumerable<double> jointVia,
            IEnumerable<double> joint,
            double rad,
            double a,
            double v,
            double t,
            double r)
        {
            using (var via = ToDoubleVector(jointVia))
            using (var target = ToDoubleVector(joint))
            {
                return native.movec(via, target, rad, a, v, t, r);
            }
        }

        public int SpeedJoint(double a, IEnumerable<double> speed, double t)
        {
            using (var values = ToDoubleVector(speed))
            {
                return native.speed_joint(a, values, t);
            }
        }

        public int SpeedJoint(double a, IEnumerable<double> speed)
        {
            using (var values = ToDoubleVector(speed))
            {
                return native.speed_joint(a, values);
            }
        }

        public int SpeedJ(double a, IEnumerable<double> speed, double t)
        {
            using (var values = ToDoubleVector(speed))
            {
                return native.speedj(a, values, t);
            }
        }

        public int SpeedJ(double a, IEnumerable<double> speed)
        {
            using (var values = ToDoubleVector(speed))
            {
                return native.speedj(a, values);
            }
        }

        public int TowardJoint(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.toward_joint(joints, a, v, t, r);
            }
        }

        public int TowardJ(IEnumerable<double> jointPositions, double a, double v, double t, double r)
        {
            using (var joints = ToDoubleVector(jointPositions))
            {
                return native.towardj(joints, a, v, t, r);
            }
        }

        public void MovePvat(IEnumerable<double> p, IEnumerable<double> v, IEnumerable<double> a, double t)
        {
            using (var positions = ToDoubleVector(p))
            using (var speeds = ToDoubleVector(v))
            using (var accelerations = ToDoubleVector(a))
            {
                native.move_pvat(positions, speeds, accelerations, t);
            }
        }

        public double[] GetActualJointPositions()
        {
            return ToArray(native.get_actual_joint_positions());
        }

        public double[] GetTargetJointPositions()
        {
            return ToArray(native.get_target_joint_positions());
        }

        public double[] GetActualJointSpeed()
        {
            return ToArray(native.get_actual_joint_speed());
        }

        public double[] GetTargetJointSpeed()
        {
            return ToArray(native.get_target_joint_speed());
        }

        public double[] GetActualJointTorques()
        {
            return ToArray(native.get_actual_joint_torques());
        }

        public double[] GetTargetJointTorques()
        {
            return ToArray(native.get_target_joint_torques());
        }

        public uint[] GetDos(string device, uint pin, uint count)
        {
            return ToArray(native.get_dos(device, pin, count));
        }

        public void SetDos(string device, uint pin, IEnumerable<uint> values)
        {
            using (var vector = ToUintVector(values))
            {
                native.set_dos(device, pin, vector);
            }
        }

        public uint[] GetDis(string device, uint pin, uint count)
        {
            return ToArray(native.get_dis(device, pin, count));
        }

        public double[] GetAos(string device, uint pin, uint count)
        {
            return ToArray(native.get_aos(device, pin, count));
        }

        public void SetAos(string device, uint pin, IEnumerable<double> values)
        {
            using (var vector = ToDoubleVector(values))
            {
                native.set_aos(device, pin, vector);
            }
        }

        public double[] GetAis(string device, uint pin, uint count)
        {
            return ToArray(native.get_ais(device, pin, count));
        }

        public void SetLed(uint mode, uint speed, IEnumerable<uint> color)
        {
            using (var vector = ToUintVector(color))
            {
                native.set_led(mode, speed, vector);
            }
        }

        public int[] GetSignals(uint index, uint length)
        {
            return ToArray(native.get_signals(index, length));
        }

        public void SetSignals(uint index, IEnumerable<int> values)
        {
            using (var vector = ToIntVector(values))
            {
                native.set_signals(index, vector);
            }
        }

        public string[] GetBoxDevices(string prefix)
        {
            return ToArray(native.get_box_devices(prefix));
        }

        public string[] LoadTcpList(string dir)
        {
            return ToArray(native.load_tcp_list(dir));
        }

        public string[] LoadTcpList()
        {
            return ToArray(native.load_tcp_list());
        }

        public string[] LoadPoseList(string dir)
        {
            return ToArray(native.load_pose_list(dir));
        }

        public string[] LoadPoseList()
        {
            return ToArray(native.load_pose_list());
        }

        public string[] LoadFrameList(string dir)
        {
            return ToArray(native.load_frame_list(dir));
        }

        public string[] LoadFrameList()
        {
            return ToArray(native.load_frame_list());
        }

        public string[] LoadTrajectoryList(string dir)
        {
            return ToArray(native.load_trajectory_list(dir));
        }

        public string[] LoadTrajectoryList()
        {
            return ToArray(native.load_trajectory_list());
        }

        private static l_master.DoubleVector ToDoubleVector(IEnumerable<double> values)
        {
            if (values == null)
            {
                throw new ArgumentNullException("values");
            }
            return new l_master.DoubleVector(values);
        }

        private static l_master.UintVector ToUintVector(IEnumerable<uint> values)
        {
            if (values == null)
            {
                throw new ArgumentNullException("values");
            }
            return new l_master.UintVector(values);
        }

        private static l_master.IntVector ToIntVector(IEnumerable<int> values)
        {
            if (values == null)
            {
                throw new ArgumentNullException("values");
            }
            return new l_master.IntVector(values);
        }

        private static double[] ToArray(l_master.DoubleVector values)
        {
            using (values)
            {
                return values.ToArray();
            }
        }

        private static uint[] ToArray(l_master.UintVector values)
        {
            using (values)
            {
                return values.ToArray();
            }
        }

        private static int[] ToArray(l_master.IntVector values)
        {
            using (values)
            {
                return values.ToArray();
            }
        }

        private static string[] ToArray(l_master.StrVector values)
        {
            using (values)
            {
                return values.ToArray();
            }
        }
    }
}
