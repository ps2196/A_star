#include "PathFinder.h"


using namespace std;

void visualize(Graph g, Path& sp)
{
	// load font
	sf::Font font;
	if (!font.loadFromFile("bebas-neue-bold.ttf"))
	{
		cout << "ERROR FONT" << endl;
	}

	sf::Text textN;
	textN.setFont(font);
	textN.setFillColor(sf::Color::White);
	textN.setCharacterSize(20);

	sf::Text textE;
	textE.setFont(font);
	textE.setFillColor(sf::Color::White);
	textE.setCharacterSize(20);

	// visualization
	sf::RenderWindow window(sf::VideoMode(1024, 512), "Graph visualization");
	sf::CircleShape gNode(5.0f);
	gNode.setFillColor(sf::Color::Green);
	gNode.setOrigin(2.5f, 2.5f);
	sf::CircleShape rNode(5.0f);
	rNode.setFillColor(sf::Color::Red);
	rNode.setOrigin(2.5f, 2.5f);
	sf::VertexArray gLine(sf::Lines, 2);
	gLine[0].color = sf::Color::Green;
	gLine[1].color = sf::Color::Green;
	sf::VertexArray rLine(sf::Lines, 2);
	rLine[0].color = sf::Color::Red;
	rLine[1].color = sf::Color::Red;

	float scale = 1;
	float moveX, moveY;
	moveX = moveY = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				moveX += 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				moveX -= 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				moveY -= 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				moveY += 4.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				scale *= 2.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				scale *= 0.5f;
			}
		}


		window.clear();
		for (auto i = g.nodes.begin(); i != g.nodes.end(); i++)
		{
			Node& n = i->second;
			gNode.setPosition(scale*(n.point.x) + moveX, scale*(n.point.y) + moveY);
			textN.setPosition(scale*(n.point.x) + moveX, scale*(n.point.y) + moveY);
			textN.setString("P" + n.name);
			if (scale > 0.1f)
				window.draw(textN);
			window.draw(gNode);
			for (auto j = n.edges.begin(); j != n.edges.end(); j++)
			{
				textE.setPosition(scale*(0.5f*(j._Ptr->node1->point.x + j._Ptr->node2->point.x)) + moveX, scale*(0.5f*(j._Ptr->node1->point.y + j._Ptr->node2->point.y)) + moveY);
				textE.setString(to_string((int)(j._Ptr->fee)));
				gLine[0].position = sf::Vector2f(scale*(n.point.x) + moveX, scale*(n.point.y) + moveY);
				gLine[1].position = sf::Vector2f(scale*(j._Ptr->node1->point.x) + moveX, scale*(j._Ptr->node1->point.y) + moveY);
				if (scale > 0.1f)
					window.draw(textE);
				window.draw(gLine);
			}
		}

		for (auto i = sp.begin(); i != sp.end(); i++)
		{
			Node* n = *i;
			auto j = i;
			j++;
			rNode.setPosition(scale*(n->point.x) + moveX, scale*(n->point.y) + moveY);
			window.draw(rNode);
			if (j != sp.end())
			{
				Node* m = *j;
				rLine[0].position = sf::Vector2f(scale*(n->point.x) + moveX, scale*(n->point.y) + moveY);
				rLine[1].position = sf::Vector2f(scale*(m->point.x) + moveX, scale*(m->point.y) + moveY);
				window.draw(rLine);
			}


		}
		window.display();
	}
	getchar();

}