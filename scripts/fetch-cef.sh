#!/usr/bin/env bash
# Download and unpack the pinned CEF binary distribution for Linux CI/local use.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
# shellcheck source=../ci/cef-version.env
source "${ROOT_DIR}/ci/cef-version.env"

DEST_ROOT="${1:-${ROOT_DIR}/third_party/cef}"
DIR_NAME="cef_binary_${CEF_VERSION}_${CEF_PLATFORM}"
CEF_ROOT="${DEST_ROOT}/${DIR_NAME}"
ARCHIVE_NAME="${DIR_NAME}_${CEF_BUILD_VARIANT}.tar.bz2"
ARCHIVE_PATH="${DEST_ROOT}/${ARCHIVE_NAME}"
ENCODED_VERSION="${CEF_VERSION//+/%2B}"
URL="https://cef-builds.spotifycdn.com/cef_binary_${ENCODED_VERSION}_${CEF_PLATFORM}_${CEF_BUILD_VARIANT}.tar.bz2"

mkdir -p "${DEST_ROOT}"

if [[ -d "${CEF_ROOT}/cmake" ]]; then
  echo "CEF already present at ${CEF_ROOT}"
  echo "CEF_ROOT=${CEF_ROOT}"
  exit 0
fi

if [[ ! -f "${ARCHIVE_PATH}" ]]; then
  echo "Downloading ${URL}"
  curl -fL --retry 3 --retry-delay 2 -o "${ARCHIVE_PATH}.partial" "${URL}"
  mv "${ARCHIVE_PATH}.partial" "${ARCHIVE_PATH}"
fi

echo "Extracting ${ARCHIVE_PATH}"
tar -xjf "${ARCHIVE_PATH}" -C "${DEST_ROOT}"

if [[ ! -d "${CEF_ROOT}/cmake" ]]; then
  echo "error: expected CEF cmake bootstrap at ${CEF_ROOT}/cmake" >&2
  exit 1
fi

# Drop the archive in CI to keep the Actions cache smaller; keep it locally for re-extract.
if [[ "${CI:-}" == "true" ]]; then
  rm -f "${ARCHIVE_PATH}"
fi

echo "CEF_ROOT=${CEF_ROOT}"
