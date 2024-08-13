#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::trace("spdlog trace");
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("spdlog debug");
    spdlog::info("spdlog info");
    spdlog::error("spdlog warning");
    spdlog::warn("spdlog error");
    spdlog::critical("spdlog critical");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::info("spdlog info");
}
