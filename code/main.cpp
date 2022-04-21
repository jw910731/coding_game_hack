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
        }
        if (maker.foodDelivery())
            continue;
        else
            maker.action("WAIT");
    }
    return 0;
}
