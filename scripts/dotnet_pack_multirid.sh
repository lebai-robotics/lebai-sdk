#!/usr/bin/env bash
set -euo pipefail

build_dir="${1:-build-dotnet-multirid}"
artifacts_dir="${2:-dotnet-native-artifacts}"

cmake -S . -B "${build_dir}" \
  -DBUILD_DOTNET=ON \
  -DBUILD_TESTING=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_DOCUMENTATION=OFF

native_assets_dir="${build_dir}/dotnet/native-assets"
mkdir -p "${native_assets_dir}"

if [[ -d "${artifacts_dir}" ]]; then
  while IFS= read -r native_dir; do
    rid="$(printf '%s\n' "${native_dir}" | sed -n 's#.*runtimes/\([^/]*\)/native$#\1#p')"
    if [[ -z "${rid}" ]]; then
      continue
    fi
    target_dir="${native_assets_dir}/runtimes/${rid}/native"
    mkdir -p "${target_dir}"
    cp -a "${native_dir}/." "${target_dir}/"
  done < <(find "${artifacts_dir}" -type d -path "*/runtimes/*/native" | sort)
fi

echo "Staged .NET native assets:"
find "${native_assets_dir}" -type f | sort

rm -f "${build_dir}/dotnet/native-assets/timestamp"
rm -f "${build_dir}/dotnet/lebai/timestamp"
rm -f "${build_dir}"/dotnet/packages/lebai.[0-9]*.nupkg
rm -f "${build_dir}"/dotnet/packages/lebai.[0-9]*.snupkg

cmake --build "${build_dir}" --target dotnet_package --config Release
