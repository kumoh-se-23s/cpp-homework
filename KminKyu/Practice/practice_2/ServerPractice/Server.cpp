#include<iostream>
#include "Server.h"

using namespace std;

int Server::turn = 0;
int Server::lastServed = 0;
bool Server::nowOpen = true;

Server::Server(char letterName) : name(letterName){}

int Server::setTurn() {
    turn++;
    return turn;
}

bool Server::stillOpen() {
    return nowOpen;
}

void Server::serverOne() const {
    if (nowOpen && lastServed < turn) {
        lastServed++;
        cout << "Server" << name << " now Serving " << lastServed << endl;
    }
    if (lastServed >= turn) nowOpen = false;
}
