#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

# We're building poppler with cpp bindings with minimal dependencies for our own since
# the official recipe is quite old and there is a lot of complexity due to lots of wrappers,
# options and stuff like this. We avoid this by using an own stripped minimal recipe.
${WORKSPACE_ROOT}/etc/poppler/conan-create.sh ${BUILD_TYPE}

# Dependencies like eaasyloggingcpp are still compatible with quite old cmake versions
# like v2.8 but the support for this has been remove in cmake v4. So to make them
# compilable, we force compatibility mode of cmake v3.5 but this might fail at any point.
export CMAKE_POLICY_VERSION_MINIMUM=3.5

# On systems having just clang installed without gcc, this would not be necessary since
# default profile would autodetect the matching stdlib by default.
# But e.g. on ubuntu having installed gcc and libstdc++11 by default, but using non-default clang for build,
# the autodetect defines libstdc++11 in default profile even when we force the compiler to clang.
# So we overrule this setting here explicitly for clang and use the default autodetect value otherwiese.
# readonly SETTING_STANDARD_LIB=$(if [[ "$COMPILER_NAME" =~ ^(clang|apple-clang)$ ]]; then echo '-s compiler.libcxx=libc++'; else echo ''; fi)

conan install ${WORKSPACE_ROOT} \
    --build missing \
    -of ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -s:b "build_type=${BUILD_TYPE}" \
    ${@:2}

conan cache clean '*'
