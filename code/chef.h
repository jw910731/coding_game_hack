#ifndef CHEF_H
#define CHEF_H
#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class tool
{
public:
    static const int d[8][2];
    static std::vector<std::string> splitStr(
        const std::string &,
        std::string);
};

class pos
{
public:
    int x, y;
    pos() : x(-1), y(-1) {}
    pos(int ix, int iy)
    {
        if (ix < 0 || ix > 10 || iy < 0 || iy > 6)
            x = y = -1;
        else
            x = ix, y = iy;
    }
    pos operator+(const pos &p2) const
    {
        return pos(x + p2.x, y + p2.y);
    }
    bool operator==(const pos &p2) const
    {
        return x == p2.x && y == p2.y;
    }
    bool operator!=(const pos &p2) const
    {
        return !(*this == p2);
    }
};

class kitchen
{
private:
    pos oven, blueberry, icecream, strawberry, windows, dish,
        choppingBoard, dough;
    std::vector<std::pair<pos, std::string>> table;
    std::vector<std::string> maze;
    std::string ovenContent;
    int ovenCounter;

public:
    static constexpr int mazeXNum = 11, mazeYNum = 7;
    void addMaze(const std::string &);
    void addTable(pos, const std::string &);
    void clearTable();
    void initKitchen();
    pos getItemPos(const std::string &) const;
    void setOven(const std::string &, int);
    std::string getOvenitem() const;
    std::vector<std::pair<pos, std::string>> getTableItem() const;
    bool isEmptyTable(const pos &) const;
};

class customer
{
private:
    std::vector<std::pair<std::string, int>> order;

public:
    void addOrder(const std::string &, int);
    std::vector<std::pair<std::string, int>> getOrder() const;
    void clearOrder();
};

class player
{
private:
    pos selfPos;
    std::string hand;

public:
    void setPos(pos);
    void setHand(const std::string &);
    pos getPos() const;
    std::string getHand() const;
};

// 提供給 User 的
class chef
{
private:
    kitchen cookhouse;
    customer client;
    player me, partner;
    int turnsRemaining;

public:
    bool useOvenFlag;
    std::string recipe;
    std::vector<std::string> chopList, ovenList, mergeList,
        lackList;
    enum state { ALLDONE, COOKING } chefState = ALLDONE;
    chef() : useOvenFlag(false), chefState(ALLDONE){};
    void gameInit();
    void roundInit();
    void clearAll();
    bool action(const std::string &, const pos &) const;
    void action(const std::string &) const;
    void recipeStratege(const std::string &);
    void checkLackList();
    bool take(const pos &);
    bool take(const std::string &);
    void put() const;
    void chop();
    void oven();
    void merge();
    bool foodDelivery();
};

const int tool::d[8][2] = {{0, 1}, {0, -1}, {1, 0},  {-1, 0},
                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};


std::vector<std::string> tool::splitStr(
    const std::string &str,
    std::string del)
{
    std::vector<std::string> res;
    int start = 0, end = str.find(del);
    while (end != -1) {
        res.emplace_back(str.substr(start, end - start));
        start = end + del.size();
        end = str.find(del, start);
    }
    res.emplace_back(str.substr(start, end - start));
    return res;
}

std::ostream &operator<<(std::ostream &os, const pos &input)
{
    os << input.x << " " << input.y;
    return os;
}

void kitchen::addMaze(const std::string &line)
{
    maze.emplace_back(line);
}

std::string kitchen::getOvenitem() const
{
    return ovenContent;
}

void kitchen::addTable(pos p, const std::string &name)
{
    table.emplace_back(p, name);
}
void kitchen::clearTable()
{
    table.clear();
}
void kitchen::initKitchen()
{
    for (int i = 0; i < mazeYNum; ++i) {
        for (int j = 0; j < mazeXNum; ++j) {
            if (maze[i][j] == 'B')
                blueberry = {j, i};
            else if (maze[i][j] == 'I')
                icecream = {j, i};
            else if (maze[i][j] == 'S')
                strawberry = {j, i};
            else if (maze[i][j] == 'D')
                dish = {j, i};
            else if (maze[i][j] == 'W')
                windows = {j, i};
            else if (maze[i][j] == 'C')
                choppingBoard = {j, i};
            else if (maze[i][j] == 'H')
                dough = {j, i};
            else if (maze[i][j] == 'O')
                oven = {j, i};
        }
    }
}

pos kitchen::getItemPos(const std::string &name) const
{
    if (name == "BLUEBERRIES")
        return blueberry;
    else if (name == "ICE_CREAM")
        return icecream;
    else if (name == "STRAWBERRIES")
        return strawberry;
    else if (name == "DISH")
        return dish;
    else if (name == "WINDOW")
        return windows;
    else if (name == "CHOPPED")
        return choppingBoard;
    else if (name == "DOUGH")
        return dough;
    else if (name == "OVEN")
        return oven;
    else {
        auto it = std::find_if(
            table.begin(), table.end(),
            [&](const std::pair<pos, std::string> &element) {
                return element.second == name;
            });
        return it == table.end() ? pos(-1, -1) : it->first;
    }
}

void kitchen::setOven(const std::string &content, int counter)
{
    ovenContent = content;
    ovenCounter = counter;
}


bool kitchen::isEmptyTable(const pos &inPos) const
{
    if (inPos.x < 0 || inPos.x > 10 || inPos.y < 0 || inPos.y > 6)
        return false;
    if (maze[inPos.y][inPos.x] != '#')
        return false;
    for (auto &it : table) {
        if (inPos == it.first)
            return false;
    }
    return true;
}

std::vector<std::pair<pos, std::string>> kitchen::getTableItem()
    const
{
    return table;
}

void customer::addOrder(const std::string &item, int award)
{
    order.emplace_back(item, award);
    sort(
        order.begin(), order.end(),
        [](const std::pair<std::string, int> &a,
           const std::pair<std::string, int> &b) {
            if (a.second != b.second)
                return a.second > b.second;
            else
                return a.first < a.first;
        });
}
void customer::clearOrder()
{
    order.clear();
}

std::vector<std::pair<std::string, int>> customer::getOrder()
    const
{
    return order;
}

void player::setPos(pos input)
{
    selfPos = input;
}

void player::setHand(const std::string &Hand)
{
    hand = Hand;
}

pos player::getPos() const
{
    return selfPos;
}

std::string player::getHand() const
{
    return hand;
}

void chef::gameInit()
{
    int numAllCustomers;
    std::cin >> numAllCustomers;
    client.clearOrder();
    for (int i = 0; i < numAllCustomers; ++i) {
        std::string customerItem;
        int customerAward;
        std::cin >> customerItem >> customerAward;
        client.addOrder(customerItem, customerAward);
    }
    std::cin.ignore();
    for (int i = 0; i < cookhouse.mazeYNum; ++i) {
        std::string kitchenLine;
        getline(std::cin, kitchenLine);
        cookhouse.addMaze(kitchenLine);
    }
    cookhouse.initKitchen();
}

void chef::roundInit()
{
    std::cin >> turnsRemaining;
    if (turnsRemaining <= 2)
        clearAll();
    auto playerInit = [](player &inPlayer) {
        int x, y;
        std::string item;
        std::cin >> x >> y >> item;
        inPlayer.setPos({x, y});
        inPlayer.setHand(item);
    };
    playerInit(me);
    playerInit(partner);
    cookhouse.clearTable();
    int numTablesWithItems;
    std::cin >> numTablesWithItems;
    for (int i = 0; i < numTablesWithItems; ++i) {
        int tableX, tableY;
        std::string item;
        std::cin >> tableX >> tableY >> item;
        cookhouse.addTable({tableX, tableY}, item);
    }
    std::string ovenContents;
    int ovenTimer;
    std::cin >> ovenContents >> ovenTimer;
    cookhouse.setOven(ovenContents, ovenTimer);
    bool recipeFlag = false;
    client.clearOrder();
    int numCustomers;
    std::cin >> numCustomers;
    for (int i = 0; i < numCustomers; ++i) {
        std::string customerItem;
        int customerAward;
        std::cin >> customerItem >> customerAward;
        if (recipe == customerItem)
            recipeFlag = true;
        client.addOrder(customerItem, customerAward);
    }
    if (!recipeFlag) {
        recipe.clear();
        clearAll();
        chefState = ALLDONE;
    }
    if (!recipe.empty())
        checkLackList();
}

void chef::clearAll()
{
    chopList.clear();
    ovenList.clear();
    mergeList.clear();
    lackList.clear();
}

bool chef::action(const std::string &mode, const pos &destination)
    const
{
    std::cout << mode << " " << destination << std::endl;
    if (mode == "USE") {
        if (abs(me.getPos().x - destination.x) <= 1 &&
            abs(me.getPos().y - destination.y) <= 1)
            return true;
    }
    return false;
}

void chef::action(const std::string &mode) const
{
    std::cout << mode << std::endl;
}

void chef::recipeStratege(const std::string &type)
{
    int retPos = 0;
    if (type == "Random") {
        srand(time(nullptr));
        retPos = rand() % 3;
    } else if (type == "MaxAward")
        retPos = 0;
    else if (type == "SecondAward")
        retPos = 1;
    else if (type == "MinAward")
        retPos = 2;
    recipe = client.getOrder()[retPos].first;
    chefState = chef::state::COOKING;
}

void chef::checkLackList()
{
    auto recipeList = tool::splitStr(recipe, "-");
    auto meHandList = tool::splitStr(me.getHand(), "-");
    lackList.clear();
    for (auto &it : recipeList) {
        if (cookhouse.getItemPos(it) == pos(-1, -1)) {
            lackList.emplace_back(it);
        }
    }
    bool flagHandCanUse = true;
    for (auto &it : meHandList) {
        if (std::find(recipeList.begin(), recipeList.end(), it) ==
            recipeList.end()) {
            flagHandCanUse = false;
            break;
        }
    }
    if (flagHandCanUse) {
        for (auto &it : meHandList) {
            if (auto findPos = std::find(
                    lackList.begin(), lackList.end(), it);
                findPos != lackList.end())
                lackList.erase(findPos);
        }
    }
    bool flagTableCanUse = true;
    auto tableItem = cookhouse.getTableItem();
    std::vector<std::string> targetTableItemList;
    for (auto &i : tableItem) {
        flagTableCanUse = true;
        if (i.second.find("DISH") != std::string::npos) {
            targetTableItemList = tool::splitStr(i.second, "-");
            for (auto &j : targetTableItemList) {
                if (std::find(
                        recipeList.begin(), recipeList.end(),
                        j) == recipeList.end()) {
                    flagTableCanUse = false;
                    break;
                }
            }
        }
        if (flagTableCanUse)
            break;
    }
    if (flagTableCanUse) {
        for (auto &it : targetTableItemList) {
            if (auto findPos = std::find(
                    lackList.begin(), lackList.end(), it);
                findPos != lackList.end())
                lackList.erase(findPos);
        }
    }
}

bool chef::foodDelivery()
{
    if (!lackList.empty()) {
        auto tableInfo = cookhouse.getTableItem();
        for (auto &it : lackList) {
            if (it == "CHOPPED_STRAWBERRIES" &&
                std::find(
                    chopList.begin(), chopList.end(),
                    "STRAWBERRIES") == chopList.end())
                chopList.emplace_back("STRAWBERRIES");
            else if (
                it == "CROISSANT" &&
                std::find(
                    ovenList.begin(), ovenList.end(), "DOUGH") ==
                    ovenList.end())
                ovenList.emplace_back("DOUGH");
            else if (it == "TART") {
                if (cookhouse.getItemPos("RAW_TART") ==
                        pos(-1, -1) &&
                    me.getHand() != "RAW_TART") {
                    if (cookhouse.getItemPos("CHOPPED_DOUGH") ==
                            pos(-1, -1) &&
                        me.getHand() != "CHOPPED_DOUGH") {
                        if (std::find(
                                chopList.begin(), chopList.end(),
                                "DOUGH") == chopList.end())
                            chopList.emplace_back("DOUGH");
                    }
                }
                if (std::find(
                        ovenList.begin(), ovenList.end(),
                        "RAW_TART") == ovenList.end())
                    ovenList.emplace_back("RAW_TART");
            }
        }
        return false;
    }
    auto meHand = me.getHand();
    if (meHand == "NONE") {
        for (auto &it : cookhouse.getTableItem()) {
            if (it.second.find("DISH") != std::string::npos) {
                auto itemList = tool::splitStr(it.second, "-");
                bool canUseFlag = true;
                for (auto &i : itemList) {
                    if (recipe.find(i) == std::string::npos) {
                        canUseFlag = false;
                        break;
                    }
                }
                if (canUseFlag) {
                    action("USE", it.first);
                    return true;
                }
            }
        }
    }
    if (meHand != "NONE" &&
        meHand.find("DISH") == std::string::npos) {
        put();
        return true;
    }
    auto recipeList = tool::splitStr(recipe, "-");
    recipeList.emplace_back("WINDOW");
    auto meHandList = tool::splitStr(me.getHand(), "-");
    for (auto &it : meHandList) {
        if (auto findPos = std::find(
                recipeList.begin(), recipeList.end(), it);
            findPos != recipeList.end())
            recipeList.erase(findPos);
    }
    if (!recipeList.empty()) {
        pos dishPosXY = cookhouse.getItemPos(recipeList[0]);
        if (action("USE", dishPosXY) &&
            recipeList[0] == "WINDOW") {
            clearAll();
            chefState = ALLDONE;
        }
        return true;
    }
    return false;
}

bool chef::take(const pos &destination)
{
    if (destination == cookhouse.getItemPos("OVEN")) {
        action("USE", destination);
        if (me.getHand() == "CROISSANT" || me.getHand() == "TART")
            useOvenFlag = false;
        return true;
    }
    return action("USE", destination);
}

bool chef::take(const std::string &itemName)
{
    return take(cookhouse.getItemPos(itemName));
}

void chef::put() const
{
    for (int i = 0; i < 8; ++i) {
        pos nPos = {
            me.getPos().x + tool::d[i][0],
            me.getPos().y + tool::d[i][1]};
        if (cookhouse.isEmptyTable(nPos)) {
            action("USE", nPos);
            return;
        }
    }
    int nx = me.getPos().x, ny = me.getPos().y;
    nx += (nx >= 5) ? -2 : +2;
    ny += (ny >= 5) ? -2 : +2;
    if (action("MOVE", {nx, ny}))
        return;
}

void chef::chop()
{
    std::string meItem = me.getHand();
    if (meItem != "NONE" && meItem != chopList[0]) {
        put();
        return;
    }
    std::string tmpItem =
        (meItem == chopList[0]) ? "CHOPPED" : chopList[0];
    pos dishPosXY = cookhouse.getItemPos(tmpItem);
    bool doneFlag = action("USE", dishPosXY);

    if (tmpItem == "CHOPPED" && doneFlag) {
        chopList.erase(chopList.begin());
    }
}

void chef::oven()
{
    std::string meItem = me.getHand();
    if (meItem != "NONE" && meItem != ovenList[0]) {
        put();
        return;
    }
    std::string tmpItem =
        (meItem == ovenList[0]) ? "OVEN" : ovenList[0];
    pos dishPosXY = cookhouse.getItemPos(tmpItem);
    if (dishPosXY == pos(-1, -1)) {
        mergeList.push_back(tmpItem);
        action("WAIT");
        return;
    } else {
        bool doneFlag = action("USE", dishPosXY);
        if (tmpItem == "OVEN" && doneFlag) {
            useOvenFlag = true;
            ovenList.erase(ovenList.begin());
        }
        return;
    }
}

void chef::merge()
{
    std::string meItem = me.getHand();
    std::string tmpItem;
    if (mergeList[0] == "RAW_TART") {
        if (meItem == "RAW_TART") {
            mergeList.erase(mergeList.begin());
            put();
            return;
        }
        if (meItem != "NONE") {
            if (meItem != "CHOPPED_DOUGH" &&
                meItem != "BLUEBERRIES") {
                put();
                return;
            }
            tmpItem = (meItem == "CHOPPED_DOUGH")
                          ? "BLUEBERRIES"
                          : "CHOPPED_DOUGH";
        } else
            tmpItem = "CHOPPED_DOUGH";
        pos dishPosXY = cookhouse.getItemPos(tmpItem);
        if (dishPosXY == pos(-1, -1)) {
            chopList.push_back("DOUGH");
            action("WAIT");
            return;
        }
        action("USE", dishPosXY);
    }
}

#endif
