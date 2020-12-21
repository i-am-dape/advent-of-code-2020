#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

ABSL_FLAG(bool, step, false, "require enter for every line");

std::vector<std::string> ReadInput() {   
    std::vector<std::string> lines;
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}

struct Cmd {
    char cmd;
    int val;
};

std::vector<Cmd> ReadCmds() {
    auto txts = ReadInput();
    std::vector<Cmd> cmds;
    for (auto &t : txts) {
        char cmd = t[0];
        int val = 0;
        absl::SimpleAtoi(t.substr(1), &val);

        cmds.push_back({cmd, val});
    }
    return cmds;
}

class Ship {
  public:
    Ship(bool step) : _dir(90), _vert(0), _horiz(0), _step(step) {}

    void Apply(const Cmd &c) {
        if (c.cmd == 'R' || c.cmd == 'L') {
            Turn(c);
        } else {
            Move(c);
        }
    }

    char TranslateDir() {
        switch (_dir) {
            case 0:
                return 'N';

            case 90:
                return 'E';

            case 180:
                return 'S';

            case 270:
                return 'W';

            default:
                std::cerr << "unexpected: " << _dir << std::endl;
                exit(1);
        }

        return '#';
    }

    void Move(const Cmd& c) {
        int vert_initial = _vert;
        int horiz_initial = _horiz;

        char dir = c.cmd == 'F' ? TranslateDir() : c.cmd;
        switch (dir) {
            case 'N':
                _vert += c.val;
                break;

            case 'S':
                _vert -= c.val;
                break;

            case 'E':
                _horiz += c.val;
                break;
            
            case 'W':
                _horiz -= c.val;
                break;
        }

        if (_step) {
            std::cout << c.cmd << ":" << c.val << " |" << horiz_initial << ", " << vert_initial << " => " << _horiz << ", " << _vert << std::endl;
        }
    }

    void Turn(const Cmd& c) {
        int initial = _dir;
        int dir = c.cmd == 'R' ? 1 : -1;
        int tot = _dir + (dir * c.val);
        
        if (tot >= 360) {
            tot -= 360;
        } else if (tot < 0) {
            tot += 360;
        }

        if (_step) {
            std::cout << "T:" << c.val << " |" << initial << " => " << tot << std::endl;
        }
        _dir = tot;
        
    }

    int ManhattanDistance() {
        int h = std::abs(_horiz);
        int v = std::abs(_vert);
        std::cout << h << " (" << _horiz << ") " << v << " (" << _vert << ")" << std::endl;

        return h + v;
    }

    bool _step;
    int _dir;
    int _vert;
    int _horiz;
};

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "12" << std::endl;
    auto cmds = ReadCmds();
    Ship sss(absl::GetFlag(FLAGS_step));

    for (auto &c : cmds) {        
        sss.Apply(c);
    }

    std::cout << "manhattan: " << sss.ManhattanDistance() << std::endl;

    return 0;
}
