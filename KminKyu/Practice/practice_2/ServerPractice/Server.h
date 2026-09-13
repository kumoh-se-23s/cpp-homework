#pragma once

class Server {
public:
    Server(char letterName);
    static int setTurn();
    void serverOne() const;
    static bool stillOpen();
private:
    static int turn;
    static int lastServed;
    static bool nowOpen;
    char name;
};
