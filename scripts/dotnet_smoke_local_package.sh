#!/usr/bin/env bash
set -euo pipefail

package_dir="${1:?usage: dotnet_smoke_local_package.sh <package-dir> <version>}"
version="${2:?usage: dotnet_smoke_local_package.sh <package-dir> <version>}"
work_dir="${TMPDIR:-/tmp}/lebai-dotnet-smoke"
nuget_cache="${TMPDIR:-/tmp}/lebai-dotnet-smoke-packages"
dotnet_home="${TMPDIR:-/tmp}/dotnet-cli-home"

rm -rf "${work_dir}" "${nuget_cache}"
mkdir -p "${dotnet_home}"

DOTNET_CLI_HOME="${dotnet_home}" \
DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 \
dotnet new console -o "${work_dir}" --framework net8.0 --no-restore

cat > "${work_dir}/NuGet.config" <<EOF
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <packageSources>
    <clear />
    <add key="local-lebai" value="${package_dir}" />
  </packageSources>
</configuration>
EOF

cat > "${work_dir}/Program.cs" <<'EOF'
Console.WriteLine(lebai.l_master.l_master.version());

try {
    using var gripper = new lebai.gripper.Gripper("/tmp/lebai_missing_serial");
    Console.WriteLine("unexpected gripper open");
    Environment.ExitCode = 2;
} catch (Exception ex) when (ex.GetType().Name != "DllNotFoundException") {
    Console.WriteLine(ex.GetType().Name);
}
EOF

DOTNET_CLI_HOME="${dotnet_home}" \
DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 \
NUGET_PACKAGES="${nuget_cache}" \
dotnet add "${work_dir}/lebai-dotnet-smoke.csproj" package lebai \
  --version "${version}" \
  --source "${package_dir}"

DOTNET_CLI_HOME="${dotnet_home}" \
DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 \
NUGET_PACKAGES="${nuget_cache}" \
dotnet run --project "${work_dir}/lebai-dotnet-smoke.csproj" --no-restore

