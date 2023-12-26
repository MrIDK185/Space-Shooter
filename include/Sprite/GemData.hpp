#ifndef GEM_DATA_HPP
#define GEM_DATA_HPP

typedef struct
{
	unsigned int
		blinkDuration,
		lifetimeDuration,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;
} GemData;

#endif //! GEM_DATA_HPP