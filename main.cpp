#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class ImageToAscii
{
private:
    // ASCII characters ordered from darkest to lightest
    const std::string ASCII_CHARS = "@%#*+=-:. ";

    int width, height, channels;
    unsigned char *imageData;
    int outputWidth, outputHeight;
    bool invertColors;
    char customChars[256];
    bool useCustomChars;

public:
    ImageToAscii() : imageData(nullptr), outputWidth(80), outputHeight(0),
                     invertColors(false), useCustomChars(false) {}

    ~ImageToAscii()
    {
        if (imageData)
        {
            stbi_image_free(imageData);
        }
    }

    bool loadImage(const std::string &filename)
    {
        imageData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (!imageData)
        {
            std::cerr << "Error: Could not load image " << filename << std::endl;
            std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
            return false;
        }

        std::cout << "Loaded image: " << width << "x" << height
                  << " with " << channels << " channels" << std::endl;
        return true;
    }

    void setOutputSize(int w, int h)
    {
        outputWidth = w;
        outputHeight = h;
    }

    void setInvertColors(bool invert)
    {
        invertColors = invert;
    }

    void setCustomChars(const std::string &chars)
    {
        if (chars.length() > 0)
        {
            useCustomChars = true;
            size_t len = std::min(chars.length(), sizeof(customChars) - 1);
            chars.copy(customChars, len);
            customChars[len] = '\0';
        }
    }

    int getPixelGrayvalue(int x, int y)
    {
        if (x >= width || y >= height || x < 0 || y < 0)
            return 0;

        int index = (y * width + x) * channels;

        if (channels == 1)
        {
            return imageData[index];
        }
        else if (channels >= 3)
        {
            // Convert RGB to grayscale using luminance formula
            int r = imageData[index];
            int g = imageData[index + 1];
            int b = imageData[index + 2];
            return (int)(0.299 * r + 0.587 * g + 0.114 * b);
        }
        return 0;
    }

    char grayToAscii(int grayValue)
    {
        if (invertColors)
        {
            grayValue = 255 - grayValue;
        }

        std::string chars = useCustomChars ? std::string(customChars) : ASCII_CHARS;
        int index = (grayValue * (chars.length() - 1)) / 255;
        index = std::max(0, std::min((int)chars.length() - 1, index));
        return chars[index];
    }

    void convertToAscii()
    {
        if (!imageData)
        {
            std::cerr << "No image data loaded!" << std::endl;
            return;
        }

        // Calculate output height maintaining aspect ratio if not specified
        if (outputHeight == 0)
        {
            // Account for terminal character aspect ratio (roughly 2:1)
            outputHeight = (height * outputWidth) / (width * 2);
        }

        std::cout << "\nASCII Art (" << outputWidth << "x" << outputHeight << "):\n"
                  << std::endl;

        for (int y = 0; y < outputHeight; y++)
        {
            for (int x = 0; x < outputWidth; x++)
            {
                // Map output coordinates to image coordinates
                int imgX = (x * width) / outputWidth;
                int imgY = (y * height) / outputHeight;

                // Sample multiple pixels for better quality (simple box filter)
                int xEnd = ((x + 1) * width) / outputWidth;
                int yEnd = ((y + 1) * height) / outputHeight;

                int totalGray = 0;
                int pixelCount = 0;

                for (int iy = imgY; iy < yEnd && iy < height; iy++)
                {
                    for (int ix = imgX; ix < xEnd && ix < width; ix++)
                    {
                        totalGray += getPixelGrayvalue(ix, iy);
                        pixelCount++;
                    }
                }

                if (pixelCount > 0)
                {
                    int avgGray = totalGray / pixelCount;
                    std::cout << grayToAscii(avgGray);
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << std::endl;
        }
    }
};

void printUsage(const char *programName)
{
    std::cout << "Usage: " << programName << " <image_path> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -w <width>     Set output width (default: 80)\n";
    std::cout << "  -h <height>    Set output height (default: auto-calculated)\n";
    std::cout << "  -i             Invert colors (light becomes dark)\n";
    std::cout << "  -c <chars>     Custom ASCII characters (from dark to light)\n";
    std::cout << "  --help         Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " image.jpg\n";
    std::cout << "  " << programName << " image.png -w 120 -h 40\n";
    std::cout << "  " << programName << " image.jpg -i -c \"██▓▒░  \"\n";
    std::cout << "  " << programName << " image.bmp -w 60\n\n";
    std::cout << "Supported formats: JPEG, PNG, BMP, TGA, PSD, GIF, HDR, PIC\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string imagePath = argv[1];

    if (imagePath == "--help" || imagePath == "-help" || imagePath == "help")
    {
        printUsage(argv[0]);
        return 0;
    }

    ImageToAscii converter;

    // Parse command line arguments
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-w" && i + 1 < argc)
        {
            int width = std::stoi(argv[++i]);
            converter.setOutputSize(width, 0);
        }
        else if (arg == "-h" && i + 1 < argc)
        {
            int height = std::stoi(argv[++i]);
            converter.setOutputSize(0, height); // Will need to be handled properly
        }
        else if (arg == "-i")
        {
            converter.setInvertColors(true);
        }
        else if (arg == "-c" && i + 1 < argc)
        {
            converter.setCustomChars(argv[++i]);
        }
        else if (arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    // Load and convert image
    if (!converter.loadImage(imagePath))
    {
        return 1;
    }

    converter.convertToAscii();

    return 0;
}