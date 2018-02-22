#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;
using namespace sf;

int main()
{
	cout << "Server" << endl;
	//ESTABLIM CONEXIÓ
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	Socket::Status status;
	sf::TcpListener listener;
	
	//Comprovem el port
	status = listener.listen(5000);
	if (status == Socket::Done)
		cout << "port ok" << endl;
	else
		cout << "port error" << endl;
	
	//Esperem a que es conecti tothom
	listener.setBlocking(false);
	int maxPlayers = 2;
	int numPlayers = 0;
	while (numPlayers < maxPlayers) {
		status = listener.accept(socket);
		if (status == Socket::Done) {
			std::cout << "Client " << numPlayers << " connected" << std::endl;
			numPlayers++;
		}		
	}
	cout << "All players are connected" << endl;
	listener.close();
	system("pause");
	
	
	return 0;
}