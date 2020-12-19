#include <iostream>
#include <vector>
#include <set>
#include <cstring>


struct Coord {
    int x = 0, y = 0;

    Coord(int x, int y) : x(x), y(y) {}

    Coord &operator+=(const Coord &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Coord operator+(const Coord &other) {
        Coord sum = *this;
        sum += other;
        return sum;
    }

    Coord() = default;

    ~Coord() = default;
};

class TerminatorProblemSolver {
public:
    static const int bs = 3;
    static const int bs2 = bs * bs;

    Coord cell_shifts[8];

    bool _board[bs2]{};
    bool _visited[bs2][bs2][2]{};
    bool _winnable[bs2][bs2][2]{};
    bool _known[bs2][bs2][2]{};
    int _neighbors[bs2][bs2][2]{};
    int _init_term = 0;
    int _init_runner = 0;
public:

    TerminatorProblemSolver() {
        _init_fields();
    };

    ~TerminatorProblemSolver() = default;

    void ReadInitState();

    bool CanRunnerWin();

private:

    static int _posFromXY(int x, int y) {
        return x * bs + y;
    }

    static Coord _coordFromInt(int pos) {
        return Coord(pos / bs, pos % bs);
    }

    [[nodiscard]] bool _isCellFree(const Coord &p) const;

    [[nodiscard]] bool _isIntersectY(const Coord &p1, const Coord &p2) const {
        if (p1.x != p2.x) {
            return false;
        }
        for (int y = p1.y; y != p2.y; y += (p2.y > p1.y) ? 1 : -1) {
            if (_board[_posFromXY(p1.x, y)]) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool _isIntersectX(const Coord &p1, const Coord &p2) const {
        if (p1.y != p2.y) {
            return false;
        }
        for (int x = p1.x; x != p2.x; x += (p2.x > p1.x) ? 1 : -1) {
            if (_board[_posFromXY(x, p1.y)]) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool _isIntersectDiag(const Coord &p1, const Coord &p2) const {
        if (std::abs(p1.x - p2.x) != std::abs(p1.y - p2.y)) {
            return false;
        }
        for (int x = p1.x, y = p1.y; x != p2.x;
             (x += (p2.x > p1.x) ? 1 : -1),
                     (y += (p2.y > p1.y) ? 1 : -1)) {
            if (_board[_posFromXY(x, y)]) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool _isObstacle(const Coord &p1, const Coord &p2) const;

    [[nodiscard]] bool _isPlayersOnLine(int player1, int player2) const;

    void _handle_cell(int runner, int term, bool turn);

    void _init_fields();

    void _initialize();
};


int main() {
    TerminatorProblemSolver game_solver;
    game_solver.ReadInitState();

    if (game_solver.CanRunnerWin()) {
        std::cout << 1 << '\n';
    } else {
        std::cout << -1 << '\n';
    }

    return 0;
}

void TerminatorProblemSolver::ReadInitState() {
    std::string row;
    for (int y = 0; y < bs; ++y) {
        std::cin >> row;
        for (int x = 0; x < bs; ++x) {
            int cell = row[x] - '0';
            int pos = _posFromXY(x, y);
            if (cell == 1) {
                _board[pos] = true;
            } else if (cell == 2) {
                _init_runner = pos;
            } else if (cell == 3) {
                _init_term = pos;
            }
        }
    }
}

bool TerminatorProblemSolver::CanRunnerWin() {
    _initialize();
    int c = 0;
    for (int i = 0; i < bs2; ++i) {
        for (int j = 0; j < bs2; ++j) {
            for (int k = 0; k < 2; ++k) {
                c += _known[i][j][k] * i;
            }
        }
    }
    for (int turn = 0; turn < 2; ++turn) {
        for (int runner = 0; runner < bs2; ++runner) {
            for (int term = 0; term < bs2; ++term) {
                if (_known[runner][term][turn] && !_visited[runner][term][turn]) {
                    _handle_cell(runner, term, turn);
                }
            }
        }
    }
    return _winnable[_init_runner][_init_term][0];
}

void TerminatorProblemSolver::_init_fields() {
    const int x_shift[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    const int y_shift[8] = {0, 0, 1, -1, 1, -1, 1, -1};
    for (int k = 0; k < 8; ++k) {
        cell_shifts[k] = Coord(x_shift[k], y_shift[k]);
    }
    memset(_board, 0, sizeof(_board));
    memset(_known, 0, sizeof(_known));
    memset(_visited, 0, sizeof(_visited));
    memset(_winnable, 0, sizeof(_winnable));
    memset(_neighbors, 0, sizeof(_neighbors));
}

void TerminatorProblemSolver::_initialize() {
    for (int runner = 0; runner < bs2; ++runner) {
        for (int term = 0; term < bs2; ++term) {
            if (!_isCellFree(_coordFromInt(runner)) || !_isCellFree(_coordFromInt(term))) {
                continue;
            }
            for (int turn = 0; turn < 2; ++turn) {
                if (_isPlayersOnLine(runner, term)) {
                    _known[runner][term][turn] = true;
                    _winnable[runner][term][turn] = turn;
                } else if (_coordFromInt(runner).y == bs - 1) {
                    _known[runner][term][0] = true;
                    _winnable[runner][term][0] = true;
                }
                for (auto shift: cell_shifts) {
                    Coord player = shift;
                    if (turn) {
                        player += _coordFromInt(term);
                    } else {
                        player += _coordFromInt(runner);
                    }
                    if (_isCellFree(player)) {
                        ++_neighbors[runner][term][turn];
                    }
                }
            }

        }
    }
}

void TerminatorProblemSolver::_handle_cell(int runner, int term, bool turn) {
    _visited[runner][term][turn] = true;

    for (auto shift: cell_shifts) {
        int new_runner = runner;
        int new_term = term;
        Coord runner_c = _coordFromInt(runner);
        Coord term_c = _coordFromInt(term);
        if (turn) {
            new_runner += _posFromXY(shift.x, shift.y);
            runner_c += shift;
        } else {
            new_term += _posFromXY(shift.x, shift.y);
            term_c += shift;
        }
        if (!_isCellFree(runner_c)
            || !_isCellFree(term_c)
            || _visited[new_runner][new_term][!turn]) {
            continue;
        }
        if (!_known[new_runner][new_term][!turn]) {
            if (!_winnable[runner][term][turn]) {
                _winnable[new_runner][new_term][!turn] = true;
                _known[new_runner][new_term][!turn] = true;
            } else if (--_neighbors[new_runner][new_term][!turn] == 0) {
                _winnable[new_runner][new_term][!turn] = false;
                _known[new_runner][new_term][!turn] = true;
            }
        }
        _handle_cell(new_runner, new_term, !turn);
    }
}

bool TerminatorProblemSolver::_isPlayersOnLine(int player1, int player2) const {
    Coord p1 = _coordFromInt(player1);
    Coord p2 = _coordFromInt(player2);
    return !_isObstacle(p1, p2);
}

bool TerminatorProblemSolver::_isObstacle(const Coord &p1, const Coord &p2) const {
    return !(_isIntersectX(p1, p2) || _isIntersectY(p1, p2) || _isIntersectDiag(p1, p2));
}

bool TerminatorProblemSolver::_isCellFree(const Coord &p) const {
    if (p.x < 0 || p.x >= bs || p.y < 0 || p.y >= bs) {
        return false;
    }
    return !_board[_posFromXY(p.x, p.y)];
}
