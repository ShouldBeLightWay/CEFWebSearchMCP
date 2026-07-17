# CEFWebSearchMCP Overview

## Mission
Build a reliable localhost Web Search MCP server that uses authenticated browser sessions (Google, Bing, Yandex) through CEF to avoid anonymous scraping bans and improve search quality.

## Problem Statement
Existing localhost crawlers are often blocked by search engines because they run anonymous requests and do not preserve trusted user sessions. They also lack resilience features such as strict timeouts, fallback providers, and robust failure handling.

## Project Goals
- Run as a local MCP-compatible search service for AI agents.
- Support authenticated sessions in Google, Bing, and Yandex via CEF profiles.
- Provide predictable latency with configurable timeouts and retry policies.
- Use provider failover when one provider degrades or blocks requests.
- Expose transparent status and diagnostics for reliability.

## Non-Goals (Phase 1)
- Bypass anti-bot protections with hidden automation tricks.
- Cloud multi-tenant deployment.
- Full distributed architecture.

## Core Stack
- C++20
- CMake
- libcef (Chromium Embedded Framework)
- Boost (program_options, filesystem, system)

## Milestone Deliverables
1. Buildable project scaffold and docs.
2. CEF bootstrap and persisted profile management.
3. Search provider adapters with fallback and timeout controls.
4. MCP transport and tool contracts.
5. Hardening, observability, and packaging.
