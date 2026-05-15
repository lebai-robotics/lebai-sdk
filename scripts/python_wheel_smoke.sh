#!/usr/bin/env bash
set -euo pipefail

build_dir="${1:-build}"
python_exe="${2:-python3}"

wheel_dir="${build_dir}/python/dist"
venv_dir="${build_dir}/python-smoke-venv"
pip_cache_dir="${build_dir}/.pip-cache"

if ! command -v "${python_exe}" >/dev/null 2>&1; then
  echo "Python executable not found: ${python_exe}" >&2
  exit 1
fi

shopt -s nullglob
wheels=("${wheel_dir}"/pylebai-*.whl)
shopt -u nullglob

if [[ ${#wheels[@]} -ne 1 ]]; then
  echo "Expected exactly one pylebai wheel in ${wheel_dir}, found ${#wheels[@]}" >&2
  exit 1
fi

mkdir -p "${pip_cache_dir}"
export PIP_CACHE_DIR="${pip_cache_dir}"

"${python_exe}" -m venv --clear "${venv_dir}"
"${venv_dir}/bin/python" -m pip install --no-index "${wheels[0]}"
"${venv_dir}/bin/python" - <<'PY'
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
PY
