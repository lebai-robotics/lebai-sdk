param(
  [string]$WheelDir = "build\python\dist",
  [string]$Python = "python.exe",
  [string]$VenvDir = "build\python-smoke-venv"
)

$ErrorActionPreference = "Stop"

$wheels = Get-ChildItem -Path $WheelDir -Filter "pylebai-*.whl"
if ($wheels.Count -ne 1) {
  throw "Expected exactly one pylebai wheel in $WheelDir, found $($wheels.Count)"
}

& $Python -m venv --clear $VenvDir
$venvPython = Join-Path $VenvDir "Scripts\python.exe"
& $venvPython -m pip install --no-index $wheels[0].FullName
& $venvPython -c @"
import pylebai
from pylebai import gripper, l_master, zeroconf

print("pylebai", pylebai.__version__)
print("l_master version", l_master.version())
print("Robot has move_joint", hasattr(l_master.Robot, "move_joint"))
print("Robot.move_joint has docstring", bool(l_master.Robot.move_joint.__doc__))
print("zeroconf module", bool(zeroconf))
print("gripper module", bool(gripper))

if not hasattr(l_master.Robot, "move_joint"):
    raise SystemExit("l_master.Robot.move_joint is missing")
if not l_master.Robot.move_joint.__doc__:
    raise SystemExit("l_master.Robot.move_joint docstring is missing")
"@
