
#include "FileWatcher.hpp"
#include "logger.hpp"

std::map<std::filesystem::path, WatchListEntry> FileWatcher::watchlist = {};
uint FileWatcher::lastUpdate = 0;
uint FileWatcher::delay = 1000;

bool FileWatcher::add(std::filesystem::path path, std::function<void()> callback) {
	if (watchlist.find(path) != watchlist.end()) return false;
	auto time = std::filesystem::last_write_time(path);
	WatchListEntry entry = std::make_pair(time, callback);
	watchlist.emplace(path, entry);
	return true;
}

void FileWatcher::update(uint ticks) {
	if (ticks < lastUpdate + delay) return;
	// LOG("Watching files...");
	for (auto& p : watchlist) {
		auto time = std::filesystem::last_write_time(p.first);
		if (time != p.second.first) {
			p.second.first = time;
			p.second.second();
		}
	}
	lastUpdate = ticks;
}
