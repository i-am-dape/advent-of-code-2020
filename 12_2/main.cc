#include <iostream>
#include <set>
#include <string>
#include <sstream>
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
    std::string txt;
};

std::vector<Cmd> ReadCmds() {
    auto txts = ReadInput();
    std::vector<Cmd> cmds;
    for (auto &t : txts) {
        char cmd = t[0];
        int val = 0;
        absl::SimpleAtoi(t.substr(1), &val);

        cmds.push_back({cmd, val, t});
    }
    return cmds;
}

struct Coordinate {
    int x;
    int y;

    std::string str() {
        std::ostringstream txt;
        txt << "(" << x << ", " << y << ")";
        return txt.str();
    }
};

class Ship {
  public:
    Ship(bool step) : _step(step), _ship({0, 0}), _wp({10, 1}) {}

    void Apply(const Cmd &c) {
        if (c.cmd == 'R' || c.cmd == 'L') {
            Turn(c);
        } else if (c.cmd != 'F') {
            MoveWaypoint(c);
        } else {
            Move(c);
        }
    }   

    void MoveWaypoint(const Cmd& c) {
        TraceStart(c);

        switch (c.cmd) {
            case 'N':
                _wp.y += c.val;
                break;

            case 'S':
                _wp.y -= c.val;
                break;

            case 'E':
                _wp.x += c.val;
                break;
            
            case 'W':
                _wp.x -= c.val;
                break;
        }
                
        TraceEnd();
    }

    void TraceStart(const Cmd& c) {
        std::cout << c.txt << ":" << str() << "==>";
    }

    void TraceEnd() {
        std::cout << str() << std::endl;
    }

    Coordinate Rotate(Coordinate c, char dir, int angle) {
        if (angle == 0) {
            return c;
        }

        if (dir == 'R') {
            if (c.x >= 0) {
                if (c.y >= 0) {
                    // TR
                    return Rotate({c.y, -c.x}, dir, angle - 90);
                } else {
                    // BR
                    return Rotate({c.y, -c.x}, dir, angle - 90);
                }
            } else {
                if (c.y >= 0) {
                    // TL
                    return Rotate({c.y, -c.x}, dir, angle - 90);
                } else {
                    // BL
                    return Rotate({c.y, -c.x}, dir, angle - 90);
                }
            }
        } else {
            if (c.x >= 0) {
                if (c.y >= 0) {
                    // TR
                    return Rotate({-c.y, c.x}, dir, angle - 90);
                } else {
                    // BR
                    return Rotate({-c.y, c.x}, dir, angle - 90);
                }
            } else {
                if (c.y >= 0) {
                    // TL
                    return Rotate({-c.y, c.x}, dir, angle - 90);
                } else {
                    // BL
                    return Rotate({-c.y, c.x}, dir, angle - 90);
                }
            }
        }     
    }

    void Turn(const Cmd& c) {
        TraceStart(c);
        Coordinate delta_wp = {_wp.x - _ship.x, _wp.y - _ship.y};
        std::cout << " wp:" << delta_wp.str();
        Coordinate delta_wp_rotated = Rotate(delta_wp, c.cmd, c.val);
        std::cout << " rot wp:" << delta_wp_rotated.str();
        _wp = {delta_wp_rotated.x + _ship.x, delta_wp_rotated.y + _ship.y};

        TraceEnd();
    }

    void Move(const Cmd& c) {        
        TraceStart(c);

        int dx = (_wp.x - _ship.x) * c.val;
        int dy = (_wp.y - _ship.y) * c.val;
        _ship.x += dx;
        _ship.y += dy;
        _wp.x += dx;
        _wp.y += dy;

        TraceEnd();
    }

    int ManhattanDistance() {
        int x = std::abs(_ship.x);
        int y = std::abs(_ship.y);
        std::cout << x << " (" << _ship.x << ") " << x << " (" << _ship.y << ")" << std::endl;

        return x + y;
    }

    std::string str() {
        std::ostringstream txt;
        txt << "{ship:" << _ship.str() << " wp:" << _wp.str() << "}";
        return txt.str();
    }

    bool _step;
    Coordinate _ship;
    Coordinate _wp;
};

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "12-2" << std::endl;
    auto cmds = ReadCmds();
    Ship sss(absl::GetFlag(FLAGS_step));

    for (auto &c : cmds) {        
        sss.Apply(c);
    }

    std::cout << "manhattan: " << sss.ManhattanDistance() << std::endl;

    return 0;
}
