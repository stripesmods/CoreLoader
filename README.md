# CoreLoader

This repository now includes a small safe starter structure for a mod-loading project:

- `src/` — source code examples
- `mods/` — place plugin modules or DLLs here
- `.gitignore` — ignores build artifacts

Important:
- This repository does not include a real process injector.
- Any process interaction or injection capability should only be used with explicit, authorized access and in compliance with applicable laws and platform policies.

To create a ZIP archive locally from your machine:

  zip -r CoreLoader.zip .

or on Windows PowerShell:

  Compress-Archive -Path . -DestinationPath CoreLoader.zip
