#pragma once

class DDayAPP {
public:
    DDayAPP() {}
    bool menu(const char command[]);
    void run();
    void PrintMenu();
private:
    DDay dday;
};