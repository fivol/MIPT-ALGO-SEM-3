#include <iostream>
#include "wavfile.h"

#define PRINT(x) std::cout << #x <<" = " << x << '\n'

const char *failed_arguments_warning = "Specify filename to compress and output filename\n "
                                       "Example: ./program file.wav compressed.wav";

int main(int argc, char **args) {
    if (argc < 3) {
        std::cout << failed_arguments_warning << '\n';
        return 1;
    }
    WAVFile file;
    if (!file.ReadFile(args[1])) {
        std::cout << "Incorrect filename" << '\n';
        return 1;
    }
    double compression_ratio = 0.3;
    int page_size = 128;
    std::string compression_type;
    if (argc > 3) {
        compression_ratio = std::stod(args[3]);
    }
    if (argc > 4) {
        compression_type = args[4];
        if (compression_type != COMPRESS_BEGIN &&
            compression_type != COMPRESS_MIDDLE &&
            compression_type != COMPRESS_END) {
            std::cout << "Incorrect compression type. Please specify 'begin' or 'middle' or 'end'";
            return 1;
        }
    }
    if (argc > 5) {
        page_size = std::stoi(args[5]);
    }

    std::cout << "Input file: " << args[1] << '\n';
    std::cout << "Output file: " << args[1] << '\n';
    std::cout << "Compression ratio: " << compression_ratio << '\n';
    std::cout << "Compression page size: " << page_size << '\n';
    auto header = file.GetHeader();
    PRINT(header.audioFormat);
    PRINT(header.numChannels);
    PRINT(header.sampleRate);
    PRINT(header.byteRate);
    PRINT(header.blockAlign);
    PRINT(header.bitsPerSample);
    PRINT(header.subchunk2Size);

    file.MakeFFTCompression(compression_ratio, compression_type, page_size);

    if (!file.SaveFile(args[2])) {
        std::cout << "Can not save file" << '\n';
        return 1;
    }

    std::cout << "Done!" << '\n';

    return 0;
}
