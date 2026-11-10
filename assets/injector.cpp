#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdint>

// XOR encryption/decryption
inline uint8_t xor_byte(uint8_t a, uint8_t b) { return a ^ b; }
std::vector<uint8_t> xor_decrypt(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    std::vector<uint8_t> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = xor_byte(data[i], key[i % key.size()]);
    }
    return result;
}

int main() {
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_DUPLICATE_HANDLE, FALSE, <GorillaTag PID>);
    if (!hProcess) {
        std::cerr << "Failed to open process\n";
        return 1;
    }

    // Load encrypted DLL
    std::vector<uint8_t> encryptedPayload = /* load encrypted file */;
    std::vector<uint8_t> decryptedPayload = xor_decrypt(encryptedPayload, <your key>);

    // Map DLL in memory
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)decryptedPayload.data();
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(pDosHeader + pDosHeader->e_lfanew);
    PIMAGE_DATA_DIRECTORY pDataDir = &pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMAGE_BASE];
    PBYTE pImageBase = (PBYTE)(pDosHeader + pDosHeader->e_lfanew + pNtHeaders->OptionalHeader.ImageBase);

    void* mapped = VirtualAllocEx(hProcess, NULL, decryptedPayload.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mapped) { std::cerr << "VirtualAllocEx failed\n"; return 1; }

    if (WriteProcessMemory(hProcess, mapped, decryptedPayload.data(), decryptedPayload.size(), NULL) != 0) {
        std::cerr << "WriteProcessMemory failed\n";
        VirtualFreeEx(hProcess, mapped, 0, MEM_RELEASE);
        return 1;
    }

    // Call mod entry point (example: MyMod::MainLoader::StartMod)
    // You must resolve the function address via PE parsing or known offset
    // For simplicity, here we just show the call pattern:
    // FARPROC func = (FARPROC)0x<function address>;
    // VirtualCallEx(hProcess, func, <args>);

    CloseHandle(hProcess);
    return 0;
}
