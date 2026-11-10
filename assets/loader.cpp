#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <dlfcn.h> // For dynamic loading on Linux/Mac
#ifdef _WIN32
#include <windows.h> // For dynamic loading on Windows
#endif

namespace fs = std::filesystem;

struct Mod {
    std::string name;
    std::string path;
    void* handle;
};

class ModLoader {
public:
    ModLoader(const std::string& modsDir) : modsDirectory(modsDir) {}

    void discoverMods() {
        std::cout << "[CoreLoader] Scanning mods directory: " << modsDirectory << "\n";
        for (const auto& entry : fs::directory_iterator(modsDirectory)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".dll" || ext == ".so" || ext == ".dylib") {
                    Mod mod;
                    mod.name = entry.path().stem().string();
                    mod.path = entry.path().string();
                    mods.push_back(mod);
                    std::cout << "  Found mod: " << mod.name << "\n";
                }
            }
        }
    }

    void loadMods() {
        std::cout << "[CoreLoader] Loading mods...\n";
        for (auto& mod : mods) {
#ifdef _WIN32
            mod.handle = LoadLibraryA(mod.path.c_str());
            if (!mod.handle) {
                std::cerr << "  Failed to load " << mod.name << "\n";
                continue;
            }
#else
            mod.handle = dlopen(mod.path.c_str(), RTLD_LAZY);
            if (!mod.handle) {
                std::cerr << "  Failed to load " << mod.name << ": " << dlerror() << "\n";
                continue;
            }
#endif
            std::cout << "  Successfully loaded: " << mod.name << "\n";
        }
    }

    void unloadMods() {
        std::cout << "[CoreLoader] Unloading mods...\n";
        for (auto& mod : mods) {
            if (mod.handle) {
#ifdef _WIN32
                FreeLibrary((HMODULE)mod.handle);
#else
                dlclose(mod.handle);
#endif
                std::cout << "  Unloaded: " << mod.name << "\n";
            }
        }
    }

private:
    std::string modsDirectory;
    std::vector<Mod> mods;
};

int main() {
    std::string modsPath = "./mods"; // Relative path to mods folder
    ModLoader loader(modsPath);

    loader.discoverMods();
    loader.loadMods();

    std::cout << "[CoreLoader] All mods loaded. Press Enter to unload and exit.\n";
    std::cin.get();

    loader.unloadMods();
    return 0;
}
