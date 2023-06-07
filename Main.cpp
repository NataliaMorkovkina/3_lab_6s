#include "lib/HA.h"
#include "lib/RLE.h"
#include "lib/LZ78.h"
#include "lib/BWT.h"
#include "lib/MTF.h"
#include "lib/AC.h"
#include "lib/PPM/PPM.h"
#include "chrono"

int main()
{
    std::string fe = "eeee";
    std::string mc = "Compressed";
    std::string md = "Decompressed";

    auto start = std::chrono::high_resolution_clock::now();

    BWTCompression(fe      , fe + '1');
    std::cout << "d\n";
    MTFCompression(fe + '1', fe + '2');
    std::cout << "d\n";
    HACompression(fe + '2', fe + mc);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << duration.count() << "s\n";
}