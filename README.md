# IMAGE2ASCII

A fast and flexible C++ command-line tool that converts images into ASCII art for terminal display.

## Features

- 🖼️ **Multiple Image Formats**: Supports JPEG, PNG, BMP, TGA, PSD, GIF, HDR, and PIC
- 📏 **Flexible Sizing**: Automatic aspect ratio preservation or custom dimensions
- 🎨 **Color Inversion**: Option to invert dark/light mapping for different effects
- ✨ **Custom ASCII Characters**: Use your own character set for unique styles
- 🔍 **High-Quality Conversion**: Box filtering for superior output quality
- ⚡ **Fast Processing**: Optimized C++ implementation

## Quick Start

### Prerequisites

- C++ compiler (GCC, Clang, or Visual Studio)
- `stb_image.h` header file

### Building

1. **Download the STB image library**:

   ```bash
   wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
   ```

   Or download manually and place in the project directory.

2. **Compile**:

   ```bash
   g++ main.cpp -o imagetoascii -std=c++11 -O2
   ```

3. **Run**:
   ```bash
   ./imagetoascii image.png
   ```

### Windows Users

**Using MinGW/MSYS2**:

```cmd
g++ main.cpp -o imagetoascii.exe -std=c++11 -O2
imagetoascii.exe image.jpg
```

**Using Visual Studio**:

```cmd
cl main.cpp /EHsc /O2 /Fe:imagetoascii.exe
imagetoascii.exe image.jpg
```

## Usage

```bash
imagetoascii <image_path> [options]
```

### Options

| Option        | Description                                          | Example        |
| ------------- | ---------------------------------------------------- | -------------- |
| `-w <width>`  | Set output width (default: 80)                       | `-w 120`       |
| `-h <height>` | Set output height (auto-calculated if not specified) | `-h 40`        |
| `-i`          | Invert colors (light becomes dark)                   | `-i`           |
| `-c <chars>`  | Custom ASCII characters (from dark to light)         | `-c "██▓▒░  "` |
| `--help`      | Show help message                                    | `--help`       |

### Examples

**Basic conversion**:

```bash
./imagetoascii photo.jpg
```

**Custom size**:

```bash
./imagetoascii landscape.png -w 120 -h 40
```

**Inverted colors with custom characters**:

```bash
./imagetoascii portrait.jpg -i -c "█▉▊▋▌▍▎▏ "
```

**Wide terminal output**:

```bash
./imagetoascii image.bmp -w 150
```

## File Structure

```
IMAGE2ASCII/
├── main.cpp          # Main source code
├── stb_image.h       # STB image library (download required)
├── imagetoascii.exe  # Compiled executable (Windows)
├── image.png         # Sample image file
├── build             # Build script
└── README.md         # This file
```

## Technical Details

### Image Processing

- **Grayscale Conversion**: Uses luminance formula (0.299R + 0.587G + 0.114B)
- **Sampling**: Box filtering for better quality than point sampling
- **Aspect Ratio**: Automatically accounts for terminal character proportions (2:1)

### Supported Formats

- **JPEG** (.jpg, .jpeg)
- **PNG** (.png)
- **BMP** (.bmp)
- **TGA** (.tga)
- **PSD** (.psd)
- **GIF** (.gif)
- **HDR** (.hdr)
- **PIC** (.pic)

### ASCII Character Sets

**Default**: `@%#*+=-:. ` (dark to light)

**Custom Examples**:

- **Block characters**: `█▉▊▋▌▍▎▏ `
- **Density**: `██▓▒░  `
- **Symbols**: `@&#%*+=:-. `
- **Simple**: `#*. `

## Build Script

You can create a simple build script:

**Linux/Mac** (`build.sh`):

```bash
#!/bin/bash
if [ ! -f "stb_image.h" ]; then
    echo "Downloading stb_image.h..."
    wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
fi
g++ main.cpp -o imagetoascii -std=c++11 -O2
echo "Build complete! Run with: ./imagetoascii <image>"
```

**Windows** (`build.bat`):

```batch
@echo off
if not exist stb_image.h (
    echo Downloading stb_image.h...
    curl -o stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
)
g++ main.cpp -o imagetoascii.exe -std=c++11 -O2
echo Build complete! Run with: imagetoascii.exe ^<image^>
```

## Tips for Best Results

1. **Terminal Size**: Use `-w` to match your terminal width for best fit
2. **Image Type**: High contrast images work better than low contrast ones
3. **Custom Characters**: Experiment with different character sets for artistic effects
4. **Color Inversion**: Try `-i` if the output looks inverted from your expectation
5. **Font**: Monospace fonts in your terminal give the best results

## Troubleshooting

**"Could not load image"**:

- Check file path is correct
- Ensure image format is supported
- Verify file isn't corrupted

**Compilation errors**:

- Make sure `stb_image.h` is in the same directory
- Use C++11 or later standard
- Check compiler is properly installed

**Output looks stretched**:

- The aspect ratio compensation accounts for terminal character proportions
- Try adjusting width/height manually if needed

## License

This project uses the STB image library, which is in the public domain. The main code is provided as-is for educational and personal use.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve this tool!

---

**Enjoy creating ASCII art from your images!** 🎨
