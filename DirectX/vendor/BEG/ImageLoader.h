#pragma once
#include "Image.h"

#include "../stb_image/stb_image.h"

#include <string>

namespace BeastEngine::Utils
{
	template <typename Enum>
		constexpr auto getUnderlyingType(Enum e) noexcept
	{
		return static_cast<std::underlying_type_t<Enum>>(e);
	}

	class ImageLoader
	{
	public:
		/**
		 * @brief Loads image from given filepath and transforms it into the Image class.
		 *	It loads image pixels in a format specified as desiredPixelsFormat.
		 *	If PixelsFormat::BE_AUTO is set as desiredPixelsFormat,
		 *		pixels are loaded in the same format as the given file provides (i.e. it automatically detects the pixels format).
		 *	I.e. if pixels in the image contain red, green and blue for each pixel,
		 *		returned Image object will have its PixelsFormat set to BE_RGB
		 *	If flipYAxis is set to true, Y coordinates of the texture will be flipped,
		 *		meaning: Y(0) coordinate will be Y(height), and Y(height) will be Y(0)
		 *
		 * @param const std::string& imageFilepath
		 * @param const PixelsFormat desiredPixelsFormat
		 * @param const bool flipYAxis = true
		 *
		 * @return Image
		 */
		static Image loadImage(const std::string& imageFilepath, const PixelsFormat desiredPixelsFormat, const bool flipYAxis = true)
		{
			int width, height, actualPixelsFormat;

			if (flipYAxis) {
				stbi_set_flip_vertically_on_load(1);
			}

			const auto imageData
				= stbi_load(
					imageFilepath.c_str(),
					&width,
					&height,
					&actualPixelsFormat,
					getUnderlyingType(desiredPixelsFormat)
				);

			if (imageData == nullptr) {
				stbi_image_free(imageData);
				throw std::exception(std::string("An error occurred during " + imageFilepath + " image loading. Reason: " + stbi_failure_reason()).c_str());
			}

			const Image image = ImageLoader::createImage(actualPixelsFormat, desiredPixelsFormat, width, height, imageData);

			stbi_set_flip_vertically_on_load(0);
			stbi_image_free(imageData);

			return image;
		}

	private:
		/**
		 * @brief Creates Image object from given parameters
		 *
		 * @param const int actualPixelsFormat
		 * @param const PixelsFormat desiredPixelsFormat
		 * @param const int width
		 * @param const int height
		 * @param const unsigned char* imageData
		 *
		 * @return Image
		 */
		static Image createImage(
			const int actualPixelsFormat,
			const PixelsFormat desiredPixelsFormat,
			const int width,
			const int height,
			const unsigned char* imageData
		)
		{
			using T = std::vector<unsigned char>::size_type;

			const int numberOfChannels =
				static_cast<std::size_t>((desiredPixelsFormat == PixelsFormat::BE_AUTO) ? actualPixelsFormat : getUnderlyingType(desiredPixelsFormat));

			std::vector<unsigned char> pixels(static_cast<T>(width) * static_cast<T>(height) * numberOfChannels);
			memcpy(&pixels[0], imageData, pixels.size());

			return Image(width, height, pixels, static_cast<PixelsFormat>(numberOfChannels));
		}
	};
}
