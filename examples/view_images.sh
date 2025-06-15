#!/bin/bash
# Computer Vision Examples Viewer Script

echo "🎨 Computer Vision Examples Viewer"
echo "=================================="
echo "Available images:"
ls -1 *.ppm | nl
echo ""
if [ "$1" == "" ]; then
    echo "Usage: $0 [command] [filename]"
    echo "Commands:"
    echo "  list     - List all images"
    echo "  ascii    - View as ASCII art"
    echo "  color    - View with color coding"
    echo "  analyze  - Show image statistics"
    echo "  features - Detect features"
    echo "  gallery  - Create HTML gallery"
    echo "  convert  - Convert PPM to PNG (requires ImageMagick)"
    exit 1
fi

case $1 in
    list)
        echo "Available images:"
        ls -la *.ppm
        ;;
    ascii)
        if [ "$2" != "" ]; then
            ./image_viewer_environment ascii "$2"
        else
            echo "Usage: $0 ascii <filename.ppm>"
        fi
        ;;
    color)
        if [ "$2" != "" ]; then
            ./image_viewer_environment color "$2"
        else
            echo "Usage: $0 color <filename.ppm>"
        fi
        ;;
    convert)
        if command -v convert &> /dev/null; then
            for file in *.ppm; do
                echo "Converting $file to ${file%.ppm}.png"
                convert "$file" "${file%.ppm}.png"
            done
            echo "✅ Conversion complete!"
        else
            echo "❌ ImageMagick not installed. Please install: sudo apt install imagemagick"
        fi
        ;;
    gallery)
        ./image_viewer_environment gallery
        echo "✅ Open image_gallery.html in your browser"
        ;;
    *)
        echo "Unknown command: $1"
        ;;
esac
