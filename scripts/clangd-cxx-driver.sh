#!/usr/bin/env bash
set -euo pipefail

detect_gcc_install_dir() {
  if [[ -n "${CLANGD_GCC_INSTALL_DIR:-}" && -d "${CLANGD_GCC_INSTALL_DIR}" ]]; then
    printf '%s\n' "${CLANGD_GCC_INSTALL_DIR}"
    return 0
  fi

  local machine=""
  if command -v g++ >/dev/null 2>&1; then
    machine="$(g++ -dumpmachine 2>/dev/null || true)"
  fi

  local -a candidates=()
  local dir=""
  local ver=""

  if [[ -n "${machine}" ]]; then
    for dir in /usr/lib/gcc/"${machine}"/*; do
      [[ -d "${dir}" ]] || continue
      ver="$(basename "${dir}")"
      if [[ -d "/usr/include/c++/${ver}" ]]; then
        candidates+=("${dir}")
      fi
    done
  fi

  if [[ ${#candidates[@]} -eq 0 ]]; then
    for dir in /usr/lib/gcc/*/*; do
      [[ -d "${dir}" ]] || continue
      ver="$(basename "${dir}")"
      if [[ -d "/usr/include/c++/${ver}" ]]; then
        candidates+=("${dir}")
      fi
    done
  fi

  if [[ ${#candidates[@]} -eq 0 ]]; then
    return 1
  fi

  local best=""
  best="$({
    for dir in "${candidates[@]}"; do
      printf '%s|%s\n' "$(basename "${dir}")" "${dir}"
    done
  } | sort -t'|' -k1,1V | tail -n1 | cut -d'|' -f2-)"

  if [[ -n "${best}" ]]; then
    printf '%s\n' "${best}"
    return 0
  fi

  return 1
}

main() {
  local clang_bin="${CLANGD_CLANGXX:-/usr/bin/clang++}"
  if [[ ! -x "${clang_bin}" ]]; then
    clang_bin="$(command -v clang++ || true)"
  fi

  if [[ -z "${clang_bin}" ]]; then
    local -a clang_candidates=()
    local candidate=""
    for candidate in /usr/bin/clang++-*; do
      [[ -x "${candidate}" ]] || continue
      clang_candidates+=("${candidate}")
    done

    if [[ ${#clang_candidates[@]} -gt 0 ]]; then
      clang_bin="$({
        for candidate in "${clang_candidates[@]}"; do
          printf '%s|%s\n' "${candidate##*-}" "${candidate}"
        done
      } | sort -t'|' -k1,1V | tail -n1 | cut -d'|' -f2-)"
    fi
  fi

  if [[ -z "${clang_bin}" ]]; then
    echo "clangd-cxx-driver.sh: clang++ not found" >&2
    exit 1
  fi

  local gcc_install_dir=""
  if gcc_install_dir="$(detect_gcc_install_dir)"; then
    exec "${clang_bin}" "--gcc-install-dir=${gcc_install_dir}" "$@"
  fi

  exec "${clang_bin}" "$@"
}

main "$@"