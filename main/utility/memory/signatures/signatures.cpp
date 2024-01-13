#include "signatures.hpp"
#include "../../logger/logger.hpp"

uint8_t ToHexadecimal(char character) {
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    else if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    else if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }

    return 0xFF;
}

Scanner::Scanner(std::string signature) {
    signature.erase(std::remove(signature.begin(), signature.end(), ' '), signature.end());

    std::vector<uint8_t> byteVector;
    byteVector.reserve(signature.size());

    for (size_t i = 0, j = 0; i < signature.size(); i+=2, j++) {
        size++;

        if (signature[i] == '?') {
            byteVector.emplace_back('?');
            i--;
        }
        else {
            byteVector.emplace_back((ToHexadecimal(signature[i]) << 4 | ToHexadecimal(signature[i + 1])));
        }
    }

    signatureBytes = byteVector;
}

Result::Result(uintptr_t result) {
    address = result;
}

Result Result::Add(uintptr_t value) {
    return Result(address + value);
}

Result Result::Sub(uintptr_t value) {
    return Result(address - value);
}

Result Result::Rip(uintptr_t add) {
    uintptr_t result = address + add;
    return Result(result + (*(int32_t*)result + 0x4));
}

template <typename T>
T Result::Cast() {
    return (T)(address);
}

Scan::Scan(std::string signature, Module data) {
    Scanner scanner = Scanner(signature);

    uint8_t* bytes = (uint8_t*)(data.base);
    size_t size = data.size;
    uintptr_t base = data.base;

    size_t maxShift = scanner.size;
    size_t maxIndex = scanner.size - 1;
    size_t wildCardIndex = 0;
    for (size_t i = 0; i < maxIndex; i++) {
        if (scanner.signatureBytes[i] == '?') {
            maxShift = maxIndex - i;
            wildCardIndex = i;
        }
    }

    size_t shiftTable[UINT8_MAX + 1];
    for (size_t i = 0; i <= UINT8_MAX; i++) {
        shiftTable[i] = maxShift;
    }

    for (size_t i = wildCardIndex + 1; i < maxIndex - 1; i++) {
        shiftTable[scanner.signatureBytes[i]] = maxIndex - i;
    }

    for (size_t currentIndex = 0; currentIndex < size - scanner.size;) {
        for (size_t sigIndex = maxIndex; sigIndex >= 0; sigIndex--) {
            if (bytes[currentIndex + sigIndex] != scanner.signatureBytes[sigIndex] && scanner.signatureBytes[sigIndex] != '?') {
                currentIndex += shiftTable[bytes[currentIndex + maxIndex]];

                break;
            }
            else if (sigIndex == 0) {
                result = (uintptr_t)(currentIndex + base);
                return;
            }
        }
    }

    result = 0x0;
}

Result Scan::Get() {
    return Result(result);
}

static Signatures* signaturesInstance = nullptr;

Signatures* Signatures::Instance() {
	if (!signaturesInstance)
		signaturesInstance = new Signatures();

	return signaturesInstance;
}

void Signatures::Initialize() {
    auto timerStart = std::chrono::system_clock::now();

    definitions.getNativeAddressFromHash = Scan(xorstr_("E8 ? ? ? ? 42 8B 9C FE")).Get().Rip(0x1).Cast<decltype(definitions.getNativeAddressFromHash)>();
    definitions.getPlayerPed = Scan(xorstr_("40 53 48 83 EC 20 33 DB 81 F9")).Get().Cast<uintptr_t>();

    definitions.swapchain = Scan(xorstr_("45 8B C7 8B 53 08 48 8B 01 FF 50 40")).Get().Sub(0x7).Rip().Cast<IDXGISwapChain**>();
    definitions.commandQueue = Scan(xorstr_("49 8B F8 85 F6")).Get().Sub(0x65).Rip().Cast<ID3D12CommandQueue**>();

    auto timerEnd = std::chrono::system_clock::now();

    auto time = std::chrono::duration<float>(timerEnd - timerStart).count();
    Logger::Instance()->Log(xorstr_("Found Signatures In %fs \n"), time);
}

void Signatures::Uninitialize() {
    signaturesInstance = nullptr;
    delete this;
}