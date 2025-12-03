#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Card
{
private:
    string rank;
    string suit;
    int value;

public:
    Card( string& r, string& s)
    {
        rank = r; suit = s;
        if (rank == "Ace")
        {
            value = 11;
        }else if (rank == "King" || rank == "Queen"|| rank == "Jack")
        {
            value = 10;
        } else
        {
            value = stoi(rank);
        }
    }

    string getRank()
    {
        return rank;
    }

    int getValue()
    {
        return value;
    }

    void setValue(int v)
    {
        value = v;
    }

    string getCard()
    {
        return rank+ " of "+suit;
    }
};




class Deck
{
private:
    vector<Card> cards;

public:
    Deck()
    {
        const string suits[4] = {"Clubs","Diamonds","Hearts","Spades"};
        const string ranks[13]= {
            "2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"
        };
        for (string r : ranks)
        {
            for (string s:suits)
            {
                Card card = Card(r, s);
                cards.push_back(card);
            }
        }

        shuffleDeck();
    }

    void shuffleDeck()
    {
        random_device rd;
        mt19937 gen(rd());
        std::shuffle(cards.begin(), cards.end(), gen);
    }

    Card dealCard()
    {
        Card top = cards.back();
        cards.pop_back();
        return top;
    }

    int getCount() const { return cards.size(); }
};

class Hand
{
private:
    vector<Card> m_hand;
public:
    Hand()
    {

    }

    void clearHand()
    {
        m_hand.clear();
    }

    void operator += (Card card) {
        m_hand.push_back(card);
    }

    Card displayCard(int index)
    {
        return  m_hand[index];
    }

    int getPoints()
    {
        int total = 0;

        for (int i = 0; i < m_hand.size(); i++)
        {

            // For converting Aces later
            // if (m_hand[i].getValue() == 11)
            // {
            //     cout<<"Test: "<<m_hand[i].getCard()<<endl;
            // }

            total += m_hand[i].getValue();
        }

        return total;
    }

    int cardCount()
    {
        return m_hand.size();
    }
};

class Game {
private:
    Deck m_deck;
    Hand m_dealerHand;
    Hand m_playerHand;
public:
    Game(){
        m_deck = Deck();
        m_playerHand = Hand();
        m_dealerHand = Hand();
    };

    Hand getPlayerHand(Deck &d, Hand &h){
        char choice;

        do
        {
            cout<<"Your Cards:"<<endl;
            for (int i = 0; i< h.cardCount(); i++)
            {
                cout<<h.displayCard(i).getCard()<<endl;
            }

            do
            {
                cout<<endl<<"hit or stand? (h/s): ";
                cin >> choice;

                choice = tolower(choice);

                if (choice != 'h' && choice != 's')
                {
                    cout << "Invalid input. Please enter 'h' or 's'." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                }
            } while (choice != 'h' && choice != 's');

            if (choice == 'h')
            {
                if (d.getCount() == 0) {
                    d = Deck();
                }

                h+=(d.dealCard());

                if (h.getPoints() > 21 )
                {
                    break;
                }
            }

        } while (choice != 's');

        return h;
    };

    Hand getDealerHand(Deck &d, Hand &h){
        while (h.getPoints() < 17)
        {
            h+=d.dealCard();
        }

        return h;
    };

    void displayCards(Hand &h, string title)
    {
        cout <<title<<endl;
        for (int i = 0; i<h.cardCount(); i++)
        {
            cout<<h.displayCard(i).getCard()<<endl;
        }
        cout<<endl;
    };

    void displayRound()
    {
        displayCards(m_playerHand, "Your Cards: ");
        displayCards(m_dealerHand, "Dealer's Cards: ");

        cout << "Your Points: "<<m_playerHand.getPoints()<<endl;
        cout<< "Dealer's Points: "<<m_dealerHand.getPoints() << endl;

        if (m_playerHand.getPoints() > 21)
        {
            cout<<"Sorry. You busted. You lose."<<endl;
        } else if (m_dealerHand.getPoints()> 21)
        {
            cout<<"Yay! The dealer busted. You win!"<<endl;
        } else if (m_dealerHand.getPoints() == 21 && m_playerHand.getPoints() == 21)
        {
            cout<<"Dang, dealer has blackjack too. You push."<<endl;
        }else if (m_dealerHand.getPoints() ==21)
        {
            cout<<"Dealer has Blackjack. You lose."<<endl;
        } else if (m_playerHand.getPoints() == 21)
        {
            cout<<"Blackjack! You win!"<<endl;
        }else if (m_dealerHand.getPoints() == m_playerHand.getPoints()){
            cout<<"It's a tie! You push."<<endl;
        } else if (m_playerHand.getPoints() > m_dealerHand.getPoints())
        {
            cout<<"Hooray! You win!"<<endl;
        } else if (m_dealerHand.getPoints() > m_playerHand.getPoints())
        {
            cout<<"Sorry. You lose."<<endl;
        } else
        {
            cout<<"I'm not sure what happened."<<endl;
        }

        cout<<endl;
    }

    void playGame()
    {
        char again;
        do
        {


            cout<<"###########################"<<endl;
            cout<<"#  The Game of Blackjack  #"<<endl;
            cout<<"###########################"<<endl<<endl;

            m_playerHand+=m_deck.dealCard();
            m_dealerHand+=m_deck.dealCard();
            m_playerHand+=m_deck.dealCard();

            displayCards(m_dealerHand, "Dealer's Show Card: ");

            getPlayerHand(m_deck, m_playerHand);

            getDealerHand(m_deck, m_dealerHand);

            displayRound();

            m_dealerHand.clearHand();
            m_playerHand.clearHand();

            do
            {
                cout<<endl<<"play another round? (y/n): ";
                cin >> again;

                again = tolower(again);

                if (again != 'y' && again != 'n')
                {
                    cout << "Invalid input. Please enter 'y' or 'n'." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                }
            } while (again != 'y' && again != 'n');
        }while (again != 'n');
    } ;

};



int main()
{
    Game newGame = Game();

    newGame.playGame();

    return 0;
}