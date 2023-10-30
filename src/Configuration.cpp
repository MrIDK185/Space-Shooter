#include "Configuration.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>
#include <boost/algorithm/string.hpp>

//* private

std::pair<std::string, std::string> Configuration::SplitKeyValue(const std::string &string)
{
	std::string key;
	std::string value;
	size_t index = string.find("=");

	key = string.substr(0, index);
	boost::algorithm::trim(key);

	value = string.substr(index + 1, string.length() - (index + 1));
	boost::algorithm::trim(value);
	boost::replace_all(value, "\\n", "\n");

	return std::pair<std::string, std::string>(key, value);
}

void Configuration::ApplySetting(const std::string &key, const std::string &value)
{
	auto itUint = UIntMap.find(key);
	if (itUint != UIntMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		if (value.find('-') != std::string::npos)
		{
			throw std::out_of_range("");
			return;
		}

		*(itUint->second) = static_cast<unsigned int>(std::stoul(value));
		return;
	}

	auto itColor = colorMap.find(key);
	if (itColor != colorMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		if (value.find('-') != std::string::npos)
		{
			throw std::out_of_range("");
			return;
		}

		std::cout << "Initializing " << key << "...\n";
		int start_pos = (value[0] == '#' ? 1 : 0);
		*(itColor->second) = std::stoul(value.substr(start_pos), nullptr, 16);
		return;
	}

	auto itFloat = floatMap.find(key);
	if (itFloat != floatMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		*(itFloat->second) = std::stof(value);
		return;
	}

	auto itString = stringMap.find(key);
	if (itString != stringMap.end())
	{
		std::cout << "Initializing " << key << "...\n";
		*(itString->second) = value;
		return;
	}

	auto itWString = wstringMap.find(key);
	if (itWString != wstringMap.end())
	{
		std::cout << "Initializing " << key << "...\n";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		*(itWString->second) = converter.from_bytes(value);
		return;
	}
}

//* public

Configuration::Configuration()
{
	UIntMap["Window Width"] = &START_WINDOW_WIDTH;
	UIntMap["Window Height"] = &START_WINDOW_HEIGHT;

	UIntMap["Countdown Duration Milliseconds"] = &COUNTDOWN_DURATION_MILLISECONDS;
	UIntMap["Countdown Interval Milliseconds"] = &COUNTDOWN_INTERVAL_MILLISECONDS;

	UIntMap["Start Text Size"] = &START_TEXT_SIZE;
	UIntMap["Score Text Size"] = &SCORE_TEXT_SIZE;
	UIntMap["Start Score"] = &START_SCORE;

	UIntMap["Player Frame Width"] = &PLAYER_FRAME_WIDTH;
	UIntMap["Player Frame Height"] = &PLAYER_FRAME_HEIGHT;
	UIntMap["Player IMG Frames"] = &PLAYER_IMG_FRAMES;
	UIntMap["Player IMG Types"] = &PLAYER_IMG_TYPES;
	UIntMap["Player Animations Per Second"] = &PLAYER_ANIMATIONS_PER_SECOND;
	UIntMap["Player Effect Duration Seconds"] = &PLAYER_EFFECT_DURATION_SECONDS;
	UIntMap["Player Rotation Speed"] = &PLAYER_ROTATION_SPEED;

	UIntMap["Gem Frame Width"] = &GEM_FRAME_WIDTH;
	UIntMap["Gem Frame Height"] = &GEM_FRAME_HEIGHT;
	UIntMap["Gem IMG Frames"] = &GEM_IMG_FRAMES;
	UIntMap["Gem IMG Types"] = &GEM_IMG_TYPES;
	UIntMap["Gem Blink Factor"] = &GEM_BLINK_FACTOR;
	UIntMap["Gem Blink Duration"] = &GEM_BLINK_DURATION;
	UIntMap["Gem Lifetime Duration"] = &GEM_LIFETIME_DURATION;
	UIntMap["Gem Maximum Brightness"] = &GEM_MAXIMUM_BRIGHTNESS;
	UIntMap["Gem Minimum Brightness"] = &GEM_MINIMUM_BRIGHTNESS;

	colorMap["Font Color"] = &FONT_COLOR_HEX;

	floatMap["Player Max Velocity Boost"] = &PLAYER_MAX_VELOCITY_BOOST;
	floatMap["Player Acceleration Boost"] = &PLAYER_ACCELEARION_BOOST;
	floatMap["Player Acceleration"] = &PLAYER_ACCELEARION;
	floatMap["Player Max Velocity"] = &PLAYER_MAX_VELOCITY;
	floatMap["Player Friction"] = &PLAYER_FRICTION;
	floatMap["Player Scale"] = &PLAYER_SCALE;
	floatMap["Player Radius"] = &PLAYER_RADIUS;

	floatMap["Gem Scale"] = &GEM_SCALE;
	floatMap["Gem Radius"] = &GEM_RADIUS;

	stringMap["Font Path"] = &FONT_PATH;

	wstringMap["Start Text"] = &START_TEXT;

	return;
}

Configuration::~Configuration()
{
	UIntMap.clear();
	colorMap.clear();
	floatMap.clear();
	stringMap.clear();
	wstringMap.clear();

	return;
}

void Configuration::ImportSettings()
{
	std::cout << "Opening settings file...\n";
	std::ifstream file("config/settings.txt", std::ios_base::in);
	std::string line;

	if (!file)
	{
		std::cerr << "Could not find setting file\nClosing settings file...\n\n";
		file.close();

		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		const auto [key, value] = SplitKeyValue(line);

		try
		{
			ApplySetting(key, value);
		}

		catch (const std::invalid_argument &error)
		{
			std::cerr << "Error: Invalid argument at: " << error.what() << "\n";
			continue;
		}
		catch (const std::out_of_range &error)
		{
			std::cerr << "Error: argument out of range at: " << error.what() << "\n";
			continue;
		}
	}

	std::cout << "Closing settings file...\n\n";
	file.close();

	return;
}