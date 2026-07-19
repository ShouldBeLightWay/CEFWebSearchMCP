#!/usr/bin/env bash
# Download and unpack the pinned CEF binary distribution for Linux CI/local use.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
# shellcheck source=../ci/cef-version.env
source "${ROOT_DIR}/ci/cef-version.env"

DEST_ROOT="${1:-${ROOT_DIR}/third_party/cef}"

# Spotify CEF archives unpack to a directory matching the archive basename
# (including the build variant suffix, e.g. `_minimal`).
if [[ -n "${CEF_BUILD_VARIANT}" && "${CEF_BUILD_VARIANT}" != "standard" ]]; then
  DIR_NAME="cef_binary_${CEF_VERSION}_${CEF_PLATFORM}_${CEF_BUILD_VARIANT}"
else
  DIR_NAME="cef_binary_${CEF_VERSION}_${CEF_PLATFORM}"
fi

CEF_ROOT="${DEST_ROOT}/${DIR_NAME}"
ARCHIVE_NAME="${DIR_NAME}.tar.bz2"
ARCHIVE_PATH="${DEST_ROOT}/${ARCHIVE_NAME}"
ENCODED_VERSION="${CEF_VERSION//+/%2B}"
URL="https://cef-builds.spotifycdn.com/cef_binary_${ENCODED_VERSION}_${CEF_PLATFORM}"
if [[ -n "${CEF_BUILD_VARIANT}" && "${CEF_BUILD_VARIANT}" != "standard" ]]; then
  URL="${URL}_${CEF_BUILD_VARIANT}.tar.bz2"
else
  URL="${URL}.tar.bz2"
fi

mkdir -p "${DEST_ROOT}"

resolve_cef_root() {
  local candidates=()
  if [[ -n "${CEF_BUILD_VARIANT}" && "${CEF_BUILD_VARIANT}" != "standard" ]]; then
    candidates+=("${DEST_ROOT}/cef_binary_${CEF_VERSION}_${CEF_PLATFORM}_${CEF_BUILD_VARIANT}")
  fi
  candidates+=("${DEST_ROOT}/cef_binary_${CEF_VERSION}_${CEF_PLATFORM}")

  local candidate
  for candidate in "${candidates[@]}"; do
    if [[ -d "${candidate}/cmake" ]]; then
      CEF_ROOT="${candidate}"
      return 0
    fi
  done
  return 1
}

emit_cef_root() {
  echo "CEF_ROOT=${CEF_ROOT}"
  if [[ -n "${GITHUB_OUTPUT:-}" ]]; then
    echo "root=${CEF_ROOT}" >> "${GITHUB_OUTPUT}"
  fi
}

if resolve_cef_root; then
  echo "CEF already present at ${CEF_ROOT}"
  emit_cef_root
  exit 0
fi

if [[ ! -f "${ARCHIVE_PATH}" ]]; then
  echo "Downloading ${URL}"
  curl -fL --retry 3 --retry-delay 2 -o "${ARCHIVE_PATH}.partial" "${URL}"
  mv "${ARCHIVE_PATH}.partial" "${ARCHIVE_PATH}"
fi

echo "Extracting ${ARCHIVE_PATH}"
tar -xjf "${ARCHIVE_PATH}" -C "${DEST_ROOT}"

if ! resolve_cef_root; then
  echo "error: expected CEF cmake bootstrap under ${DEST_ROOT}" >&2
  echo "contents:" >&2
  ls -la "${DEST_ROOT}" >&2 || true
  exit 1
fi

# Drop the archive in CI to keep the Actions cache smaller; keep it locally for re-extract.
if [[ "${CI:-}" == "true" ]]; then
  rm -f "${ARCHIVE_PATH}"
fi

emit_cef_root
