#include "chef.h"
int main()
{
    chef maker;
    maker.gameInit();
    while (true) {
        maker.roundInit();
        if (maker.chefState == chef::state::ALLDONE) {
            maker.recipeStratege("MaxAward");
            maker.checkLackList();
            for (auto &it : maker.lackList) {
                if (it == "CHOPPED_STRAWBERRIES")
                    maker.chopList.emplace_back("STRAWBERRIES");
                if (it == "CROISSANT") {
                    maker.ovenList.emplace_back("DOUGH");
                }
                if (it == "TART") {
                    maker.ovenList.emplace_back("RAW_TART");
                }
            }
        }
        if (maker.useOvenFlag) {
            maker.take("OVEN");
        } else if (maker.foodDelivery()) {
            continue;
        } else if (!maker.mergeList.empty()) {
            maker.merge();
        } else if (!maker.chopList.empty()) {
            maker.chop();
        } else if (!maker.ovenList.empty()) {
            maker.oven();
        } else {
            maker.action("WAIT");
        }
    }
    return 0;
}
