
#pragma once
#include "utils.hpp"

#define MINUTE 1000

struct Time {
	int day, hour, minute;
	uint rest;

	Time()
		: day(0), hour(6), minute(0), rest(0) {}

	void update(uint dt) {
		rest += dt;
		minute += rest / MINUTE;
		rest %= MINUTE;
		hour += minute / 60;
		minute %= 60;
		day += hour / 24;
		hour %= 24;
	}

	std::string string() {
		std::string hourString = std::string(hour < 10, '0') + std::to_string(hour);
		std::string minuteString = std::string(minute < 10, '0') + std::to_string(minute);
		return "Day " + std::to_string(day) + ", " + hourString + ":" + minuteString;
	}

	int mins() {
		return 60 * hour + minute;
	}
};