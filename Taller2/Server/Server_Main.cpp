#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>

using namespace std;
using namespace sf;

// Create a list to store the future clients
list<TcpSocket*> clients;
void ServerManager();
void SendAllPlayers(string msg, TcpSocket* clientToExclude);


int main()
{
	ServerManager();
	
	/*cout << "Server" << endl;
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
	system("pause");*/
	
	
	return 0;
}

void ServerManager()
{
	bool running = true;
	// Create a socket to listen to new connections
	TcpListener listener;
	Socket::Status status = listener.listen(5000);
	if (status != Socket::Done)
	{
		cout << "Error al abrir listener\n";
		exit(0);
	}
	else
		cout << "Port ok" << endl;

	// Create a selector
	SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);

	//Define the number of players you want
	int maxPlayers = 2;

	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					clients.push_back(client);
					// Add the new client to the selector so that we will be notified when he sends something
					selector.add(*client);

					//Avisem a tots els clients que hi ha una nova connexio
					SendAllPlayers("New client connected!", client);

					//Si tots els jugadors ja estan conectats avisem
					if (clients.size() == maxPlayers)
						cout << "Todos los jugadores estan conectados!" << endl;

				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					cout << "Error al recoger conexion nueva\n";
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				list<TcpSocket*>::iterator it = clients.begin();				

				while (it != clients.end())
				{
					bool shouldErase = false;
					TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive it
						Packet packet;
						status = client.receive(packet);
						if (status == Socket::Done && clients.size() == maxPlayers) //si estan tots els jugadors ja podem comen�ar a rebre i enviar missatges.
																					//Pero ho posem aqui i a dalt pq igualment volem comprovar desconnexions
						{
							string strRec;
							packet >> strRec;
							cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << endl;
						}
						else if (status == Socket::Disconnected)
						{
							selector.remove(client);
							shouldErase = true;
							cout << "Elimino el socket con puerto : " << client.getRemotePort() << " que se ha desconectado" << endl;
							SendAllPlayers("A client disconnected", &client);
						}
						else
						{
							cout << "Error al recibir de " << client.getRemotePort() << endl;
						}
					}
					//Si volem borrar elements de la llista hem de controlar que no ens sortim fora amb l'iterador
					if (shouldErase)
						it = clients.erase(it);
					else
						++it;
				}
			}
		}
	}

}

void SendAllPlayers(string msg,TcpSocket* clientToExclude) {
	for (list<TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		TcpSocket& clientRef = **it;
		if (&clientRef != clientToExclude)
			clientRef.send(msg.c_str(), msg.length());
	}
}