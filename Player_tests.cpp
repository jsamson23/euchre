#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    Player* jimmy = Player_factory("Jimmy", "Human");
    ASSERT_EQUAL("Alice", alice->get_name());
    ASSERT_EQUAL("Jimmy", jimmy->get_name());

    delete alice;
    delete jimmy;
}

// Add more tests here

TEST(test_player_insertion) {
    // Create a Human player
    Player* human = Player_factory("NotRobot", "Human");

    // Print the player using the stream insertion operator
    ostringstream oss1;
    oss1 << *human;

    // Verify that the output is the player's name
    ASSERT_EQUAL(oss1.str(), "NotRobot");

    // Create a Simple player
    Player* alice = Player_factory("Alice", "Simple");

    // Print the player using the stream insertion operator
    ostringstream oss2;
    oss2 << *alice;
    ASSERT_EQUAL(oss2.str(), "Alice");

    // Clean up players that were created using Player_factory()
    delete human;
    delete alice;
}

TEST(test_simple_add_card_and_play_card) {
    Player* john = Player_factory("John", "Simple");
    Card arbitraryLed(TEN, SPADES);
    Suit trump = DIAMONDS;
    john->add_card(Card(JACK, HEARTS));
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(ACE, SPADES));
    john->add_card(Card(NINE, DIAMONDS));
    john->add_card(Card(TEN, CLUBS));

    Card played = john->play_card(arbitraryLed, trump);
    Card ace_spades(ACE, SPADES);
    ASSERT_EQUAL(played, ace_spades);

    played = john->play_card(arbitraryLed, trump);
    Card king_spades(KING, SPADES);
    ASSERT_EQUAL(played, king_spades);

    played = john->play_card(arbitraryLed, trump);
    Card ten_clubs(TEN, CLUBS);
    ASSERT_EQUAL(ten_clubs, played);

    delete john;

    Player* alice = Player_factory("Alice", "Simple");
    Card arbitraryLed2(ACE, CLUBS);
    alice->add_card(Card(JACK, HEARTS));
    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(ACE, SPADES));
    alice->add_card(Card(NINE, DIAMONDS));
    alice->add_card(Card(TEN, CLUBS));

    played = alice->play_card(arbitraryLed2, trump);
    ASSERT_EQUAL(ten_clubs, played);

    delete alice;

}

TEST(test_make_trump) {
    int round = 1;


    Card arbitraryLed(KING, SPADES);
    Card arbitraryLed2(KING, CLUBS);
    Card arbitraryLed3(TEN, DIAMONDS);
    Suit arbitraryOrderUp = DIAMONDS;


    bool isDealer = false;


    Player* john = Player_factory("John", "Simple");


    john->add_card(Card(JACK, HEARTS));
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(ACE, SPADES));
    john->add_card(Card(NINE, DIAMONDS));
    john->add_card(Card(TEN, CLUBS));

    //TEST 1
    bool result = john->make_trump(arbitraryLed, isDealer, round, arbitraryOrderUp);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(arbitraryOrderUp, SPADES);

    //TEST 2
    result = john->make_trump(arbitraryLed2, isDealer, round, arbitraryOrderUp);
    ASSERT_FALSE(result);
    ASSERT_EQUAL(arbitraryOrderUp, SPADES); // still spades from last time

    round = 2;


    //TEST 3
    result = john->make_trump(arbitraryLed, isDealer, round, arbitraryOrderUp);
    ASSERT_FALSE(result);
    ASSERT_EQUAL(arbitraryOrderUp, SPADES);

    //TEST 4
    result = john->make_trump(arbitraryLed2, isDealer, round, arbitraryOrderUp);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(arbitraryOrderUp, SPADES);


    //TEST 5
    result = john->make_trump(arbitraryLed3, isDealer, round, arbitraryOrderUp);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(arbitraryOrderUp, HEARTS);

    isDealer = true;


    //TEST 6
    result = john->make_trump(arbitraryLed, isDealer, round, arbitraryOrderUp);
    ASSERT_TRUE(result);
    ASSERT_EQUAL(arbitraryOrderUp, CLUBS);

    



    delete john;
    

}

TEST(test_simple_lead_card) {
    Player* john = Player_factory("John", "Simple");
    Suit trump = DIAMONDS;
    john->add_card(Card(JACK, HEARTS));
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(ACE, SPADES));
    john->add_card(Card(NINE, DIAMONDS));
    john->add_card(Card(TEN, CLUBS));
    Card test = john->lead_card(trump);
    Card ace_spades(ACE, SPADES);
    ASSERT_EQUAL(test, ace_spades);


    delete john;

    john = Player_factory("John", "Simple");
    trump = SPADES;
    john->add_card(Card(JACK, HEARTS));
    john->add_card(Card(KING, SPADES));
    john->add_card(Card(ACE, SPADES));
    john->add_card(Card(NINE, DIAMONDS));
    john->add_card(Card(TEN, CLUBS));
    test = john->lead_card(trump);
    Card jack_hearts(JACK, HEARTS);
    ASSERT_EQUAL(test, jack_hearts);

    delete john;

}

TEST(simple_add_and_discard_card) {
    Player* john = Player_factory("John", "Simple");
    Card arbitraryUpCard(NINE, DIAMONDS);
    Suit trump = DIAMONDS;
    john->add_card(Card(JACK, SPADES));
    john->add_and_discard(arbitraryUpCard);
    // he will only have one card. just test whether he discarded the right one
    Card placeholder = Card();
    Card test = john->play_card(placeholder, trump);
    ASSERT_EQUAL(test, arbitraryUpCard);

    // TEST 2: DISCARDS UPCARD
    Player* alice = Player_factory("Alice", "Simple");
    Card jack_diamonds(JACK, DIAMONDS);
    alice->add_card(Card(JACK, DIAMONDS));
    alice->add_and_discard(arbitraryUpCard);

    test = alice->play_card(placeholder, trump);
    ASSERT_EQUAL(test, jack_diamonds);
    delete john;
    delete alice;

    


}


TEST_MAIN()
