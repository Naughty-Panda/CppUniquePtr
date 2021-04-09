//////////////////////////////////////////
//	C++ Unique ptr
//	Naughty Panda @ 2021
//////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

//////////////////////////////////////////
//	7.1
//////////////////////////////////////////

class Date {

private:
	uint8_t _day{ 0 };
	uint8_t _month{ 0 };
	uint16_t _year{ 0 };

public:
	Date(const uint8_t& d, const uint8_t& m, const uint16_t& y) : _day(d), _month(m), _year(y) {}

	uint8_t GetDay() const { return _day; }
	uint8_t GetMonth() const { return _month; }
	uint16_t GetYear() const { return _year; }

	void SetDay(const uint8_t& d) { _day = d; }
	void SetMonth(const uint8_t& m) { _month = m; }
	void SetYear(const uint16_t& y) { _year = y; }

	//friend std::ostream& operator<<(std::ostream& ostr, const Date& date);
};

std::ostream& operator<< (std::ostream& ostr, const Date& date) {

	ostr << static_cast<int>(date.GetDay()) << '.';
	ostr << static_cast<int>(date.GetMonth()) << '.';
	ostr << static_cast<int>(date.GetYear());
	return ostr;
}

bool operator> (const Date& d1, const Date& d2) {

	if (d1.GetYear() > d2.GetYear()) return true;
	if (d1.GetYear() < d2.GetYear()) return false;
	if (d1.GetMonth() > d2.GetMonth()) return true;
	if (d1.GetMonth() < d2.GetMonth()) return false;
	if (d1.GetDay() > d2.GetDay()) return true;
	if (d1.GetDay() < d2.GetDay()) return false;
}

//////////////////////////////////////////
//	7.2
//////////////////////////////////////////

std::ostream& operator<< (std::ostream& ostr, const std::unique_ptr<Date>& date) {

	ostr << *date.get();
	return ostr;
}

auto CompareDates(const std::unique_ptr<Date>& d1, const std::unique_ptr<Date>& d2) {

	return (*d1.get() > *d2.get()) ? &d1 : &d2;
}

void SwapUniquePointers(std::unique_ptr<Date>& d1, std::unique_ptr<Date>& d2) {

	// Okay, we'll not be using built-in swap method
	//d1.swap(d2);

	auto temp = std::move(d1);
	d1 = std::move(d2);
	d2 = std::move(temp);
}

//////////////////////////////////////////
//	Blackjack classes
//////////////////////////////////////////

enum class ECardSuit : uint8_t {

	Clubs = 1U, Diamonds, Hearts, Spades,
	Undefined = 0U
};

ECardSuit operator++(const ECardSuit& suit) {

	switch (suit) {
	case ECardSuit::Clubs:		return ECardSuit::Diamonds;
	case ECardSuit::Diamonds:	return ECardSuit::Hearts;
	case ECardSuit::Hearts:		return ECardSuit::Spades;
	case ECardSuit::Spades:		return ECardSuit::Clubs;
	case ECardSuit::Undefined:	return ECardSuit::Undefined;
	default:					return ECardSuit::Undefined;
	}
}

std::ostream& operator<< (std::ostream& ostr, const ECardSuit& suit) {

	char SuitSymbol;
	switch (suit) {

	case ECardSuit::Clubs:		SuitSymbol = 'C'; break;
	case ECardSuit::Diamonds:	SuitSymbol = 'D'; break;
	case ECardSuit::Hearts:		SuitSymbol = 'H'; break;
	case ECardSuit::Spades:		SuitSymbol = 'S'; break;
	default:					SuitSymbol = 'X'; break;
	}

	ostr << SuitSymbol;
	return ostr;
}

enum class ECardValue : uint8_t {

	Two = 2U, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
	Jack = 10U, Queen = 10U, King = 10U, Ace = 1U,
	Undefined = 0U
};

ECardValue operator++(const ECardValue& val) {

	switch (val) {
	case ECardValue::Ace:		return ECardValue::Two;
	case ECardValue::Two:		return ECardValue::Three;
	case ECardValue::Three:		return ECardValue::Four;
	case ECardValue::Four:		return ECardValue::Five;
	case ECardValue::Five:		return ECardValue::Six;
	case ECardValue::Six:		return ECardValue::Seven;
	case ECardValue::Seven:		return ECardValue::Eight;
	case ECardValue::Eight:		return ECardValue::Nine;
	case ECardValue::Nine:		return ECardValue::Ten;
	case ECardValue::Ten:		return ECardValue::Jack;
	//case ECardValue::Jack:		return ECardValue::Queen;
	//case ECardValue::Queen:		return ECardValue::King;
	//case ECardValue::King:		return ECardValue::Ace;
	case ECardValue::Undefined: return ECardValue::Undefined;
	default:					return ECardValue::Undefined;
	}
}

std::ostream& operator<< (std::ostream& ostr, const ECardValue& val) {
	/*
	std::string CardValue;

	switch (val) {
	case ECardValue::Ace:	CardValue = 'A'; break;
	case ECardValue::Two:	CardValue = '2'; break;
	case ECardValue::Three: CardValue = '3'; break;
	case ECardValue::Four:	CardValue = '4'; break;
	case ECardValue::Five:	CardValue = '5'; break;
	case ECardValue::Six:	CardValue = '6'; break;
	case ECardValue::Seven: CardValue = '7'; break;
	case ECardValue::Eight: CardValue = '8'; break;
	case ECardValue::Nine:	CardValue = '9'; break;
	case ECardValue::Ten:	CardValue = "10"; break;
	default:				CardValue = '0'; break;
	}
	*/
	ostr << static_cast<int>(val);
	return ostr;
}

class Card {

protected:
	ECardSuit _suit{ ECardSuit::Undefined };
	ECardValue _value{ ECardValue::Undefined };
	bool _bVisible{ true };

public:
	Card(ECardSuit suit, ECardValue val) : _suit(suit), _value(val) {}

	ECardValue GetValue() const { return _value; }
	void Flip() { _bVisible = !_bVisible; }

	friend std::ostream& operator << (std::ostream& ostr, const Card& card);
};

class Hand {

protected:
	std::vector<Card*> _hand;

public:
	Hand() { _hand.reserve(7); }
	virtual ~Hand() {}

	void Add(Card* card);
	void Clear();
	uint8_t GetValue() const;
};

class GenericPlayer : public Hand {

protected:
	std::string _name{ "noname" };

public:
	GenericPlayer(const char* name) : _name(name) {}
	virtual ~GenericPlayer() override {}

	std::string GetName() const { return _name; }
	virtual bool IsHitting() const = 0;
	bool IsBusted() const { return GetValue() > 21U ? true : false; }
	void Bust() const { std::cout << _name << " busted!\n"; }

	friend std::ostream& operator<< (std::ostream& ostr, const GenericPlayer& player);

};

class Player : public GenericPlayer {

public:
	Player(const char* name) : GenericPlayer(name) {}
	~Player() override {}

	bool IsHitting() const override;
	void Win() const { std::cout << _name << " wins!\n"; }
	void Lose() const { std::cout << _name << " loses!\n"; }
	void Push() const { std::cout << _name << " pushes!\n"; }
};

class House : public GenericPlayer {

public:
	House() : GenericPlayer("House") {}
	~House() override {}

	bool IsHitting() const override;
	void FlipFirstCard();
};

class Deck : public Hand {

public:
	Deck() { _hand.reserve(52); Populate(); }
	~Deck() override {}

	void Populate();
	void Shuffle();
	void Deal(Hand& hand);
	void AdditionalCards(GenericPlayer& player);
};

class Game {

private:
	Deck _deck;
	House _house;
	std::vector<Player> _players;

public:
	Game(const std::vector<std::string>& names);
	~Game() {}

	void Play();
};

//////////////////////////////////////////
//	Card methods
//////////////////////////////////////////

std::ostream& operator<< (std::ostream& ostr, const Card& card) {

	if (!card._bVisible) {

		ostr << "XX";
		return ostr;
	}

	// TODO: FIX
	// const char Values[]{ '0', 'A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K' };
	// const char Suits[]{ 'c', 'd', 'h', 's' };

	ostr << card._value << card._suit;
	return ostr;
}

//////////////////////////////////////////
//	Hand methods
//////////////////////////////////////////

void Hand::Add(Card* card) {

	assert(card);

	_hand.push_back(card);
}

void Hand::Clear() {

	std::vector<Card*>::iterator i = _hand.begin();
	for (i = _hand.begin(); i != _hand.end(); ++i) {
		delete* i;
		*i = 0;
	}
	_hand.clear();
	//_hand.erase(_hand.begin(), _hand.end());
}

uint8_t Hand::GetValue() const {

	uint8_t result{ 0U };
	uint8_t AceCount{ 0U };

	for (auto card : _hand) {

		if (card->GetValue() == ECardValue::Ace)
			++AceCount;
		else
			result += static_cast<int>(card->GetValue());
	}

	// Here we check if one/first of our Aces can be 11
	// It can be 11 if our hand value <= 10 - (AceCount - 1)
	if (AceCount != 0U && result <= 21U - 11U - (AceCount - 1U)) {

		result += 11U;
		--AceCount;
	}

	// If Ace cant't be 11, it'll be 1
	// for each of Aces left in hand
	result += AceCount;

	return result;
}

//////////////////////////////////////////
//	Player methods
//////////////////////////////////////////

bool Player::IsHitting() const {

	std::cout << _name << ", do you want a hit? (Y/N): ";
	char input;
	std::cin >> input;
	return (input == 'y' || input == 'Y');
}

std::ostream& operator<< (std::ostream& ostr, const GenericPlayer& player) {

	ostr << player._name << ":\t";

	std::vector<Card*>::const_iterator card;

	if (!player._hand.empty()) {
		for (card = player._hand.begin(); card != player._hand.end(); ++card) {
			ostr << *(*card) << "\t";
		}

		if (player.GetValue() != 0) {
			std::cout << '(' << static_cast<int>(player.GetValue()) << ')';
		}
	}
	else {
		ostr << "<empty>\n";
	}

	return ostr;
}

//////////////////////////////////////////
//	House methods
//////////////////////////////////////////

bool House::IsHitting() const {

	return (GetValue() <= 16);
}

void House::FlipFirstCard() {

	if (_hand.empty()) {
		std::cout << "No cards to flip!\n";
		return;
	}

	_hand[0]->Flip();
}

//////////////////////////////////////////
//	Deck methods
//////////////////////////////////////////

void Deck::Populate() {

	Clear();

	for (uint8_t s = static_cast<uint8_t>(ECardSuit::Clubs); s <= static_cast<uint8_t>(ECardSuit::Spades); ++s) {
		for (uint8_t v = static_cast<uint8_t>(ECardValue::Ace); v <= static_cast<uint8_t>(ECardValue::Ten); ++v) {
			Add(new Card(static_cast<ECardSuit>(s), static_cast<ECardValue>(v)));
		}
		Add(new Card(static_cast<ECardSuit>(s), ECardValue::Jack));
		Add(new Card(static_cast<ECardSuit>(s), ECardValue::Queen));
		Add(new Card(static_cast<ECardSuit>(s), ECardValue::King));
	}
}

void Deck::Shuffle() {

	std::random_shuffle(_hand.begin(), _hand.end());
}

void Deck::Deal(Hand& hand) {

	if (!_hand.empty()) {
		hand.Add(_hand.back());
		_hand.pop_back();
	}
	else std::cout << "Out of cards. Unable to deal.\n";
}

void Deck::AdditionalCards(GenericPlayer& player) {

	while (!player.IsBusted() && player.IsHitting()) {
		Deal(player);
		std::cout << player.GetName() << " takes a card.\n";
		std::cout << player << '\n';

		if (player.IsBusted()) {
			player.Bust();
		}
	}
}

//////////////////////////////////////////
//	Game methods
//////////////////////////////////////////

Game::Game(const std::vector<std::string>& names) {

	std::vector<std::string>::const_iterator name;

	for (name = names.begin(); name != names.end(); ++name) {
		_players.push_back(Player(name->c_str()));
	}

	srand(static_cast<unsigned int>(time(NULL)));
	_deck.Populate();
	_deck.Shuffle();
}

void Game::Play() {

	std::vector<Player>::iterator player;
	for (size_t i = 0; i < 2; ++i) {
		for (player = _players.begin(); player != _players.end(); ++player) {

			_deck.Deal(*player);
		}
		_deck.Deal(_house);
	}

	_house.FlipFirstCard();

	for (player = _players.begin(); player != _players.end(); ++player) {

		std::cout << *player << '\n';
	}

	std::cout << _house << '\n';

	for (player = _players.begin(); player != _players.end(); ++player) {

		_deck.AdditionalCards(*player);	
	}

	_house.FlipFirstCard();
	std::cout << _house << '\n';

	_deck.AdditionalCards(_house);

	if (_house.IsBusted()) {

		// Players wins
		for (player = _players.begin(); player != _players.end(); ++player) {

			if (!player->IsBusted()) {
				player->Win();
			}
		}

	}
	else {

		// Compares scores for players and house
		for (player = _players.begin(); player != _players.end(); ++player) {

			if (!player->IsBusted()) {

				if (player->GetValue() > _house.GetValue()) {
					player->Win();
				}
				else if (player->GetValue() < _house.GetValue()) {

					player->Lose();
				}
				else {
					player->Push();
				}
			}
		}

		for (player = _players.begin(); player != _players.end(); ++player) {
			
			player->Clear();
		}

		_house.Clear();
	}
}


//////////////////////////////////////////
//	Main
//////////////////////////////////////////

int main() {

	//////////////////////////////////////////
	//	7.1
	//////////////////////////////////////////

	std::unique_ptr<Date> today = std::make_unique<Date>(4,8,2021);

	std::cout << "Size of \"today\": "		<< sizeof(today) << " bytes"			<< '\n';
	std::cout << "Day of \"today\": "		<< static_cast<int>(today->GetDay())	<< '\n';
	std::cout << "Month of \"today\": "		<< static_cast<int>(today->GetMonth())	<< '\n';
	std::cout << "Year of \"today\": "		<< static_cast<int>(today->GetYear())	<< '\n';
	std::cout << "Adress of \"today\": "	<< &today								<< '\n';
	std::cout << "Today's date: "			<< today								<< '\n';

	std::unique_ptr<Date> date = std::move(today);

	std::cout << "Adress of \"date\": "		<< date									<< '\n';
	std::cout << "\"today\" is "			<< (today ? "not nullptr" : "nullptr")	<< '\n';

	//////////////////////////////////////////
	//	7.2
	//////////////////////////////////////////

	auto date1 = std::make_unique<Date>(4, 8, 2021);
	auto date2 = std::make_unique<Date>(10, 21, 2022);

	std::cout << "Latest date: "	<< *CompareDates(date1, date2)	<< '\n';
	std::cout << "Date 1 was: "		<< date1						<< '\n';
	std::cout << "Date 2 was: "		<< date2						<< '\n';

	SwapUniquePointers(date1, date2);

	std::cout << "Date 1 is: " << date1 << '\n';
	std::cout << "Date 2 is: " << date2 << '\n';

	//////////////////////////////////////////
	//	Blackjack
	//////////////////////////////////////////

	bool NewGame{ true };

	while (NewGame) {

		std::vector<std::string>* players = new std::vector<std::string>;
		players->push_back("First Player");
		players->push_back("Second Player");
		players->push_back("Third Player");

		Game* game = new Game(*players);
		game->Play();

		delete players;
		delete game;

		NewGame = false;
		char result;
		std::cout << "Another round? [Y/N]: ";
		std::cin >> result;
		if (result == 'Y' || result == 'y') NewGame = true;
	}

	return 0;
}
