#include <iostream>
#include <string> //String
#include <cstdlib> //Random
#include <ctime> //Time
#include <vector> //Vector
#include <regex> //Regex tarkistus
#include <algorithm>
#include <random>

using namespace std;

class room {
	public :
		int room;
		int size;
};

class reservation {
	public:
		int room;
		string name;
		int resNumber;
		int days;
};

//Funktio luvun etsimiselle vektorista
int roomAvailable(vector<reservation> reservations, reservation reservation) {
	if (reservations.size() != 0) {
		for (int i = 0; i < reservations.size(); ++i)
		{
			if (reservations[i].room == reservation.room) {
				return 1;
			}
			if (reservations[i].resNumber == reservation.resNumber) {
				return 2;
			}
		}
		return 0;
	}
	else
	{
		return 0;
	}	
}

//Luo motellin huoneet väliltä 40-300 ja pitää huolen, että luku on parillinen
vector<room> configRooms() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(40, 300);
	int tempRooms = 0;
	vector<room> tempRoomList;
	roomCount:
	tempRooms = dist(gen);
	if (tempRooms % 2 == 1)
		goto roomCount;
	for (int i = 0; i < tempRooms; ++i) {
		int tempSize = 1;
		if (tempRooms / 2 < i + 1)
			tempSize = 2;
		room tempRoom = { (i + 1), tempSize };
		tempRoomList.push_back(tempRoom);
	}
	return tempRoomList;
}

//Varaa osa huoneista valmiiksi
vector<reservation> configReservations(int roomCount) {
	random_device rd;   // non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister.
	double toReserve = roomCount * 0.1;
	vector<reservation> tempReservations;

	for (int i = 0; i < toReserve; ++i) {
		reservation tempReserved = { 0,"Jack Daniels",0,0 };
		uniform_int_distribution<> dist(1, roomCount);
		tempReserved.room = dist(gen);
		uniform_int_distribution<> dista(10000, 99999);
		tempReserved.resNumber = dista(gen);
		uniform_int_distribution<> distb(1, 10);
		tempReserved.days = distb(gen);
		if (tempReservations.size() != 0) {
		check:
			int roomFound = roomAvailable(tempReservations, tempReserved);
			if (roomFound == 1)
			{
				uniform_int_distribution<> distc(1, roomCount);
				tempReserved.room = distc(gen);
				goto check;
			}
			else if (roomFound == 2)
			{
				uniform_int_distribution<> distd(10000, 99999);
				tempReserved.resNumber = distd(gen);
				goto check;
			}
			else
				tempReservations.push_back(tempReserved);
		}
		else
			tempReservations.push_back(tempReserved);
	}
	return tempReservations;
}

//Antaa varattujen huoneiden määrän koon perusteella
int reservedRooms(vector<reservation> reservations, int rooms, bool oneBed) {
	int tempReservedRooms = 0;
	for (int i = 0; i < reservations.size(); ++i)
	{
		if (reservations[i].room <= rooms / 2 && oneBed) {
			tempReservedRooms++;
		}
		else if(reservations[i].room > rooms / 2 && !oneBed)
			tempReservedRooms++;
	}
	int total = rooms / 2 - tempReservedRooms;
	return total;
}

//Kysyy käyttäjältä haluaako varata huoneen, etsiä huoneen vai poistua sivulta
bool askToBookOrSearch()
{
	char answer;
	while (true)
	{
		cout << "Would you like to reserve a room, find a reservation or exit? (r/s/e)\n";
		cin >> answer;
		answer = tolower(answer);
		if (answer == 'r' || answer == 's' || answer == 'e')
			break;

		cout << "Write either 'r', 's' or 'e'.\n";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (answer == 'e')
	{
		cout << "Thank you for visiting!\n";
		exit(0);
	}
	else if (answer == 'r')
	{
		return true;
	}
	else
		return false;
}

bool askNameOrNum()
{
	char answer;
	while (true)
	{
		cout << "Would you like to find a room by name or reservation number? (n/r)\n";
		cin >> answer;
		answer = tolower(answer);
		if (answer == 'n' || answer == 'r')
			break;

		cout << "Write either 'n' or 'r'.\n";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (answer == 'n')
	{
		return true;
	}
	else
		return false;
}

//Kysyy käyttäjältä huoneen koon ja tarkistaa vastauksen
int askSize(int oneA, int twoA) {
	int tempRoomSize = 0;
	while (true)
	{
		if (oneA == 0 || twoA == 0)
		{
			int roomAvailable;
			if (oneA == 0)
				roomAvailable = 2;
			else
				roomAvailable = 1;
			cout << "We only have " << roomAvailable << " bed rooms available. Write " << roomAvailable << " to confirm or 0 to cancel\n";
			cin >> tempRoomSize;
			if (!cin) {
				cin.clear(); // reset failbit
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
			}
			else {
				if (tempRoomSize == roomAvailable)
				{
					break;
				}
				else if (tempRoomSize == 0)
				{
					cout << "Thank you for visiting!\n";
					system("PAUSE");
					exit(0);
				}
				else
					cout << "We don't have rooms of that size.\n";
			}
		}
		else
		{
			cout << "Would you like to reserve a room for one or two (1/2)\n";
			cin >> tempRoomSize;
			if (!cin) {
				cin.clear(); // reset failbit
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
			}
			else {
				if (tempRoomSize == 1 || tempRoomSize == 2)
				{
					break;
				}
				else
					cout << "We don't have rooms of that size.\n";
			}
		}
	}
	return tempRoomSize;
}

//Kysyy käyttäjältä moneksiko päiväksi hän haluaa varata huoneen
int askDays() {
	int tempDays = 0;
	while (true)
	{
		cout << "For how many days would you like to reserve the room?\n";
		cin >> tempDays;
		if (!cin) {
			cin.clear(); // reset failbit
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		}
		if (tempDays > 0)
			break;

		cout << "Give an actual number.\n";
	}
	return tempDays;
}

//Kysyy käyttäjältä nimen ja hylkää sen jos ei käytä sallittuja merkkejä
string askName()
{
	string tempName;
	cin.ignore();
	while (true)
	{
		cout << "Your name: \n";
		getline(cin, tempName);
		if (regex_match(tempName, regex("^[a-z-A-Z -]+$")))
			break;
		cout << "Use only the alphabet, space and -.\n";
	}
	return tempName;
}

//Funktio huoneen numeron luomiselle (Koodi näyttää tän takia siistimmältä)
int tempRoomNum(int size, int rooms) {
	random_device rd;
	mt19937 gen(rd());
	int tempRoom;
	if (size == 1)
	{
		uniform_int_distribution<> dista(1, rooms / 2);
		tempRoom = dista(gen);
	}
	else {
		uniform_int_distribution<> dista(rooms / 2 + 1, rooms);
		tempRoom = dista(gen);
		cout << tempRoom;
	}
	return tempRoom;
}

void searchByName(vector<reservation> reservations)
{
	int tempFound = 0;
	locale loc;
	string tempUsersName = askName();
	for (auto& c : tempUsersName)
	{
		c = tolower(c);
	}
	string tempResName = "";

	for (int i = 0; i < reservations.size(); ++i)
	{
		//Nimellä haku niin että haku ei ole case sensitive
		tempResName = reservations[i].name;
		for (auto& c : tempResName)
		{
			c = tolower(c);
		}

		if (tempResName == tempUsersName) {
			if (tempFound == 0)
				cout << "\n--- Here are your reservations ---\n";
			cout << "\nReservation name: " << reservations[i].name;
			cout << "\nReservation number: " << reservations[i].resNumber;
			cout << "\nRoom number: " << reservations[i].room;
			cout << "\nReservation length: " << reservations[i].days << endl;
			tempFound++;
		}
	}
	if (tempFound == 0)
	{
		cout << "Sorry, there are no reservation with that name.\n";
	}
}

int askResNumber() {
	int tempResNum = 0;
	while (true)
	{
		cout << "What is your reservation number?\n";
		cin >> tempResNum;
		if (!cin) {
			cin.clear(); // reset failbit
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		}
		if (tempResNum > 9999 && tempResNum < 100000)
			break;

		cout << "Reservation numbers are five numbers long\n";
	}
	return tempResNum;
}

void searchByNumber(vector<reservation> reservations)
{
	int tempFound = 0;
	int tempResNum = askResNumber();

	for (int i = 0; i < reservations.size(); ++i)
	{
		if (reservations[i].resNumber == tempResNum) {
			if (tempFound == 0)
				cout << "\n--- Here is your reservation ---\n";
			cout << "\nReservation name: " << reservations[i].name;
			cout << "\nReservation number: " << reservations[i].resNumber;
			cout << "\nRoom number: " << reservations[i].room;
			cout << "\nReservation length: " << reservations[i].days << endl;
			tempFound++;
		}
	}
	if (tempFound == 0)
	{
		cout << "Sorry, there is no reservation with that reservation number.";
	}
}

//Tee uusi huonevaraus ja lisää se huonevaraus vectoriin
vector<reservation> newReservation(int size, string name, int days, vector<reservation> reservations, int rooms) {
	//Random käyttöön
	random_device rd;
	mt19937 gen(rd());

	//Random varausnumero 
	uniform_int_distribution<> dist(10000, 99999);
	int tempResNumber = dist(gen);

	//Tarkista huoneen koko ja anna oikeat huoneet mistä valita
	int tempRoom = tempRoomNum(size, rooms);

	//Temp muuttuja varaukselle
	reservation tempRes = { tempRoom,name,tempResNumber,days };

	//Tarkistus, että huone tai varausnumero ei ole jo varattu ja lisää vectoriin, jos ei ole
	check2:
	int roomFound = roomAvailable(reservations, tempRes);
	if (roomFound == 1)
	{
		int tempRoom = tempRoomNum(size, rooms);
		goto check2;
	}
	else if (roomFound == 2)
	{
		tempRes.resNumber = dist(gen);
		goto check2;
	}
	else
		reservations.push_back(tempRes);

	cout << "Your reservation number is " <<  tempRes.resNumber << "\n\n";

	//Palauta päivitetty vectori
	return reservations;
}

int main() {
	random_device rd;
	mt19937 gen(rd());
	vector<room> rooms = configRooms();
	vector<reservation> reserved = configReservations(int(rooms.size()));
	
	int oneBedAvailable = reservedRooms(reserved, int(rooms.size()), true);
	int twoBedAvailable = reservedRooms(reserved, int(rooms.size()), false);

	//"Sivu" alkaa	
	cout << "Welcome to Motel Kouvola!\n";
	if (oneBedAvailable == 0 && twoBedAvailable == 0)
	{
		cout << "Unfortunately all of the rooms are booked at the moment! \nThank you for visiting!\n";
		system("PAUSE");
		return 0;
	}
	cout << "Rooms available: " << rooms.size() - reserved.size() << "/" << rooms.size() << "\n \n";
	cout << "Pricing \n1 bed 100$/Night \n2 beds 150$/Night \nWe also raffle 10% and 20% discounts this week.\n\n";	

	
	answer:

	bool answer = askToBookOrSearch();

	if (answer)
	{
		int roomSize = askSize(oneBedAvailable, twoBedAvailable);

		int days = askDays();

		string name = askName();

		double total;
		int price;
		uniform_int_distribution<> dist(0, 2);
		int discountCode = dist(gen);
		double discount = 1;
		if (roomSize == 1)
			price = 100;
		else
			price = 150;
		if (discountCode == 1)
			discount = 0.9;
		else if (discountCode == 2)
			discount = 0.8;
		total = days * price * discount;

		cout << "\nYour total for " << days << " days in a " << roomSize << " bed room comes to a total of " << total << "$\n";

		reserved = newReservation(roomSize, name, days, reserved, int(rooms.size()));

		system("PAUSE");

		goto answer;
	}
	else
	{
		bool nameOrNum = askNameOrNum();
		if (nameOrNum)
			searchByName(reserved);
		else
			searchByNumber(reserved);

		system("PAUSE");
		goto answer;
	}
	system("PAUSE");
	return 0;
}