"""Python facade over the SWIG-generated robot binding."""

from . import l_master


class Robot:
    """Idiomatic Python facade for the Lebai robot controller."""

    def __init__(self, ip, simulator=False):
        self._native = l_master.Robot(ip, simulator)

    @property
    def native(self):
        """Return the low-level SWIG robot object."""
        return self._native

    def __getattr__(self, name):
        return getattr(self._native, name)

    def move_joint(self, joint_positions, a, v, t, r):
        return self._native.move_joint(_sequence(joint_positions), a, v, t, r)

    def movej(self, joint_positions, a, v, t, r):
        return self._native.movej(_sequence(joint_positions), a, v, t, r)

    def move_linear(self, joint_positions, a, v, t, r):
        return self._native.move_linear(_sequence(joint_positions), a, v, t, r)

    def movel(self, joint_positions, a, v, t, r):
        return self._native.movel(_sequence(joint_positions), a, v, t, r)

    def move_circular(self, joint_via, joint, rad, a, v, t, r):
        return self._native.move_circular(
            _sequence(joint_via), _sequence(joint), rad, a, v, t, r
        )

    def movec(self, joint_via, joint, rad, a, v, t, r):
        return self._native.movec(
            _sequence(joint_via), _sequence(joint), rad, a, v, t, r
        )

    def speed_joint(self, a, speed, t=0.0):
        return self._native.speed_joint(a, _sequence(speed), t)

    def speedj(self, a, speed, t=0.0):
        return self._native.speedj(a, _sequence(speed), t)

    def toward_joint(self, joint_positions, a, v, t, r):
        return self._native.toward_joint(_sequence(joint_positions), a, v, t, r)

    def towardj(self, joint_positions, a, v, t, r):
        return self._native.towardj(_sequence(joint_positions), a, v, t, r)

    def move_pvat(self, p, v, a, t):
        return self._native.move_pvat(_sequence(p), _sequence(v), _sequence(a), t)

    def get_actual_joint_positions(self):
        return _list(self._native.get_actual_joint_positions())

    def get_target_joint_positions(self):
        return _list(self._native.get_target_joint_positions())

    def get_actual_joint_speed(self):
        return _list(self._native.get_actual_joint_speed())

    def get_target_joint_speed(self):
        return _list(self._native.get_target_joint_speed())

    def get_actual_joint_torques(self):
        return _list(self._native.get_actual_joint_torques())

    def get_target_joint_torques(self):
        return _list(self._native.get_target_joint_torques())

    def get_box_devices(self, prefix):
        return _list(self._native.get_box_devices(prefix))

    def get_dos(self, device, pin, num):
        return _list(self._native.get_dos(device, pin, num))

    def set_dos(self, device, pin, values):
        return self._native.set_dos(device, pin, _sequence(values))

    def get_dis(self, device, pin, num):
        return _list(self._native.get_dis(device, pin, num))

    def get_aos(self, device, pin, num):
        return _list(self._native.get_aos(device, pin, num))

    def set_aos(self, device, pin, values):
        return self._native.set_aos(device, pin, _sequence(values))

    def get_ais(self, device, pin, num):
        return _list(self._native.get_ais(device, pin, num))

    def set_led(self, mode, speed, color):
        return self._native.set_led(mode, speed, _sequence(color))

    def get_signals(self, index, length):
        return _list(self._native.get_signals(index, length))

    def set_signals(self, index, values):
        return self._native.set_signals(index, _sequence(values))

    def load_tcp_list(self, directory=""):
        return _list(self._native.load_tcp_list(directory))

    def load_pose_list(self, directory=""):
        return _list(self._native.load_pose_list(directory))

    def load_frame_list(self, directory=""):
        return _list(self._native.load_frame_list(directory))

    def load_trajectory_list(self, directory=""):
        return _list(self._native.load_trajectory_list(directory))


def _sequence(values):
    if values is None:
        raise TypeError("values must not be None")
    if isinstance(values, dict):
        return values
    return list(values)


def _list(values):
    return list(values)
