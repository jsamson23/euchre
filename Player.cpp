#include <cassert>
#include <iostream>
#include <array>
#include <algorithm>
#include "Player.hpp"

using namespace std;

class SimplePlayer : public Player {
public:

    SimplePlayer(string name) : playerName(name) {}

    const std::string& get_name() const {
        return playerName;
    }
    void add_card(const Card& c) {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }
    bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const {
        int countGoodCards = 0;
        Suit upsuit = upcard.get_suit();
        assert(round == 1 || round == 2);
        if (round == 1) {
            for (int i = 0; i < hand.size(); i++) {
                if (hand[i].is_face_or_ace() && hand[i].is_trump(upsuit)) {
                    countGoodCards++;
                }
            }
            if (countGoodCards >= 2) {
                order_up_suit = upsuit;
                return true;
            }
        }
        else if (round == 2) {
            if (is_dealer) {
                order_up_suit = Suit_next(upsuit);
                return true;
            }
            for (int i = 0; i < hand.size(); i++) {
                if (hand[i].is_face_or_ace() && hand[i].is_trump(Suit_next(upsuit))) {
                    countGoodCards++;
                }
            }
            if (countGoodCards >= 1) {
                order_up_suit = Suit_next(upsuit);
                return true;
            }
        }
        return false;
    }
    void add_and_discard(const Card & upcard) {
        assert(hand.size() > 0);
        hand.push_back(upcard);
        Suit trump = upcard.get_suit();
        int lowestIndex = 0;
        for (int i = 0; i < hand.size(); i++) {
            if (Card_less(hand[i], hand[lowestIndex], upcard, trump)) {
                lowestIndex = i;
            }
        }
        hand.erase(hand.begin() + lowestIndex);
    }

    Card lead_card(Suit trump) {
        assert(hand.size() > 0);
        int HNTIndex = 0;
        for (int i = 0; i < hand.size(); i++) {
            if (hand[HNTIndex].is_trump(trump) && hand[i] > hand[HNTIndex]) HNTIndex = i;
            else {
                if (!hand[i].is_trump(trump) && hand[i] > hand[HNTIndex]) HNTIndex = i;
            }
        }
        return hand[HNTIndex];
    }

    Card play_card(const Card& led_card, Suit trump)
    {
        assert(hand.size() > 0);
        int highestCardIndex = 0;
        int minCardIndex = 0;
        Suit led_suit = led_card.get_suit();
        for (int i = 0; i < hand.size(); i++) { //iterate through the index of the hand of cards
            if (hand[i].get_suit(trump) == led_suit) { // if current card has the led suit
                if (hand[highestCardIndex].get_suit(trump) != led_suit) highestCardIndex = i; // if the highest value card is not of led suit, then current card is new highest value card
                else { // if the highest value card is also of led suit
                    if (Card_less(hand[highestCardIndex], hand[i], led_card, trump)) highestCardIndex = i; // compare the cards, if current card is larger, it becomes new highest value card
                }
            }
            else { // if current card is not the led suit
                if (hand[minCardIndex].get_suit(trump) == led_suit) minCardIndex = i; // if the minimum value card is the led suit, then current card is the new lowest value card (we can technically remove this since if there exists a card of led suit, it will never return the minimum value card)
                else { // if they are both NOT the led suit
                    if (Card_less(hand[i], hand[minCardIndex], led_card, trump)) minCardIndex = i; // if current card is smaller than the minimum card, it becomes new lowest card
                }
            }
        }
        if (hand[highestCardIndex].get_suit(trump) == led_card.get_suit()) { // if highest card index IS of led suit, return that card
            Card returnable(hand[highestCardIndex].get_rank(), hand[highestCardIndex].get_suit());
            hand.erase(hand.begin() + highestCardIndex);
            return returnable;
        } // since the code reached this point, this means that the highest value card is not of led suit, THERE DOESN'T EXIST A CARD WITH LED SUIT IN THEIR HAND, thus return the minimum value card.
        Card returnable(hand[minCardIndex].get_rank(), hand[minCardIndex].get_suit());
        hand.erase(hand.begin() + minCardIndex);
        return returnable;

    }
        
    
private:
    vector<Card> hand;
    string playerName;
};

class HumanPlayer : public Player {
private:
    vector<Card> hand;
    string playerName;
public:

    HumanPlayer(string name) : playerName(name) {}

    void print_hand() const {
        for (size_t i = 0; i < hand.size(); ++i)
            cout << "Human player " << playerName << "'s hand: "
            << "[" << i << "] " << hand[i] << "\n";
    }

    const std::string& get_name() const {
        return playerName;
    }

    void add_card(const Card& c) {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const {
        print_hand();
        cout << "Human player " << playerName << ", please enter a suit, or \"pass\":\n";
        string decision;
        cin >> decision;

        if (decision != "pass") {
            order_up_suit = string_to_suit(decision);
            return true;
        }
        else {
            return false;
        }

    }

    //REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card& upcard) {
        std::sort(hand.begin(), hand.end());
        int indexRemove;
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << playerName << ", please select a card to discard:\n";
        cin >> indexRemove;
        if (indexRemove == -1)
            return;
        else {
            hand.erase(hand.begin() + indexRemove);
            hand.push_back(upcard);
        }
    }

    //REQUIRES Player has at least one card
//EFFECTS  Leads one Card from Player's hand according to their strategy
//  "Lead" means to play the first Card in a trick.  The card
//  is removed the player's hand.
    Card lead_card(Suit trump) {
        std::sort(hand.begin(), hand.end());
        int indexPlay;
        print_hand();
        cout << "Human player " << playerName << ", please select a card:\n";
        cin >> indexPlay;
        Card returnable(hand[indexPlay].get_rank(), hand[indexPlay].get_suit());
        hand.erase(hand.begin() + indexPlay);
        return returnable;
    }

    Card play_card(const Card& led_card, Suit trump) {
        std::sort(hand.begin(), hand.end());
        int indexPlay;
        print_hand();
        cout << "Human player " << playerName << ", please select a card:\n";
        cin >> indexPlay;
        Card returnable(hand[indexPlay].get_rank(), hand[indexPlay].get_suit());
        hand.erase(hand.begin() + indexPlay);
        return returnable;
    }


};


Player* Player_factory(const std::string& name,
    const std::string& strategy) {
    // We need to check the value of strategy and return 
    // the corresponding player type.
    if (strategy == "Simple") {
        // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }
    else if (strategy == "Human") {
        return new HumanPlayer(name);
    }
    // Repeat for each other type of Player
        // Invalid strategy if we get here
        assert(false);
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << p.get_name();
    return os;
}

