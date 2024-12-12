
#pragma once
#include <filesystem>
#include <map>

typedef std::pair<std::filesystem::file_time_type, std::function<void()>> WatchListEntry;

class FileWatcher {
public:
	static bool add(std::filesystem::path path, std::function<void()> callback);
	static void update(uint ticks);
private:
	static std::map<std::filesystem::path, WatchListEntry> watchlist;
	static uint lastUpdate;
	static uint delay;
};
