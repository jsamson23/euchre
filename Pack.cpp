#include <cassert>
#include <iostream>
#include <array>
#include "Pack.hpp"

using namespace std;

Pack::Pack() : next(0) {
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			cards[i * 6 + j] = Card(Rank(j+7), Suit(i));
		}
	}
}

Pack::Pack(std::istream& pack_input) : next(0) {
	for (int i = 0; i < 24; i++) {
		pack_input >> cards[i];
	}
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
	assert(next < cards.size());
	Card returnable = cards[next];
	next++;
	return returnable;
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
	next = 0;
}

void Pack::shuffle() {
	std::array<Card, PACK_SIZE> partition;
	for (int j = 0; j < 7; j++) {
		for (int i = 0; i < PACK_SIZE; i = i + 2) {
			partition[i+1] = cards[i/2];
			partition[i] = cards[PACK_SIZE/2 + i/2];
		}
		for (int i = 0; i < PACK_SIZE; i++) {
			cards[i] = partition[i];
		}
	}
	reset();

	
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
	return next == cards.size();
}