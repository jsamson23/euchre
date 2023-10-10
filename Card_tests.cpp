#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    Card p(TWO, SPADES);
    Card j = Card();
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
    ASSERT_EQUAL(TWO, p.get_rank());
    ASSERT_EQUAL(SPADES, p.get_suit());
    ASSERT_EQUAL(TWO, j.get_rank());
    ASSERT_EQUAL(SPADES, j.get_suit());
}

// Add more test cases here

TEST(test_get_suit_overloaded_and_rank) {
    Card c(JACK, HEARTS);
    Suit trump = DIAMONDS;

    ASSERT_EQUAL(c.get_suit(trump), DIAMONDS);
    ASSERT_EQUAL(c.get_rank(), JACK);
}

TEST(test_face_or_ace) {
    for (int j = TWO; j <= ACE; ++j) {
        for (int i = SPADES; i <= DIAMONDS; ++i) {
            Card* testCard = new Card(static_cast<Rank>(j), static_cast<Suit>(i));
            if (j >= JACK) {
                ASSERT_TRUE(testCard->is_face_or_ace());
            }
            else {
                ASSERT_FALSE(testCard->is_face_or_ace());
            }
            delete testCard;
        }
    }
}

TEST(test_right_and_left_bower) {
    Suit trump = SPADES;
    for (int j = TWO; j <= ACE; ++j) {
        for (int i = SPADES; i <= DIAMONDS; ++i) {
            Card* testCard = new Card(static_cast<Rank>(j), static_cast<Suit>(i));
            if (j == JACK && i == SPADES) {
                ASSERT_TRUE(testCard->is_right_bower(trump));
            }
            else if (j == JACK && i == CLUBS) {
                ASSERT_TRUE(testCard->is_left_bower(trump));
            }
            else {
                ASSERT_FALSE(testCard->is_right_bower(trump));
                ASSERT_FALSE(testCard->is_left_bower(trump));
            }
            delete testCard;
        }
    }
}

TEST(test_is_trump) {
    Suit trump = HEARTS;
    for (int j = TWO; j <= ACE; ++j) {
        for (int i = SPADES; i <= DIAMONDS; ++i) {
            Card* testCard = new Card(static_cast<Rank>(j), static_cast<Suit>(i));
            if (i == HEARTS) {
                ASSERT_TRUE(testCard->is_trump(trump));
            }
            else if (testCard->is_left_bower(trump)) {
                ASSERT_TRUE(testCard->is_trump(trump));
            }
            else {
                ASSERT_FALSE(testCard->is_trump(trump));
                ASSERT_FALSE(testCard->is_trump(trump));
            }
            delete testCard;
        }
    }
}

TEST(test_suit_next) {
    Suit test1 = HEARTS;
    Suit test2 = SPADES;
    ASSERT_EQUAL(Suit_next(test1), DIAMONDS);
    ASSERT_EQUAL(Suit_next(test2), CLUBS);
    ASSERT_EQUAL(Suit_next(Suit_next(test1)), HEARTS);
    ASSERT_EQUAL(Suit_next(Suit_next(test2)), SPADES);
}

TEST(test_operators) {
    Card a(ACE, DIAMONDS);
    Card b(JACK, CLUBS);
    Card c(JACK, CLUBS);
    ASSERT_TRUE(a > b);
    ASSERT_FALSE(a < b);
    ASSERT_TRUE(b == c);
    ASSERT_TRUE(b <= c);
    ASSERT_TRUE(b >= c);
    ASSERT_FALSE(b != c);
    ASSERT_TRUE(a != b);
}

TEST(test_card_less) {
    Suit trump = CLUBS;
    Card led_card = Card(KING, DIAMONDS);
    Card a(JACK, SPADES);
    Card b(KING, CLUBS);
    Card c(QUEEN, DIAMONDS);
    Card d(ACE, HEARTS);
    ASSERT_FALSE(Card_less(a, b, led_card, trump));
    ASSERT_FALSE(Card_less(b, c, led_card, trump));
    ASSERT_FALSE(Card_less(c, d, led_card, trump));
    ASSERT_TRUE(Card_less(b, a, led_card, trump));
    ASSERT_TRUE(Card_less(c, b, led_card, trump));
    ASSERT_TRUE(Card_less(d, c, led_card, trump)); 
    // Ace of Hearts is less than Queen of Diamonds
    ASSERT_FALSE(Card_less(a, b, trump));
    ASSERT_FALSE(Card_less(b, d, trump));

}

TEST_MAIN()
