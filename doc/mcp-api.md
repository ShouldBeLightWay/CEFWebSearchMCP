# MCP API Draft

## Tool: `search.web`

### Input
```json
{
  "query": "best c++ mcp server design",
  "max_results": 8,
  "language": "ru",
  "provider_priority": ["google", "bing", "yandex"],
  "timeout_ms": 15000
}
```

### Output
```json
{
  "query": "best c++ mcp server design",
  "provider_used": "google",
  "fallback_count": 0,
  "duration_ms": 842,
  "results": [
    {
      "title": "Example title",
      "snippet": "Example snippet",
      "url": "https://example.com/page",
      "site_name": "example.com",
      "provider": "google"
    }
  ]
}
```

### Error Output
```json
{
  "query": "best c++ mcp server design",
  "error": {
    "code": "all_providers_failed",
    "message": "All providers failed within timeout budget",
    "details": [
      { "provider": "google", "reason": "captcha" },
      { "provider": "bing", "reason": "timeout" },
      { "provider": "yandex", "reason": "parse_error" }
    ]
  }
}
```

## Tool: `search.health`

### Output
```json
{
  "status": "ok",
  "version": "0.1.0",
  "components": {
    "cef": "ok",
    "mcp": "ok",
    "google_profile": "ready",
    "bing_profile": "ready",
    "yandex_profile": "not_authenticated"
  }
}
```

## Tool: `search.providers`

### Output
```json
{
  "providers": [
    { "name": "google", "enabled": true, "authenticated": true },
    { "name": "bing", "enabled": true, "authenticated": true },
    { "name": "yandex", "enabled": true, "authenticated": false }
  ]
}
```
