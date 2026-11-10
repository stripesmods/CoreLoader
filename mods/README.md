# CoreLoader project scaffold

This folder contains the loader skeleton and example module entry points.

- `loader.cpp` scans a `mods/` directory for shared libraries.
- `injector_example.cpp` is a placeholder and intentionally not a real process injector.
- Keep all DLL/SO files in `mods/` and do not target arbitrary processes without explicit authorization.
