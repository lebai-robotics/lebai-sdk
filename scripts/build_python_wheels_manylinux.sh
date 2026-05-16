#!/usr/bin/env bash
set -euo pipefail

out_dir="${1:-wheelhouse}"
python_glob="${PYTHON_GLOB:-/opt/python/cp*/bin}"

rm -rf whl "${out_dir}"
mkdir -p whl "${out_dir}"

shopt -s nullglob
python_bins=(${python_glob})
shopt -u nullglob

if [[ ${#python_bins[@]} -eq 0 ]]; then
  echo "No Python installations matched: ${python_glob}" >&2
  exit 1
fi

for pybin in "${python_bins[@]}"; do
  python_exe="${pybin}/python"
  tag="$(basename "$(dirname "${pybin}")")"
  build_dir="build-python-${tag}"

  echo "Building pylebai wheel with ${python_exe}"
  "${python_exe}" -m pip install --upgrade build setuptools wheel swig

  rm -rf "${build_dir}"
  PATH="${pybin}:${PATH}" cmake -S . -B "${build_dir}" \
    -DBUILD_PYTHON=ON \
    -DBUILD_TESTING=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_DOCUMENTATION=OFF \
    -DFETCH_PYTHON_DEPS=OFF \
    -DPython3_EXECUTABLE="${python_exe}"
  cmake --build "${build_dir}" --target python_package
  scripts/python_wheel_smoke.sh "${build_dir}" "${python_exe}"
  cp "${build_dir}"/python/dist/*.whl whl/
done

for wheel in whl/*.whl; do
  auditwheel repair -w "${out_dir}" "${wheel}"
done
