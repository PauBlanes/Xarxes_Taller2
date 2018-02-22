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
	//ESTABLIM CONEXI�
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();	
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
	vector<TcpSocket> sockets(maxPlayers);
	int connectedPlayers = 0;
	while (connectedPlayers < maxPlayers) {
		status = listener.accept(sockets[connectedPlayers]);
		if (status == Socket::Done) {
			std::cout << "Client " << connectedPlayers << " connected" << std::endl;
			connectedPlayers++;
		}		
	}
	cout << "All players are connected" << endl;
	listener.close();
	system("pause");
	
	
	return 0;
}