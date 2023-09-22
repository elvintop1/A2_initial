#include "knight2.h"

/* * * BEGIN implementation of class BaseBag * * */

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */

string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    //cout << this->id << '\n';
    s += "[Knight:id:" + to_string(this->id)
        + ",hp:" + to_string(this->hp)
        + ",maxhp:" + to_string(this->maxhp)
        + ",level:" + to_string(this->level)
        + ",gil:" + to_string(this->gil) + ','
        + bag->toString()
        + ",knight_type:" + typeString[this->knightType]
        + "]";
    return s;
}

BaseKnight *(BaseKnight::create)(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI){
    //cout << maxhp << '\n';
    KnightType type = verified_type(maxhp);
    BaseKnight * knight;
    //cout << 0 << '\n';
    switch(type) {
        case PALADIN: {
            knight = new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
            break;
        }
        case LANCELOT: {
            knight = new Lancelot(id, maxhp, level, gil, antidote, phoenixdownI);
            break;
        }
        case DRAGON: {
            knight = new DragonKnight(id, maxhp, level, gil, 0, phoenixdownI);
            break;
        }
        case NORMAL: {
            knight = new Normal(id, maxhp, level, gil, antidote, phoenixdownI);
            break;
        }

    }
    //cout << 0 << '\n';
    //cout << knight << '\n';
    return knight;

}

/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const {
    //cout << 1 << '\n';
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = this->lastKnight(); // last knight
        //cout << lknight << '\n';
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}


ArmyKnights::ArmyKnights(const string & file_armyknights) {
    ifstream in(file_armyknights,ios::in);
    in >> this->realnum;
    this->number = this->realnum;
    //cout << this->number << '\n';
    army = new BaseKnight*[number + 1];
    for(int id = 1;id <= number;id++) {
        int maxhp,level,gil,antidote,phoenixdownI;
        in >> maxhp >> level >> phoenixdownI >> gil >> antidote;
        army[id] = BaseKnight::create(id, maxhp, level, gil, antidote, phoenixdownI);
        if(id > 1) army[id]->next = army[id - 1];
        //army[id]->print_bag();
    }
    in.close();

}

bool ArmyKnights::fight(BaseOpponent *opponent) {
    //cout << 1 << '\n';
    opponent->attack(this);
    //cout << 1 << '\n';
    if(this->number > 0) return true;
    else return false;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}

bool ArmyKnights::adventure (Events * events) {
    for (int i = 0; i < events->count(); ++i) {
        BaseOpponent *opponent = nullptr;
        //cout << i << " " << events->get(i) << '\n';
        if (events->get(i) <= 11) {
            if (events->get(i) == 1) opponent = new MADBEAR(events->get(i), i);
            if (events->get(i) == 2) opponent = new BANDIT(events->get(i), i);
            if (events->get(i) == 3) opponent = new LORDLUPIN(events->get(i), i);
            if (events->get(i) == 4) opponent = new ELF(events->get(i), i);
            if (events->get(i) == 5) opponent = new TROLL(events->get(i), i);
            if (events->get(i) == 6) opponent = new TORNBERY(events->get(i), i);
            if (events->get(i) == 7) opponent = new QUEEENOFCARD(events->get(i), i);
            if (events->get(i) == 8) opponent = new NINADERING(events->get(i), i);
            if (events->get(i) == 9) opponent = new DURIANGARDEN(events->get(i), i);
            if (events->get(i) == 10 && !this->have_met_Omega) opponent = new OMEGAWEAPON(events->get(i), i);
            if (events->get(i) == 11 && !this->have_met_Hades) opponent = new HADES(events->get(i), i);
            //cout << 5 << '\n';
            if(opponent != nullptr) this->fight(opponent);
            delete opponent;
        } else {
            if (events->get(i) == 112) {
                PhoenixDownII *newitem = new PhoenixDownII;
                newitem->item = PHOENIX;
                this->insertitem(newitem);
            }
            if (events->get(i) == 113) {
                PhoenixDownIII *newitem = new PhoenixDownIII;
                newitem->item = PHOENIX;
                this->insertitem(newitem);
            }
            if (events->get(i) == 114) {
                PhoenixDownIV *newitem = new PhoenixDownIV;
                newitem->item = PHOENIX;
                this->insertitem(newitem);
            }
            if (events->get(i) == 95) {
                this->have_Paladin_shield = true;
            }
            if (events->get(i) == 96) {
                this->have_Lancelot_spear = true;
            }
            if (events->get(i) == 97) {
                this->have_Guinevere_hair = true;
            }
            if (events->get(i) == 98) {
                if (this->have_Lancelot_spear && this->have_Paladin_shield && this->have_Guinevere_hair)
                    this->have_Excalibur = true;
            }
            if (events->get(i) == 99) {
                BaseKnight *knight = this->lastKnight();
                INFO curknight;
                if(knight != nullptr) curknight = knight->get_infomation();
                if (!this->hasExcaliburSword()) {
                    if (this->hasPaladinShield() && this->hasGuinevereHair() && this->hasLancelotSpear()) {
                        int Bosshp = 5000;
                        while (Bosshp > 0 && this->number > 0 && knight != nullptr) {
                            while (knight != nullptr && curknight.knightType == NORMAL) {
                                knight = this->next_knight(knight);
                                if(knight != nullptr) curknight = knight->get_infomation();
                            }
                            if(knight == nullptr) break;
                            int damage = curknight.hp * curknight.level * knightBaseDamage[curknight.knightType];
                            //cout << curknight.hp << ' ';
                            //cout << damage << ' ';
                            Bosshp -= damage;
                            //cout << Bosshp << " " << curknight.hp << " " << curknight.level << '\n';
                            if (Bosshp > 0) {
                                curknight.hp = 0;
                                knight->update(curknight, 1);
                                this->number--;
                                //cout << this->number << '\n';
                                knight = this->next_knight(knight);
                                if(knight != nullptr) curknight = knight->get_infomation();
                            }
                            else break;
                        }
                        if (Bosshp > 0) {
                            while (this->number > 0) {
                                knight = this->lastKnight();
                                curknight = knight->get_infomation();
                                curknight.hp = 0;
                                knight->update(curknight, 1);
                                this->number--;
                                //cout << this->number << '\n';
                            }
                        }
                    } else {
                        while (this->number > 0) {
                            knight = this->lastKnight();
                            curknight = knight->get_infomation();
                            curknight.hp = 0;
                            knight->update(curknight, 1);
                            this->number--;
                            //cout << this->number << '\n';
                        }
                    }
                }
            }
        }
        this->printInfo();
        if (this->number <= 0) {
            //this->printResult(this->number > 0);
            return false;
        }
    }
    if (this->number > 0) return true;
    return false;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */

void KnightAdventure::loadArmyKnights(const string &file_armyknights) {
    armyKnights = new ArmyKnights(file_armyknights);
}

void KnightAdventure::loadEvents(const string &file_events) {
    events = new Events(file_events);
}

void KnightAdventure::run() {
    //cout << events->count() << '\n';

    armyKnights->printResult(armyKnights->adventure(this->events));
}

/* * * END implementation of class KnightAdventure * * */

