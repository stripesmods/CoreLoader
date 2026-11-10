// Safe loader skeleton for a legitimate local mod-loading project.
// This is a template only and does not implement arbitrary process injection.

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace fs = std::filesystem;

struct ModEntry {
    std::string name;
    std::string path;
#ifdef _WIN32
    HMODULE handle = nullptr;
#else
    void* handle = nullptr;
#endif
};

class ModLoader {
public:
    explicit ModLoader(std::string modsDir) : modsDirectory(std::move(modsDir)) {}

    void discoverMods() {
        std::cout << "[CoreLoader] Scanning: " << modsDirectory << "\n";
        if (!fs::exists(modsDirectory)) {
            std::cout << "[CoreLoader] Folder does not exist yet.\n";
            return;
        }

        for (const auto& entry : fs::directory_iterator(modsDirectory)) {
            if (!entry.is_regular_file()) continue;

            const auto ext = entry.path().extension().string();
            if (ext == ".dll" || ext == ".so" || ext == ".dylib") {
                ModEntry mod;
                mod.name = entry.path().stem().string();
                mod.path = entry.path().string();
                mods.push_back(mod);
                std::cout << "  Found mod: " << mod.name << "\n";
            }
        }
    }

    void loadMods() {
        std::cout << "[CoreLoader] Loading mods...\n";
        for (auto& mod : mods) {
#ifdef _WIN32
            mod.handle = LoadLibraryA(mod.path.c_str());
            if (!mod.handle) {
                std::cerr << "  Failed to load: " << mod.name << "\n";
                continue;
            }
#else
            mod.handle = dlopen(mod.path.c_str(), RTLD_LAZY);
            if (!mod.handle) {
                std::cerr << "  Failed to load: " << mod.name << "\n";
                continue;
            }
#endif
            std::cout << "  Loaded: " << mod.name << "\n";
        }
    }

    void unloadMods() {
        std::cout << "[CoreLoader] Unloading mods...\n";
        for (auto& mod : mods) {
#ifdef _WIN32
            if (mod.handle) {
                FreeLibrary(mod.handle);
                mod.handle = nullptr;
            }
#else
            if (mod.handle) {
                dlclose(mod.handle);
                mod.handle = nullptr;
            }
#endif
        }
    }

private:
    std::string modsDirectory;
    std::vector<ModEntry> mods;
};

int main() {
    ModLoader loader("mods");
    loader.discoverMods();
    loader.loadMods();

    std::cout << "[CoreLoader] Press Enter to unload and exit.\n";
    std::cin.get();

    loader.unloadMods();
    return 0;
}
