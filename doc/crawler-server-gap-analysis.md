# Gap Analysis: current crawler-server.js

## Current Behavior Summary
The current Node.js crawler issues direct HTTP requests to search engine HTML and parses the result page with Cheerio.

## Key Weaknesses
- Anonymous traffic profile causes frequent anti-bot blocks and captcha flows.
- No persisted authenticated browser session.
- Parser logic depends on volatile CSS selectors (`div.g`, `div.VwiC3b`, `li.b_algo`).
- Fallback strategy is coarse and lacks failure classification.
- No request-level trace metadata or robust observability.

## Improvements in CEFWebSearchMCP
- Use authenticated CEF profiles to lower block rates.
- Keep provider adapters isolated and versioned.
- Add strict timeout and fallback budget.
- Return explicit failure reasons and provider trace.
- Expose MCP-native health and provider state tools.
