#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

class Link {
private:
    int _x;
    int _y;
    bool _breakable;
    bool _alive;
public:
    Link(int x, int y): _x(x), _y(y) {}

    int getX() {
        return (_x);
    }

    int getY() {
        return (_y);
    }

    void set(int x, int y) {
        _x = x;
        _y = y;
    }

    void setBreakable(bool breakable) {
        _breakable = breakable;
    }

    bool getBreakable() {
        return (_breakable);
    }

    bool kill() {
        if (_breakable && _alive) {
            _alive = false;
            return (true);
        }
        return (false);
    }

};

class Node {
private:
    std::vector<int>    _links;
    int                 _value;
    int                 _dist;
    int                 _id;
    int                 _gatewayContact;
    bool                _isGateway;
    bool                _isAgent;

public:
    Node(int id, bool isGateway) {
        _id = id;
        _isGateway = isGateway;
        _gatewayContact = 0;
        _value = 0;
        _dist = 2;
        _isAgent = false;
    }

    void setValue(int value) {
        _value = value;
    }

    void changeValue(int value) {
        _value += value;
    }

    int getValue() {
        return (_value);
    }

    int getId() {
        return (_id);
    }

    void setGateway(bool isGateway) {
        _isGateway = isGateway;
    }

    bool isGateway() {
        return (_isGateway);
    }

    bool isAgent() {
        return (_isAgent);
    }

    void setGatewayContact(std::vector<Node> nodes) {
        for (int i = 0; i < _links.size(); i++) {
            if (nodes[_links[i]].isGateway()) {
                _gatewayContact++;
            }
        }
    }

    int getGatewayContact() {
        return (_gatewayContact);
    }

    void setLinks(std::vector<Link> links) {
        for (int i = 0; i < links.size(); i++) {
            if (links[i].getX() == _id) {
                _links.push_back(links[i].getY());
            }
            if (links[i].getY() == _id) {
                _links.push_back(links[i].getX());
            }
        }
        std::sort(_links.begin(), _links.end());
    }

    void addLink(Link link) {
        if (link.getX()) {
            _links.push_back(link.getY());
        }
        if (link.getY()) {
            _links.push_back(link.getX());
        }
    }

    void removeLink(int target) {
        for (int i = 0; i < _links.size(); i++) {
            if (_links[i] == target) {
                _links.erase(_links.begin()+i);
            }
        }
    }

    std::vector<int> getLinks() {
        return (_links);
    }

    int getNumberOfLinks() {
        return (_links.size());
    }

    void setDist(int dist) {
        _dist = dist;
    }

    int getDist() {
        return (_dist);
    }

    int isUrgent(std::vector<Node> nodes) {
        for (int i = 0; i < _links.size(); i++) {
            if (nodes[_links[i]].isGateway()) {
                return (_links[i]);
            }
        }
        return (-1);
    }
};

class Game {
public:
    static std::vector<Node> setValue(std::vector<Node> nodes, std::vector<Link> links, std::vector<Node> gateways) {
        std::vector<int> l;
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].setValue(0);
            nodes[i].setGatewayContact(nodes);
        }
        for (int i = 0; i < gateways.size(); i++) {
            int id = gateways[i].getId();
            l = nodes[id].getLinks();
            for (int j = 0; j < l.size(); j++) {
                nodes[l[j]].changeValue(1);
            }
        }
        for (int k = 0; k < 2; k++) {
            for (int i = 0; i < nodes.size(); i++) {
                l = nodes[i].getLinks();
                for (int j = 0; j < l.size(); j++) {
                    if (nodes[l[j]].isGateway() == false && nodes[i].getValue() - 1 > nodes[l[j]].getValue()) {
                        nodes[l[j]].changeValue(1);
                    }
                }
            }
        }
        return (nodes);
    }

    static std::vector<Node> endTurn(std::vector<Node> nodes, int node1, int node2) {
        nodes[node1].removeLink(node2);
        nodes[node2].removeLink(node1);
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].setDist(2);
        }
        return (nodes);
    }

    static int getGateway(std::vector<Node> nodes, int id) {
        std::vector<int> l = nodes[id].getLinks();
        int max = 0;
        int idTarget = -1;
        for (int i = 0; i < l.size(); i++) {
            if (nodes[l[i]].isGateway()) {
                if (nodes[l[i]].getNumberOfLinks() > max) {
                    max = nodes[l[i]].getNumberOfLinks();
                    idTarget = l[i];
                }
            }
        }
        return (idTarget);
    }

    static std::vector<Node> calcDist(std::vector<Node> nodes, int pos, int dist, std::vector<int> visited) {
        if (dist > 1) {
            nodes[pos].setDist(2);
        }
        else {
            std::vector<int> l = nodes[pos].getLinks();
            for (int i = 0; i < l.size(); i++) {
                if (!(std::count(visited.begin(), visited.end(), l[i])) && nodes[l[i]].isGateway() == false && nodes[l[i]].getGatewayContact() != 0) {
                    if (nodes[pos].getValue() == 1 && nodes[l[i]].getValue() > 0) {
                        visited.push_back(pos);
                        nodes = calcDist(nodes, l[i], dist, visited);
                    } else {
                        visited.push_back(pos);
                        nodes = calcDist(nodes, l[i], dist + 1, visited);
                    }
                }
            }
            nodes[pos].setDist(dist);
        }
        return (nodes);
    }
};

int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    int node1; // Node target 1
    int node2; // Node target 2
    std::vector<Link> links;
    std::vector<Node> nodes;
    std::vector<Node> gateways;
    std::cin >> N >> L >> E; std::cin.ignore();
    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        std::cin >> N1 >> N2; std::cin.ignore();
        links.push_back(Link(N1, N2));
    }
    for (int i = 0; i < N; i++) {
        nodes.push_back(Node(i, false));
        nodes[i].setLinks(links);
    }
    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        std::cin >> EI; std::cin.ignore();
        gateways.push_back(Node(EI, true));
        nodes[EI].setGateway(true);
    }

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        std::cin >> SI; std::cin.ignore();

        nodes = Game::setValue(nodes, links, gateways);

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        std::cerr << "Node debbuging logs start" << std::endl;
        for (int i = 0; i < nodes.size(); i++) {
            std::cerr << "Node " << i << " with id " << nodes[i].getId() << " and value " << nodes[i].getValue() << "." << std::endl;
            std::vector<int> l = nodes[i].getLinks();
            for (int j = 0; j < l.size(); j++) {
                std::cerr << "Is linked  with " << l[j] << "." << std::endl;
            }
        }
        std::cerr << "Node debbuging logs end" << std::endl;

        std::cerr << "Links debbuging logs start" << std::endl;
        for (int i = 0; i < links.size(); i++) {
            std::cerr << "Link from " << links[i].getX() << " to " << links[i].getY() << "." << std::endl;
        }
        std::cerr << "Links debbuging logs end" << std::endl;

        int urgent = nodes[SI].isUrgent(nodes);
        if (urgent != -1) {
            std::cerr << "Node " << urgent << " is urgent (agent is nearby)." << std::endl;
            node1 = SI;
            node2 = urgent;
        } else {
            std::vector<int> v;
            int id = -1;
            int links = 0;
            nodes = Game::calcDist(nodes, SI, 0, v);
            for (int i = 0; i < nodes.size(); i++) {
                std::cerr << "Node " << i << " have dist " << nodes[i].getDist() << "." << std::endl;
                if (nodes[i].getDist() < 2) {
                    if (nodes[i].getValue() > links) {
                        id = i;
                        links = nodes[i].getValue();
                    }
                }
            }
            std::cerr << "Target ID : " << id << std::endl; 
            if (id != -1) {
                node1 = id;
                node2 = Game::getGateway(nodes, id);
            } else {
                int max = 0;
                int id = -1;
                for (int i = 0; i < nodes.size(); i++) {
                    if (nodes[i].getValue() > max) {
                        id = i;
                        max = nodes[i].getValue();
                    }
                }
                node1 = id;
                node2 = Game::getGateway(nodes, id);
            }
        }

        // Example: 3 4 are the indices of the nodes you wish to sever the link between
        std::cout << node1 << " " << node2 << std::endl;
        nodes = Game::endTurn(nodes, node1, node2);
    }
}