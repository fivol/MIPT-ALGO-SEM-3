#include <iostream>
#include <vector>


class Game {
private:
    std::vector<int> dp;
    std::vector<int> was;
    int n = 0;
public:
    explicit Game(int n) : n(n), dp(n + 1), was(n + 1) {}

    void Calculate();

    bool CanFirstPlayerWin() {
        return dp[n];
    }

    std::vector<int> GetWinningSteps();
};


int main() {

    int n;
    std::cin >> n;
    Game game(n);

    game.Calculate();

    if (game.CanFirstPlayerWin()) {
        auto moves = game.GetWinningSteps();
        std::cout << "Schtirlitz\n";
        for (auto step: moves) {
            std::cout << step << '\n';
        }
    } else {
        std::cout << "Mueller";
    }
}

std::vector<int> Game::GetWinningSteps() {
    std::vector<int> result;
    for (int i = 0; i < n; i++) {
        if (!(dp[i] ^ dp[n - i - 1])) {
            result.emplace_back(i + 1);
        }
    }
    return result;
}

void Game::Calculate() {
    dp[0] = 0;
    dp[1] = 0;
    int counter = 0;

    for (int k = 2; k <= n; k++) {
        counter++;
        for (int i = 0; i < k; i++) {
            was[dp[i] ^ dp[k - i - 1]] = counter;
        }
        while (was[dp[k]] == counter) {
            dp[k]++;
        }
    }
}
