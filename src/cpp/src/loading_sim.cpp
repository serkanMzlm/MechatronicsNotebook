#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <string>
#include <stdexcept>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

namespace ProgressBar {

    struct Config {
        int   totalSteps;
        int   barWidth;
        int   delayMs;
    };

    inline void printTimestamp(long long elapsedMs) {
        int ms      = elapsedMs % 1000;
        int secs    = static_cast<int>(elapsedMs / 1000) % 60;
        int mins    = static_cast<int>(elapsedMs / 1000) / 60;
        std::cout << "[" << std::setw(2) << std::setfill('0') << mins
                  << ":"  << std::setw(2) << std::setfill('0') << secs
                  << "."  << std::setw(3) << std::setfill('0') << ms << "]";
    }

    inline long long elapsed(const std::chrono::steady_clock::time_point& start) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
    }

    void classic(const Config& cfg) {
        std::cout << "\n";
        for (int i = 0; i <= cfg.totalSteps; ++i) {
            float pct = static_cast<float>(i) / cfg.totalSteps * 100.0f;
            int   pos = static_cast<int>(cfg.barWidth * pct / 100.0f);

            std::cout << "\r|";
            for (int j = 0; j < cfg.barWidth; ++j) {
                if      (j < pos)  std::cout << "=";
                else if (j == pos) std::cout << ">";
                else               std::cout << " ";
            }
            std::cout << "| " << std::setw(3) << i << "/" << cfg.totalSteps
                      << " [" << std::fixed << std::setprecision(2) << pct << "%]";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs));
        }
        std::cout << "\n\n";
    }

    void block(const Config& cfg) {
        std::cout << "\n";
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i <= cfg.totalSteps; ++i) {
            float pct = static_cast<float>(i) / cfg.totalSteps * 100.0f;
            int   pos = static_cast<int>(cfg.barWidth * pct / 100.0f);

            std::cout << "\r|";
            for (int j = 0; j < cfg.barWidth; ++j)
                std::cout << (j < pos ? "█" : " ");
            std::cout << "| " << std::setw(3) << i << "/" << cfg.totalSteps
                      << " [" << std::fixed << std::setprecision(2) << pct << "%] ";
            printTimestamp(elapsed(start));
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs));
        }
        std::cout << "\n\n";
    }

    void hash(const Config& cfg) {
        std::cout << "\n";
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i <= cfg.totalSteps; ++i) {
            float pct = static_cast<float>(i) / cfg.totalSteps * 100.0f;
            int   pos = static_cast<int>(cfg.barWidth * pct / 100.0f);

            std::cout << "\r|";
            for (int j = 0; j < cfg.barWidth; ++j)
                std::cout << (j < pos ? "#" : ".");
            std::cout << "| " << std::setw(3) << i << "/" << cfg.totalSteps
                      << " [" << std::fixed << std::setprecision(2) << pct << "%] ";
            printTimestamp(elapsed(start));
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs));
        }
        std::cout << "\n\n";
    }

    void colorFill(const Config& cfg) {
        std::cout << "\n";
        for (int i = 0; i <= cfg.totalSteps; ++i) {
            float       pct   = static_cast<float>(i) / cfg.totalSteps * 100.0f;
            int         pos   = static_cast<int>(cfg.barWidth * pct / 100.0f);
            const char* color = (pct <= 35.0f) ? RED : (pct <= 75.0f) ? YELLOW : GREEN;

            std::cout << "\r|";
            for (int j = 0; j < cfg.barWidth; ++j) {
                if      (j < pos)  std::cout << color << "=" << RESET;
                else if (j == pos) std::cout << color << ">" << RESET;
                else               std::cout << " ";
            }
            std::cout << "| " << std::setw(3) << i << "/" << cfg.totalSteps
                      << " [" << color << std::fixed << std::setprecision(2)
                      << pct << "%" << RESET << "]";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs));
        }
        std::cout << "\n\n";
    }

    void colorPercent(const Config& cfg) {
        std::cout << "\n";
        for (int i = 0; i <= cfg.totalSteps; ++i) {
            float       pct   = static_cast<float>(i) / cfg.totalSteps * 100.0f;
            int         pos   = static_cast<int>(cfg.barWidth * pct / 100.0f);
            const char* color = (pct <= 35.0f) ? RED : (pct <= 75.0f) ? YELLOW : GREEN;

            std::cout << "\r[" << std::setw(3) << i << "/" << cfg.totalSteps << "]|";
            for (int j = 0; j < cfg.barWidth; ++j) {
                if      (j < pos)  std::cout << "=";
                else if (j == pos) std::cout << ">";
                else               std::cout << " ";
            }
            std::cout << "|[" << color << std::fixed << std::setprecision(2)
                      << pct << "%" << RESET << "]";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.delayMs));
        }
        std::cout << "\n\n";
    }

}

struct MenuItem {
    std::string               label;
    std::function<void()>     action;
};

static void printMenu(const std::vector<MenuItem>& items) {
    std::cout << BOLD << CYAN
              << "┌─────────────────────────────────────────┐\n"
              << "│       LOADING BAR SİMÜLATÖRÜ            │\n"
              << "└─────────────────────────────────────────┘\n"
              << RESET;
    for (std::size_t i = 0; i < items.size(); ++i)
        std::cout << "  [" << (i + 1) << "] " << items[i].label << "\n";
    std::cout << "  [0] Çıkış\n\n"
              << "Seçiminiz: ";
}

int main() {
    const ProgressBar::Config cfg { 55, 50, 90 };

    const std::vector<MenuItem> menu = {
        { "Classic  ( |===>    | 23/55 [41.81%] )",              [&]{ ProgressBar::classic(cfg);      } },
        { "Block    ( |████    |       [41.81%] [00:02.100] )",   [&]{ ProgressBar::block(cfg);        } },
        { "Hash     ( |###.... |       [41.81%] [00:02.100] )",   [&]{ ProgressBar::hash(cfg);         } },
        { "ColorFill( |====>   | renkli çubuk )",                 [&]{ ProgressBar::colorFill(cfg);    } },
        { "ColorPct ( |[23/55] renkli yüzde )",                   [&]{ ProgressBar::colorPercent(cfg); } },
    };

    while (true) {
        std::cout << "\n";
        printMenu(menu);

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << RED << "Geçersiz giriş.\n" << RESET;
            continue;
        }

        if (choice == 0) {
            std::cout << CYAN << "Çıkılıyor...\n" << RESET;
            break;
        }

        if (choice < 1 || choice > static_cast<int>(menu.size())) {
            std::cout << RED << "Lütfen geçerli bir seçenek girin.\n" << RESET;
            continue;
        }

        std::cout << BOLD << "\n>> " << menu[choice - 1].label << "\n" << RESET;
        menu[choice - 1].action();
    }

    return 0;
}