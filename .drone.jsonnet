# Copyright 2022 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

local library = "uuid";

local triggers =
{
    branch: [ "master", "develop", "feature/*" ]
};

local ubsan = { UBSAN: '1', UBSAN_OPTIONS: 'print_stacktrace=1' };
local asan = { ASAN: '1' };

local linux_pipeline(name, image, environment, packages = "", sources = [], arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "docker",
    trigger: triggers,
    platform:
    {
        os: "linux",
        arch: arch
    },
    steps:
    [
        {
            name: "everything",
            image: image,
            environment: environment,
            commands:
            [
                'set -e',
                'wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -',
            ] +
            (if sources != [] then [ ('apt-add-repository "' + source + '"') for source in sources ] else []) +
            (if packages != "" then [ 'apt-get update', 'apt-get -y install ' + packages ] else []) +
            [
                'export LIBRARY=' + library,
                './.drone/drone.sh',
            ]
        }
    ]
};

local macos_pipeline(name, environment, xcode_version = "12.2", osx_version = "catalina", arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "exec",
    trigger: triggers,
    platform: {
        "os": "darwin",
        "arch": arch
    },
    node: {
        "os": osx_version
    },
    steps: [
        {
            name: "everything",
            environment: environment + { "DEVELOPER_DIR": "/Applications/Xcode-" + xcode_version + ".app/Contents/Developer" },
            commands:
            [
                'export LIBRARY=' + library,
                './.drone/drone.sh',
            ]
        }
    ]
};

local windows_pipeline(name, image, environment, arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "docker",
    trigger: triggers,
    platform:
    {
        os: "windows",
        arch: arch
    },
    "steps":
    [
        {
            name: "everything",
            image: image,
            environment: environment,
            commands:
            [
                'cmd /C .drone\\\\drone.bat ' + library,
            ]
        }
    ]
};

[
    linux_pipeline(
        "Linux 23.04 GCC 13 32 C++11",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '11', ADDRMD: '32' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 64 C++11",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '11', ADDRMD: '64' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 32 C++14",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '14', ADDRMD: '32' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 64 C++14",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '14', ADDRMD: '64' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 32 C++17",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '17', ADDRMD: '32' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 64 C++17",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '17', ADDRMD: '64' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 32 C++20",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '20', ADDRMD: '32' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 64 C++20",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '20', ADDRMD: '64' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 32 C++23",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '23', ADDRMD: '32' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 23.04 GCC 13 64 C++23",
        "cppalliance/droneubuntu2304:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-13', CXXSTD: '23', ADDRMD: '64' },
        "g++-13-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 32 C++11",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '11', ADDRMD: '32' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 64 C++11",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '11', ADDRMD: '64' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 32 C++14",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '14', ADDRMD: '32' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 64 C++14",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '14', ADDRMD: '64' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 32 C++17",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '17', ADDRMD: '32' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 64 C++17",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '17', ADDRMD: '64' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 32 C++20",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '20', ADDRMD: '32' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 64 C++20",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '20', ADDRMD: '64' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 32 C++23",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '23', ADDRMD: '32' },
        "g++-14-multilib",
    ),

    linux_pipeline(
        "Linux 24.04 GCC 14 64 C++23",
        "cppalliance/droneubuntu2404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-14', CXXSTD: '23', ADDRMD: '64' },
        "g++-14-multilib",
    ),
]
