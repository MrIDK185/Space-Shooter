#include "Game/Configuration.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>
#include <boost/algorithm/string.hpp>

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

void Configuration::ApplySetting(const std::string &key, std::string &value)
{
	auto intPair = UIntMap.find(key);
	if (intPair != UIntMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		if (value.find('-') != std::string::npos)
		{
			throw std::out_of_range("");
			return;
		}

		*(intPair->second) = static_cast<unsigned int>(std::stoul(value));
		return;
	}

	auto floatPair = floatMap.find(key);
	if (floatPair != floatMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		*(floatPair->second) = std::stof(value);
		return;
	}

	auto stringPair = stringMap.find(key);
	if (stringPair != stringMap.end())
	{
		std::cout << "Initializing " << key << "...\n";
		*(stringPair->second) = value;
		return;
	}

	auto wstringPair = wstringMap.find(key);
	if (wstringPair != wstringMap.end())
	{
		std::cout << "Initializing " << key << "...\n";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		*(wstringPair->second) = converter.from_bytes(value);
		return;
	}

	auto colorPair = colorMap.find(key);
	if (colorPair != colorMap.end())
	{
		std::cout << "Initializing " << key << " ... \n";
		if (value.find('-') != std::string::npos)
		{
			throw std::out_of_range("");
			return;
		}

		boost::replace_first(value, "#", "");
		unsigned int hex_value = std::stoul(value, nullptr, 16);

		Uint8 r = ((hex_value >> 16) & 0xFF);
		Uint8 g = ((hex_value >> 8) & 0xFF);
		Uint8 b = (hex_value & 0xFF);
		*(colorPair->second) = {r, g, b, 255};
		return;
	}

	return;
}

Configuration::Configuration()
{
	UIntMap["Window Width"] = &START_WINDOW_WIDTH;
	UIntMap["Window Height"] = &START_WINDOW_HEIGHT;

	UIntMap["Countdown Duration Milliseconds"] = &COUNTDOWN_DURATION_MILLISECONDS;
	UIntMap["Countdown Interval Milliseconds"] = &COUNTDOWN_INTERVAL_MILLISECONDS;

	UIntMap["Start Score"] = &START_SCORE;

	UIntMap["Start Text Size"] = &START_TEXT_SIZE;
	UIntMap["Game Over Text Size"] = &GAME_OVER_TEXT_SIZE;
	UIntMap["Ingame Text Size"] = &INGAME_TEXT_SIZE;

	UIntMap["Player Radius"] = &PLAYER_RADIUS;

	UIntMap["Player Friction"] = &PLAYER_FRICTION;
	UIntMap["Player Max Velocity"] = &PLAYER_MAX_VELOCITY;
	UIntMap["Player Max Velocity Boost"] = &PLAYER_MAX_VELOCITY_BOOST;
	UIntMap["Player Acceleration"] = &PLAYER_ACCELEARION;
	UIntMap["Player Acceleration Boost"] = &PLAYER_ACCELEARION_BOOST;
	UIntMap["Player Rotation Speed"] = &PLAYER_ROTATION_SPEED;

	UIntMap["Player Frame Width"] = &PLAYER_FRAME_WIDTH;
	UIntMap["Player Frame Height"] = &PLAYER_FRAME_HEIGHT;
	UIntMap["Player IMG Frames"] = &PLAYER_IMG_FRAMES;
	UIntMap["Player IMG Types"] = &PLAYER_IMG_TYPES;
	UIntMap["Player Animations Per Second"] = &PLAYER_ANIMATIONS_PER_SECOND;
	UIntMap["Player Effect Duration Milliseconds"] = &PLAYER_EFFECT_DURATION_MILLISECONDS;

	UIntMap["Gem Radius"] = &GEM_RADIUS;

	UIntMap["Gem Blink Duration Milliseconds"] = &GEM_BLINK_DURATION_MILLISECONDS;
	UIntMap["Gem Lifetime Duration Milliseconds"] = &GEM_LIFETIME_DURATION_MILLISECONDS;
	UIntMap["Gem Blink Factor"] = &GEM_BLINK_FACTOR;
	UIntMap["Gem Minimum Brightness"] = &GEM_MINIMUM_BRIGHTNESS;
	UIntMap["Gem Maximum Brightness"] = &GEM_MAXIMUM_BRIGHTNESS;

	UIntMap["Gem Frame Width"] = &GEM_FRAME_WIDTH;
	UIntMap["Gem Frame Height"] = &GEM_FRAME_HEIGHT;
	UIntMap["Gem IMG Frames"] = &GEM_IMG_FRAMES;
	UIntMap["Gem IMG Types"] = &GEM_IMG_TYPES;

	floatMap["Gem Scale"] = &GEM_SCALE;
	floatMap["Player Scale"] = &PLAYER_SCALE;

	stringMap["Font Path"] = &FONT_PATH;

	wstringMap["Start Text"] = &START_TEXT;
	wstringMap["Game Over Text"] = &GAME_OVER_TEXT;

	colorMap["Start Text Color"] = &START_TEXT_COLOR;
	colorMap["Game Over Text Color"] = &GAME_OVER_TEXT_COLOR;
	colorMap["Ingame Text Color"] = &INGAME_TEXT_COLOR;

	return;
}

Configuration::~Configuration()
{
	UIntMap.clear();
	floatMap.clear();
	stringMap.clear();
	wstringMap.clear();
	colorMap.clear();

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

		auto [key, value] = SplitKeyValue(line);

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