# Risk Register

## Technical Risks
- R-1: CEF packaging complexity on Linux can slow delivery.
- R-2: Search markup changes may break parsers.
- R-3: Captcha/challenge pages may still appear even for authenticated profiles.
- R-4: Timeout tuning may be difficult under unstable network conditions.

## Product Risks
- R-5: Provider terms may constrain automation patterns.
- R-6: User onboarding for account authentication may be confusing.

## Mitigations
- M-1: Keep provider adapters isolated and test with HTML fixtures.
- M-2: Add provider health and parser-failure telemetry.
- M-3: Implement clear first-run auth flow and status reporting.
- M-4: Keep strict timeout and fallback defaults with user overrides.
