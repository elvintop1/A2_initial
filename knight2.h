#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
class BaseBag;
class BaseItem;
class BaseKnight;
class ArmyKnights;
class BaseOpponent;
class KnightAdventure;
class Antidote;
class PhoenixDownI;
class PhoenixDownII;
class PhoenixDownIII;
class PhoenixDownIV;
enum KnightType { PALADIN = 0, LANCELOT = 1, DRAGON = 2, NORMAL = 3 };
//enum OpponentType {MadBear = 0, Bandit = 1, LordLupin = 2, Elf = 3, Troll = 4, Tornbery = 5, QueenOfCards, NinaDeRings, DurianGarden, OmegaWeapon, Hades };
const int BaseDamage[] = {10,15,45,75,95};
const double knightBaseDamage[] = {0.06,0.05,0.075,0};
enum ItemType {ANTIDOTE = 0,PHOENIX};
struct INFO {
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag * bag;
    KnightType knightType;
};

struct Node {
    Node* next = nullptr;
    ItemType type;
    BaseItem *Item = nullptr;
    Node() {
        type = PHOENIX;
        next = nullptr;
        Item = nullptr;
    }
    ~Node () {
        this->type = ANTIDOTE;
    }
};



class Events {
public:
    int number = 0;
    int *events;
    explicit Events(const string& file_events) {
        ifstream in(file_events,ios::in);
        in >> this->number;
        //cout << this->number << '\n';
        this->events = new int[number];
        for(int i = 0;i < number;++i) {
            in >> this->events[i];
        }
        in.close();
    }
    int count() const {
        return this->number;
    }
    int get(int i) const {
        return this->events[i];
    }
    ~Events() {
        delete []events;
    }
};

class BaseItem {
public:
    //int number;
    ItemType item = ANTIDOTE;
    virtual bool canUse ( BaseKnight * knight ) = 0;
    virtual void use ( BaseKnight * knight ) = 0;
    virtual string ToString() = 0;
    virtual ~BaseItem() = default;
};

class BaseBag {
public:
    int number = 0;
    Node* headPtr = nullptr;
    Node* headPtr1 = nullptr;
    BaseKnight *knight = nullptr;
    void insertitem(Node*& head, ItemType type, BaseItem* item) {
        //cout << 1 << '\n';
        Node *it = new Node;
        it->next = nullptr;
        it->type = type;
        it->Item = item;
        if (head == nullptr) {
            head = it;
        } else {
            it->next = head;
            head = it;
        }
        headPtr1 = headPtr;
        this->number++;
        //cout << this->number << '\n';
    }

    void Delete(BaseItem *item) {
        Node *curPtr = headPtr;
        Node *prePtr = nullptr;
        Node *nxtPtr;
        while(curPtr->Item != item) {
            prePtr = curPtr;
            curPtr = curPtr->next;
        }
        if(curPtr == headPtr->next) {
            headPtr->next = curPtr->next;
            delete curPtr->Item;
            delete curPtr;
            headPtr1 = headPtr;
            this->number--;
        }
        else {
            Node *needPtr = curPtr;
            nxtPtr = headPtr->next;
            headPtr->next = curPtr->next;
            if(prePtr != nullptr) prePtr->next = headPtr;
            headPtr = needPtr;
            delete headPtr->Item;
            delete headPtr;
            headPtr = nxtPtr;
            headPtr1 = headPtr;
            this->number--;
        }
    }

    void DeleteAll() {
        Node *deletePtr;
        while(headPtr != nullptr) {
            deletePtr = headPtr;
            headPtr = headPtr->next;
            delete deletePtr->Item;
            delete deletePtr;
        }
    }

    void drop() {
        Node *deletePtr;
        int n = 3;
        while (headPtr != nullptr && n > 0 && this->number > 0) {
            deletePtr = headPtr;
            headPtr = headPtr->next;
            delete deletePtr->Item;
            delete deletePtr;
            n--;
            this->number--;
        }
        headPtr1 = headPtr;
    }

    virtual bool insertFirst(BaseItem * item) = 0;
    virtual BaseItem * get(ItemType itemType) = 0;
    virtual string toString() const  = 0;
    BaseBag() = default;
    virtual ~BaseBag() = default;
};

class BaseOpponent {
public:
    int event,event_id;
    virtual bool attack(ArmyKnights *army) = 0;
    BaseOpponent() = default;
};
class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag * bag;
    KnightType knightType;
public:
    BaseKnight * next = nullptr;

    INFO get_infomation() {
        return {id,hp,maxhp,level,gil,antidote,bag,knightType};
    }

    static KnightType verified_type(int maxhp) {
        KnightType type;
        if(maxhp == 1) return NORMAL;
        bool ok_Paladin = true,ok_Lancelot = false,ok_Dragon = false;
        for(int i = 2;i < maxhp;i++) {
            if(maxhp % i == 0) {
                ok_Paladin = false;
                break;
            }
        }
        if(maxhp == 888) ok_Lancelot = true;
        if(maxhp == 345 || maxhp == 354 || maxhp == 435 || maxhp == 453 || maxhp == 543 || maxhp == 534){
            ok_Dragon = true;
        }
        if(ok_Paladin) type = PALADIN;
        else if(ok_Lancelot) type = LANCELOT;
        else if(ok_Dragon) type = DRAGON;
        else type = NORMAL;
        return type;
    }
    static BaseKnight * create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    bool update(INFO info,int meet_boss) {
        //cout << this->hp << '\n';
        this->id = info.id;
        this->maxhp = info.maxhp;
        this->hp = info.hp;
        this->level = info.level;
        this->gil = info.gil;
        this->antidote = info.antidote;
        this->bag = info.bag;
        if(meet_boss == 1) return false;
        if(meet_boss == 3) return true;
        if(this->hp < this->maxhp) {
            BaseItem *need;
            need = this->bag->get(PHOENIX);
            if(need != nullptr) this->bag->Delete(need);
            this->bag->headPtr1 = this->bag->headPtr;
            if(this->hp > 0) {
                return true;
            }
            if(this->hp <= 0) {
                if(this->gil >= 100) {
                    this->hp = this->maxhp / 2;
                    this->gil -= 100;
                    if(this->hp <= 0) {
                        return false;
                    }
                    else return true;
                }
                else return false;
            }
            else return true;
        }
        if(this->hp <= 0) {
            return false;
        }
        else return true;
    }
    BaseKnight() {};
    virtual ~BaseKnight() = default;
};


class ArmyKnights {
public:
    int number;
    int realnum;
    bool have_met_Omega = false;
    bool have_Paladin_shield = false;
    bool have_Lancelot_spear = false;
    bool have_Guinevere_hair = false;
    bool have_Excalibur = false;
    bool have_met_Hades = false;
    BaseKnight **army;

    explicit ArmyKnights(const string &file_armyknights);

    ~ArmyKnights() {
        for(int i = 1;i <= realnum;++i) {
            delete army[i];
            army[i] = nullptr;
        }
        delete []army;
        //delete army;
        army = nullptr;
    }

    bool fight(BaseOpponent *opponent);
    bool adventure (Events * events);
    int count() const {
        return this->number;
    }
    BaseKnight * lastKnight() const {
       for(int i = realnum;i >= 1;--i) {
           INFO cur = army[i]->get_infomation();
           if(cur.hp > 0) return army[i];
       }
       return nullptr;
    }
    BaseKnight * next_knight(BaseKnight *knight) const {
        while(knight->next != nullptr) {
            INFO info = knight->next->get_infomation();
            if(info.hp > 0) return knight->next;
            knight = knight->next;
        }
        return nullptr;
    }

    bool hasPaladinShield() const {
        return have_Paladin_shield;
    }
    bool hasLancelotSpear() const {
        return have_Lancelot_spear;
    }
    bool hasGuinevereHair() const {
        return have_Guinevere_hair;
    }
    bool hasExcaliburSword() const {
        return have_Excalibur;
    }

    void change_gil(int gil) const {
        BaseKnight* nxtknight = this->lastKnight();
        while (gil > 0 && nxtknight != nullptr) {
            //cout << 1 << '\n';
            INFO nxt = nxtknight->get_infomation();
            nxt.gil += gil;
            if(nxt.gil > 999) {
                gil = nxt.gil - 999;
                nxt.gil = 999;
            }
            else gil = 0;
            //cout << nxt.gil << '\n';
            nxtknight->update(nxt, 3);
            nxtknight = this->next_knight(nxtknight);
        }
    }

    void insertitem(BaseItem *item) {
        BaseKnight* nxtknight = this->lastKnight();
        while (nxtknight != nullptr) {
            INFO nxt = nxtknight->get_infomation();
            if(nxt.bag->insertFirst(item)) {
                nxtknight->update(nxt, 3);
                return;
            }
            nxtknight = this->next_knight(nxtknight);
        }
    }

    void printInfo() const;
    void printResult(bool win) const;
};

class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;

public:
    KnightAdventure() {
        armyKnights = nullptr;
        events = nullptr;
    }
    ~KnightAdventure() {
        delete events;
        delete armyKnights;
    }

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

class  Antidote:public BaseItem {
public:
    string ToString() override {
        string s = "Antidote";
        return s;
    }
    bool canUse (BaseKnight* knight) override {
        return true;
    }
    void use(BaseKnight * knight) override {
    }
};
class  PhoenixDownI:public BaseItem {
public:
    string ToString() override {
        string s = "PhoenixI";
        return s;
    }
    bool canUse ( BaseKnight * knight ) override {
        INFO cur = knight->get_infomation();
        if(cur.hp <= 0) return true;
        else return false;
    }
    void use(BaseKnight * knight) override {
        INFO cur = knight->get_infomation();
        cur.hp = cur.maxhp;
        knight->update(cur,3);
    }
};
class  PhoenixDownII:public BaseItem {
public:
    string ToString() override {
        string s = "PhoenixII";
        return s;
    }
    bool canUse ( BaseKnight * knight ) override {
        INFO cur = knight->get_infomation();
        if(cur.hp < cur.maxhp / 4) return true;
        else return false;
    }
    void use(BaseKnight * knight) override {
        INFO cur = knight->get_infomation();
        cur.hp = cur.maxhp;
        knight->update(cur,3);
    }
};
class  PhoenixDownIII:public BaseItem {
public:
    string ToString() override {
        string s = "PhoenixIII";
        return s;
    }
    bool canUse ( BaseKnight * knight ) override {
        INFO cur = knight->get_infomation();
        if(cur.hp < cur.maxhp / 3) return true;
        else return false;
    }
    void use(BaseKnight * knight) override {
        INFO cur = knight->get_infomation();
        if(cur.hp <= 0) cur.hp = cur.maxhp / 3;
        else cur.hp += cur.maxhp / 4;
        knight->update(cur,3);
    }
};
class  PhoenixDownIV:public BaseItem {
public:
    string ToString() override {
        string s = "PhoenixIV";
        return s;
    }
    bool canUse ( BaseKnight * knight ) override {
        INFO cur = knight->get_infomation();
        if(cur.hp < cur.maxhp / 2) return true;
        else return false;
    }
    void use(BaseKnight * knight) override {
        INFO cur = knight->get_infomation();
        //cout << cur.hp << '\n';
        //cur.hp = cur.maxhp;
        if(cur.hp <= 0) cur.hp = cur.maxhp / 2;
        else cur.hp += cur.maxhp / 5;
        //cout << cur.hp << '\n';
        knight->update(cur,3);
    }
};
class MADBEAR:public BaseOpponent {
public:
    MADBEAR(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == LANCELOT || curknight.knightType == PALADIN) {
            army->change_gil(100);
            curknight = knight->get_infomation();
            knight->update(curknight,3);
            return true;
        }
        else {
            //cout << this->event << ' ' << this->event_id << '\n';
            int levelO = (this->event + this->event_id) % 10 + 1;\
            //cout << levelO << '\n';
            if(curknight.level >= levelO) {
                army->change_gil(100);
                curknight = knight->get_infomation();
                knight->update(curknight,3);
                return true;
            }
            int new_hp = curknight.hp;
            new_hp = new_hp - BaseDamage[0] * (levelO - curknight.level);
            //cout << new_hp << '\n';
            curknight.hp = new_hp;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
    }
};


class BANDIT:public BaseOpponent {
public:
    BANDIT(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == LANCELOT || curknight.knightType == PALADIN) {
            army->change_gil(150);
            curknight = knight->get_infomation();
            knight->update(curknight,3);
            return true;
        }
        else {
            int levelO = (this->event + this->event_id) % 10 + 1;
            if(curknight.level >= levelO) {
                army->change_gil(150);
                curknight = knight->get_infomation();
                knight->update(curknight,3);
                return true;
            }
            int new_hp = curknight.hp;
            new_hp = new_hp - BaseDamage[1] * (levelO - curknight.level);
            //cout << new_hp << '\n';
            curknight.hp = new_hp;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
    }
};

class LORDLUPIN:public BaseOpponent {
public:
    LORDLUPIN(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        //cout << 1 << '\n';
        INFO curknight = knight->get_infomation();
        //cout << 1 << '\n';
        if(curknight.knightType == LANCELOT || curknight.knightType == PALADIN) {
            //cout << 2 << '\n';
            army->change_gil(450);
            curknight = knight->get_infomation();
            knight->update(curknight,3);
            return true;
        }
        else {
            //cout << 2 << '\n';
            int levelO = (this->event + this->event_id) % 10 + 1;
            if(curknight.level >= levelO) {
                army->change_gil(450);
                curknight = knight->get_infomation();
                knight->update(curknight,3);
                return true;
            }
            int new_hp = curknight.hp;
            new_hp = new_hp - BaseDamage[2] * (levelO - curknight.level);
            //cout << new_hp << '\n';
            curknight.hp = new_hp;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
    }
};

class ELF:public BaseOpponent {
public:
    ELF(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override{
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == LANCELOT || curknight.knightType == PALADIN) {
            army->change_gil(750);
            curknight = knight->get_infomation();
            knight->update(curknight,3);
            return true;
        }
        else {
            int levelO = (this->event + this->event_id) % 10 + 1;
            if(curknight.level >= levelO) {
                army->change_gil(750);
                curknight = knight->get_infomation();
                knight->update(curknight,3);
                return true;
            }
            int new_hp = curknight.hp;
            new_hp = new_hp - BaseDamage[3] * (levelO - curknight.level);
            //cout << BaseDamage[3] * (levelO - curknight.level) << '\n';
            curknight.hp = new_hp;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
    }
};

class TROLL:public BaseOpponent {
public:
    TROLL(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override{
        BaseKnight *knight = army->lastKnight();
        //cout << knight << '\n';
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == LANCELOT || curknight.knightType == PALADIN) {
            //cout << 1 << '\n';
            army->change_gil(800);
            curknight = knight->get_infomation();
            knight->update(curknight,3);
            return true;
        }
        else {
            int levelO = (this->event + this->event_id) % 10 + 1;
            if(curknight.level >= levelO) {
                army->change_gil(800);
                curknight = knight->get_infomation();
                knight->update(curknight,3);
                return true;
            }
            int new_hp = curknight.hp;
            new_hp = new_hp - BaseDamage[4] * (levelO - curknight.level);
            //cout << new_hp << '\n';
            curknight.hp = new_hp;
            if(!knight->update(curknight,0))  {
                //cout << 0 << '\n';
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }

    }
};
class TORNBERY:public BaseOpponent {
public:
    TORNBERY(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }

    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        int levelO = (this->event + this->event_id) % 10 + 1;
        if(curknight.level >= levelO) {
            curknight.level++;
            if(curknight.level > 10) curknight.level = 10;
            knight->update(curknight,3);
            return true;
        }
        if(curknight.knightType == DRAGON) return true;
        BaseItem *item = curknight.bag->get(ANTIDOTE);
        //cout << (item != nullptr) << '\n';
        if(item != nullptr) curknight.bag->Delete(item);
        else {
            curknight.bag->drop();
            curknight.hp -= 10;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
        if(!knight->update(curknight,3))  {
            //delete army->lastKnight();
            army->number--;
            return false;
        }
        else return true;
    }
};

class QUEEENOFCARD:public BaseOpponent {
public:
    QUEEENOFCARD(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        int num = army->number;
        BaseKnight *nxtknight = army->lastKnight();
        BaseKnight *knight = army->lastKnight();
        INFO curknight = nxtknight->get_infomation();
        int levelO = (this->event + this->event_id) % 10 + 1;
        if(curknight.level >= levelO) {
            curknight.gil *= 2;
            if (curknight.gil > 999) {
                int remainder = curknight.gil - 999;
                curknight.gil = 999;
                nxtknight->update(curknight,3);
                nxtknight = army->next_knight(nxtknight);
                while (remainder > 0) {
                    num--;
                    if(nxtknight == nullptr) break;
                    INFO nxt = nxtknight->get_infomation();
                    nxt.gil += remainder;
                    if(nxt.gil > 999) {
                        remainder = nxt.gil- 999;
                        nxt.gil = 999;
                    }
                    else remainder = 0;
                    nxtknight->update(nxt, 3);
                    nxtknight = army->next_knight(nxtknight);
                }
            }
        }
        else {
            if(curknight.knightType == PALADIN) return true;
            curknight.gil /= 2;
        }
        if(!knight->update(curknight,3))  {
            //delete army->lastKnight();
            army->number--;
            return false;
        }
        else return true;
    }
};

class NINADERING:public BaseOpponent {
public:
    NINADERING(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        //cout << 1 << '\n';
        BaseKnight *knight = army->lastKnight();
        //cout << 1 << '\n';
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == PALADIN) {
            if(curknight.hp < curknight.maxhp / 3) curknight.hp += curknight.maxhp / 5;
            knight->update(curknight,3);
            return true;
        }
        if(curknight.gil < 50) return true;
        else {
            if(curknight.hp < curknight.maxhp / 3) {
                curknight.hp += curknight.maxhp / 5;
                curknight.gil -= 50;
            }
            return knight->update(curknight,3);
        }
    }
};

class DURIANGARDEN:public BaseOpponent {
public:
    DURIANGARDEN(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        curknight.hp = curknight.maxhp;
        return knight->update(curknight,3);
    }
};

class OMEGAWEAPON:public BaseOpponent {
public:
    OMEGAWEAPON(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override{
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        if(curknight.knightType == DRAGON || (curknight.level == 10 && curknight.hp == curknight.maxhp)) {
            curknight.level = 10;
            curknight.gil = 999;
            army->have_met_Omega = true;
        }
        else {
            curknight.hp = 0;
            army->have_met_Omega = false;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
        if(!knight->update(curknight,3))  {
            //delete army->lastKnight();
            army->number--;
            return false;
        }
        else return true;
    }
};

class HADES:public BaseOpponent {
public:
    HADES(int event,int event_id) {
        this->event = event;
        this->event_id = event_id;
    }
    bool attack(ArmyKnights *army) override {
        BaseKnight *knight = army->lastKnight();
        INFO curknight = knight->get_infomation();
        if(curknight.level == 10 || (curknight.knightType == PALADIN && curknight.level >= 8)) {
            if(!army->hasPaladinShield()) army->have_Paladin_shield = true;
            army->have_met_Hades = true;
        }
        else {
            curknight.hp = 0;
            army->have_met_Hades = false;
            if(!knight->update(curknight,0))  {
                //delete army->lastKnight();
                army->number--;
                return false;
            }
            else return true;
        }
        if(!knight->update(curknight,3))  {
            //delete army->lastKnight();
            army->number--;
            return false;
        }
        else return true;
    }
};

class Bag_Paladin:public BaseBag {
public:
    bool insertFirst(BaseItem* item) override {
        if (item->item == ANTIDOTE) {
            insertitem(headPtr, ANTIDOTE, item);
        }
        if (item->item == PHOENIX) {
            insertitem(headPtr, PHOENIX, item);
        }
        return true;
    }
    BaseItem* get(ItemType itemType) override {
        while (headPtr1 != nullptr) {
            //cout << this->number << '\n';
            if (headPtr1->type == itemType && headPtr1->Item->canUse(knight))  {
                headPtr1->Item->use(knight);
                BaseItem *newitem = headPtr1->Item;
                headPtr1 = headPtr1 -> next;
                return newitem;
            }
            headPtr1 = headPtr1->next;
        }
        return nullptr;
    }
    string toString() const  override {
        string s;
        s += "Bag[count=" + to_string(number) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            string name = current->Item->ToString();
            s += name;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    Bag_Paladin(BaseKnight* knight, int phoenixdownI, int antidote) {
        this->knight = knight;
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            item->item = PHOENIX;
            insertFirst(item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            item->item = ANTIDOTE;
            insertFirst(item);
        }
        //this->toString();
    }
    ~Bag_Paladin() override {
        DeleteAll();
    }
};
class Bag_Lancelot:public BaseBag {
public:
    bool insertFirst(BaseItem* item) override {
        if(number == 16) return false;
        if (item->item == ANTIDOTE) {
            insertitem(headPtr, ANTIDOTE, item);
        }
        if (item->item == PHOENIX) {
            insertitem(headPtr, PHOENIX, item);
        }
        return true;
    }
    BaseItem* get(ItemType itemType) override {
        while (headPtr1 != nullptr) {
            //cout << this->number << '\n';
            if (headPtr1->type == itemType && headPtr1->Item->canUse(knight))  {
                headPtr1->Item->use(knight);
                BaseItem *newitem = headPtr1->Item;
                headPtr1 = headPtr1 -> next;
                return newitem;
            }
            headPtr1 = headPtr1->next;
        }
        return nullptr;
    }
    string toString() const override {
        string s;
        s += "Bag[count=" + to_string(number) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            string name = current->Item->ToString();
            s += name;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    Bag_Lancelot(BaseKnight* knight, int phoenixdownI, int antidote) {
        this->knight = knight;
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            item->item = PHOENIX;
            insertFirst(item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            item->item = ANTIDOTE;
            insertFirst(item);
        }
    }
    ~Bag_Lancelot() override {
        DeleteAll();
    }
};
class Bag_DragonKnight:public BaseBag {
public:
    bool insertFirst(BaseItem* item) override {
        if(number == 14) return false;
        if (item->item == ANTIDOTE) {
            insertitem(headPtr, ANTIDOTE, item);
        }
        if (item->item == PHOENIX) {
            insertitem(headPtr, PHOENIX, item);
        }
        return true;
    }
    BaseItem* get(ItemType itemType) override {
        while (headPtr1 != nullptr) {
            if (headPtr1->type == itemType && headPtr1->Item->canUse(knight))  {
                headPtr1->Item->use(knight);
                BaseItem *newitem = headPtr1->Item;
                headPtr1 = headPtr1 -> next;
                return newitem;
            }
            headPtr1 = headPtr1->next;
        }
        return nullptr;
    }
    string toString() const override{
        string s;
        s += "Bag[count=" + to_string(number) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            string name = current->Item->ToString();
            s += name;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    Bag_DragonKnight(BaseKnight* knight, int phoenixdownI, int antidote) {
        this->knight = knight;
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            item->item = PHOENIX;
            insertFirst(item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            item->item = ANTIDOTE;
            insertFirst(item);
        }
        //this->toString();
    }
    ~Bag_DragonKnight() override {
        DeleteAll();
    }
};
class Bag_NormalKnight:public BaseBag {
public:
    bool insertFirst(BaseItem* item) override {
        if(number == 19) return false;
        if (item->item == ANTIDOTE) {
            insertitem(headPtr, ANTIDOTE, item);
        }
        if (item->item == PHOENIX) {
            insertitem(headPtr, PHOENIX, item);
        }
        return true;
    }
    BaseItem* get(ItemType itemType) override {
        while (headPtr1 != nullptr) {
            //cout << this->number << '\n';
            if (headPtr1->type == itemType && headPtr1->Item->canUse(knight))  {
                headPtr1->Item->use(knight);
                BaseItem *newitem = headPtr1->Item;
                headPtr1 = headPtr1 -> next;
                return newitem;
            }
            headPtr1 = headPtr1->next;
        }
        return nullptr;
    }
    string toString() const override {
        string s;
        s += "Bag[count=" + to_string(number) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            string name = current->Item->ToString();
            s += name;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    Bag_NormalKnight(BaseKnight* knight, int phoenixdownI, int antidote) {
        this->knight = knight;
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            item->item = PHOENIX;
            insertFirst(item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            item->item = ANTIDOTE;
            insertFirst(item);
        }
        //cout << this->toString() << '\n';
    }
    ~Bag_NormalKnight() override {
        DeleteAll();
    }
};

class PaladinKnight:public BaseKnight {
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI): BaseKnight() {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        knightType = PALADIN;
        this->bag = new Bag_Paladin(this,phoenixdownI,antidote);
    }
    ~PaladinKnight() override {
        delete bag;
    }
};
class Lancelot:public BaseKnight {
public:
    Lancelot(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) : BaseKnight() {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        knightType = LANCELOT;
        this->bag = new Bag_Lancelot(this,phoenixdownI,antidote);
    }
    ~Lancelot() override {
        delete bag;
    }
};
class DragonKnight:public BaseKnight {
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) : BaseKnight() {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        knightType = DRAGON;
        this->bag = new Bag_DragonKnight(this,phoenixdownI,antidote);
    }
    ~DragonKnight() override {
        delete bag;
    }
};
class Normal:public BaseKnight {
public:
    Normal(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        this->knightType = NORMAL;
        this->bag = new Bag_NormalKnight(this,phoenixdownI,antidote);

    }
    ~Normal() override {
        delete bag;
    }
};





#endif // __KNIGHT2_H__
#pragma clang diagnostic pop