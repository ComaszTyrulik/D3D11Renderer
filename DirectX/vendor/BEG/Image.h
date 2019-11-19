#pragma once
#include <utility>
#include <vector>

#include "../stb_image/stb_image.h"

namespace BeastEngine::Utils
{
	enum class PixelsFormat : int
	{
		BE_AUTO = STBI_default,
		BE_GRAY = STBI_grey,
		BE_GRAY_ALPHA = STBI_grey_alpha,
		BE_RGB = STBI_rgb,
		BE_RGBA = STBI_rgb_alpha
	};

	class Image
	{
	public:
		/**
		 * @brief Constructor. Creates Image from given data
		 *
		 * @param const glm::ivec2& dimensions
		 * @param std::vector<BEPixel> pixels
		 * @param const PixelsFormat pixelsFormat
		 */
		Image(int width, int height, std::vector<unsigned char> pixels, const PixelsFormat pixelsFormat)
			: m_dimensions(std::make_pair(width, height)), m_pixels(std::move(pixels)), m_pixelsFormat(pixelsFormat)
		{}

		/**
		 * @brief Returns image dimensions
		 *
		 * @return const std::pair<int, int>&
		 */
		const std::pair<int, int>& getDimensions() const noexcept
		{
			return m_dimensions;
		}

		/**
		 * @brief Returns image pixels
		 *
		 * @return const std::vector<BEPixel>&
		 */
		const std::vector<unsigned char>& getPixels() const noexcept
		{
			return m_pixels;
		}

		/**
		 * @brief Returns format of a single pixel in the image data
		 *
		 * @return PixelsFormat
		 */
		PixelsFormat getPixelsFormat() const noexcept
		{
			return m_pixelsFormat;
		}

	private:
		/** image dimensions */
		std::pair<int, int> m_dimensions;

		/** image data */
		std::vector<unsigned char> m_pixels;

		/** format of each pixel in the image */
		PixelsFormat m_pixelsFormat;
	};
}
