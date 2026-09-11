#pragma once

#include "global.h"

typedef unsigned char       BYTE;

struct Window {
	sf::RenderWindow win;
	sf::Sprite sprite;
	sf::Texture texture;

	sf::Vector2u windowSize;

	sf::RectangleShape draggingPlace;
	sf::RectangleShape WinResizePlace;

	Window() = delete;

	Window(sf::Vector2u winSize, fs::path pth): texture(), sprite(texture), windowSize(winSize) {
		win.create(sf::VideoMode(windowSize), "", sf::Style::None, sf::State::Windowed);

		initSpr(pth);

		draggingPlace.setSize({ 35.0f, 35.0f });

		draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));

		draggingPlace.setOrigin(sf::Vector2f(draggingPlace.getLocalBounds().size.x, 0.0f));

		draggingPlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 0.0f));



		WinResizePlace.setSize(draggingPlace.getSize());

		WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));

		WinResizePlace.setOrigin(sf::Vector2f(WinResizePlace.getLocalBounds().size.x, WinResizePlace.getLocalBounds().size.y));

		WinResizePlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 1.0f));

	}


	~Window() {
		Print("Single window", "Window is closed!");
		win.close();
	}

	//===Sprite/Texture methods===

	void initSpr(fs::path path) {

		Print("Single window", "Init Sprite");

		if (!texture.loadFromFile(path)) {
			Print("Single window", "Texture couldn't load!");
			return;
		}

		Print("Single window", path);

		

		sprite.setTexture(texture, true);

		sf::Vector2f newSize;

		newSize.x = static_cast<float>(windowSize.x) / static_cast<float>(sprite.getTexture().getSize().x);
		newSize.y = static_cast<float>(windowSize.y) / static_cast<float>(sprite.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

		sprite.setScale({ scale, scale });

		sprite.setOrigin({ sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2 });
		sprite.setPosition(WindowSizeNor<float>(windowSize, 0.5f, 0.5f));

		Print("Single window", "X " + std::to_string(sprite.getTexture().getSize().x) + " Y " + std::to_string(sprite.getTexture().getSize().y));
	}

	void resizeSpr() {
		sf::Vector2f newSize;

		newSize.x = static_cast<float>(windowSize.x) / static_cast<float>(sprite.getTexture().getSize().x);
		newSize.y = static_cast<float>(windowSize.y) / static_cast<float>(sprite.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

		sprite.setScale({ scale, scale });

		sprite.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
	}

	//===Window methods===

	//void eventHandler() {

	//	while (const std::optional event = win.pollEvent()) {

	//		if (const auto* KP = event->getIf<sf::Event::KeyPressed>()) {
	//			if (KP->scancode == sf::Keyboard::Scan::Escape) {
	//				~Window();
	//			}
	//		}

	//	}

	//}

	void resizeWindow(sf::Vector2u NewSize) {
		sf::View view(sf::FloatRect({ 0.0f, 0.0f }, sf::Vector2f(NewSize)));
		win.setView(view);

		windowSize = NewSize;

		draggingPlace.setOrigin(sf::Vector2f(draggingPlace.getLocalBounds().size.x, 0.0f));

		draggingPlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 0.0f));


		WinResizePlace.setOrigin(sf::Vector2f(WinResizePlace.getLocalBounds().size.x, WinResizePlace.getLocalBounds().size.y));

		WinResizePlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 1.0f));

	}

	void draw() {
		if (win.isOpen()) {
			//Print("Single window", "Is drawing");
			win.clear(sf::Color::Black);

			win.draw(sprite);
			win.draw(draggingPlace);
			win.draw(WinResizePlace);

			win.display();
		}
	}



	// Getters

	sf::Vector2u& getWinSize() {
		return windowSize;
	}

	sf::RectangleShape& getDraggPlace() {
		return draggingPlace;
	}

	sf::RectangleShape& getWinRePlace() {
		return WinResizePlace;
	}

	sf::Texture& getTexture() {
		return texture;
	}

	sf::Sprite& getSprite() {
		return sprite;
	}

	sf::RenderWindow& getWindow() {
		return win;
	}

};



class FloatingWindows {
	//std::vector<std::unique_ptr<sf::RenderWindow>> windows;
	//std::vector<std::unique_ptr<sf::Sprite>> sprites;
	//std::vector<std::unique_ptr<sf::Texture>> textures;

	//sf::RectangleShape draggingPlace;
	//sf::RectangleShape WinResizePlace;

	std::vector<std::unique_ptr<Window>> windows;


	sf::Vector2u windowSize;
	sf::Vector2u newSize;
	sf::Vector2i offset;
	sf::Vector2f sprOffset;
	sf::Vector2i currMouPos;

	enum class MOUSESTATE {
		WiD,	// Window Dragging
		WiRe,	// Window Resizing
		ImD,	// Image Dragging
		ImR,	// Image Rotating
		ImRe,	// Image Resizing

		Non		// None. No user input.
	};

	MOUSESTATE MouseState;



	bool isMouseInWindow;
	bool AlwaysOnTop;
public:
	FloatingWindows();

	// init new Floating window

	void init();

	// init winows

	void initWindow();

	void closeWindow(int id);

	void closeAllWindows();

	// init graphics

	void initSprite();

	// Draw

	void draw();

	void setWindowZIndex();

	void setWindowTransp(int id, BYTE transparencyValue);

	void eventHandler();

	//================= [End of Events] =================
//============================================================================


};




/*

class FloatingWindows {
	std::vector<std::unique_ptr<sf::RenderWindow>> windows;
	std::vector<std::unique_ptr<sf::Sprite>> sprites;
	std::vector<std::unique_ptr<sf::Texture>> textures;

	sf::Vector2u windowSize;
	sf::Vector2u newSize;
	sf::Vector2i offset;
	sf::Vector2f sprOffset;
	sf::Vector2i currMouPos;

	enum class MOUSESTATE {
		WiD,	// Window Dragging
		WiRe,	// Window Resizing
		ImD,	// Image Dragging
		ImR,	// Image Rotating
		ImRe,	// Image Resizing

		Non		// None. No user input.
	};

	MOUSESTATE MouseState;

	sf::RectangleShape draggingPlace;
	sf::RectangleShape WinResizePlace;

	bool isMouseInWindow;
	bool AlwaysOnTop;
public:
	FloatingWindows() {
		windowSize = { 500, 500 };
		offset = {};

		MouseState = MOUSESTATE::Non;

		AlwaysOnTop = false;

		isMouseInWindow = false;



		draggingPlace.setSize({ 35.0f, 35.0f });

		draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));

		draggingPlace.setOrigin(sf::Vector2f(draggingPlace.getLocalBounds().size.x, 0.0f));

		draggingPlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 0.0f));



		WinResizePlace.setSize(draggingPlace.getSize());

		WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));

		WinResizePlace.setOrigin(sf::Vector2f(WinResizePlace.getLocalBounds().size.x, WinResizePlace.getLocalBounds().size.y));

		WinResizePlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 1.0f));
	}

	// init new Floating window

	void init() {
		//return;
		initWindow();
		initSprite();
	}

	// init winows

	void initWindow() {
		windows.push_back(std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize), "Floating window"
			, sf::Style::None, sf::State::Windowed));
		setWindowZIndex();
	}

	void closeWindow(int id) {
		windows.at(id)->close();
		windows.erase(windows.begin() + id);
		sprites.erase(sprites.begin() + id);
		textures.erase(textures.begin() + id);
	}

	void closeAllWindows() {

		for (size_t i = 0; i < windows.size(); i++) {
			windows.at(i)->close();
		}

		sprites.clear();
		textures.clear();

		windows.clear();

	}

	// init graphics

	void initSprite() {
		std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>(paths.at(index));
		std::unique_ptr<sf::Sprite> sp = std::make_unique<sf::Sprite>(*tex);

		sf::Vector2f newSize;

		newSize.x = static_cast<float>(windowSize.x) / static_cast<float>(sprite.getTexture().getSize().x);
		newSize.y = static_cast<float>(windowSize.y) / static_cast<float>(sprite.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

		sp->setScale({ scale, scale });

		sp->setOrigin({ sp->getLocalBounds().size.x / 2, sp->getLocalBounds().size.y / 2 });
		sp->setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

		sprites.push_back(std::move(sp));
		textures.push_back(std::move(tex));
	}

	// Draw

	void draw() {
		if (windows.empty()) return;

		//size_t valueInd = ind - 1;


		for (size_t i = 0; i < windows.size(); i++) {

			if (windows.at(i)->isOpen()) {

				windows.at(i)->clear(sf::Color::Black);

				windows.at(i)->draw(*sprites.at(i));
				windows.at(i)->draw(draggingPlace);
				windows.at(i)->draw(WinResizePlace);

				windows.at(i)->display();
			}
		}

	}

	void setWindowZIndex() {
		for (size_t i = 0; i < windows.size(); i++) {
			if (AlwaysOnTop) {
				Print("Floating Window", "Always on top.");
				HWND hwnd = windows.at(i)->getNativeHandle();
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				//windows.at(i)->
			}
			else {
				Print("Floating Window", "Not always on top.");
				HWND hwnd = windows.at(i)->getNativeHandle();
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	}

	void setWindowTransp(int id, BYTE transparencyValue) {

		HWND hwnd = windows.at(id)->getNativeHandle();

		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		if (SetLayeredWindowAttributes(hwnd, 0, transparencyValue, LWA_ALPHA)) {
			Print("WindowFloating", "The window " + std::to_string(id) + " is transperant.");
			return;
		}

		Print("WindowFloating", "The window " + std::to_string(id) + " did not be transperant.");

		return;
	}

	void eventHandler() {

		for (size_t i = 0; i < windows.size(); i++) {
			while (const auto event = windows.at(i)->pollEvent()) {

				//================= [Window Events] =================

				if (windows.at(i)->hasFocus()) {
					//Print("FloatingWIndow", "Window " + std::to_string(i) + " has focus");
				}

				if (event->is<sf::Event::Closed>()) {
					closeWindow(i);
					break;
				}


				if (const auto* resized = event->getIf<sf::Event::Resized>()) {
					sf::View view(sf::FloatRect({ 0.0f, 0.0f }, sf::Vector2f(resized->size)));
					windows.at(i)->setView(view);

					windowSize = resized->size;

					WinResizePlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 1.0f));
					draggingPlace.setPosition(WindowSizeNor<float>(windowSize, 1.0f, 0.0f));

				}


				if (event->is<sf::Event::MouseEntered>()) {
					isMouseInWindow = true;
				}
				else if (event->is<sf::Event::MouseLeft>()) {
					if ((MouseState == MOUSESTATE::Non)) {
						if (draggingPlace.getFillColor().a > 0) {
							draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));
						}

						if (WinResizePlace.getFillColor().a > 0) {
							WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));
						}
					}
					isMouseInWindow = false;
				}


				//================= [Input Events] =================


				//===Keyboard Buttons

				if (const auto* KP = event->getIf<sf::Event::KeyPressed>()) {
					if (KP->scancode == sf::Keyboard::Scan::H) {
						AlwaysOnTop = !AlwaysOnTop;
						setWindowZIndex();
					}

					if (KP->scancode == sf::Keyboard::Scan::Escape) {
						closeWindow(i);
						break;
					}

					if (KP->scancode == sf::Keyboard::Scan::Space) {
						if (windows.at(i)->hasFocus()) {
							setWindowTransp(i, 175);
						}
					}

					if (KP->scancode == sf::Keyboard::Scan::R) {
						sprites.at(i)->setRotation(sf::Angle(-sf::degrees(0)));
					}

					if (KP->scancode == sf::Keyboard::Scan::S) {
						sf::Vector2f newSize;

						newSize.x = static_cast<float>(windowSize.x) / static_cast<float>(sprite.getTexture().getSize().x);
						newSize.y = static_cast<float>(windowSize.y) / static_cast<float>(sprite.getTexture().getSize().y);

						float scale = std::min(newSize.x, newSize.y);

						sprites.at(i)->setScale({ scale, scale });

						sprites.at(i)->setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
					}

				}

				//===Mouse Buttons===

				// Mouse wheel

				if (const auto* MW = event->getIf<sf::Event::MouseWheelScrolled>()) {
					if (MW->wheel == sf::Mouse::Wheel::Vertical && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LControl)) {
						if (MW->delta > 0) {
							windows.at(i)->setSize({ windows.at(i)->getSize().x, windows.at(i)->getSize().y + 20 });
						}
						else if (MW->delta < 0) {
							windows.at(i)->setSize({ windows.at(i)->getSize().x, windows.at(i)->getSize().y - 20 });
						}

					}
					else if (MW->wheel == sf::Mouse::Wheel::Vertical && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
						if (MW->delta > 0) {
							windows.at(i)->setSize({ windows.at(i)->getSize().x + 10, windows.at(i)->getSize().y });
						}
						else if (MW->delta < 0) {
							windows.at(i)->setSize({ windows.at(i)->getSize().x - 10, windows.at(i)->getSize().y });
						}

					}

					else if (MW->wheel == sf::Mouse::Wheel::Vertical) {
						if (MW->delta > 0) {
							sprites.at(i)->scale({ 1.05f, 1.05f });
						}
						if (MW->delta < 0) {
							sprites.at(i)->scale({ 0.95f, 0.95f });
						}
					}

				}

				// Mouse Button pressed

				if (const auto* MB = event->getIf<sf::Event::MouseButtonPressed>()) {

					if (MB->position.x > draggingPlace.getPosition().x - draggingPlace.getSize().x
						&& MB->position.y < draggingPlace.getPosition().y + draggingPlace.getSize().y) {
						if (MB->button == sf::Mouse::Button::Left) {
							MouseState = MOUSESTATE::WiD;
							//draggingPlace.setFillColor(sf::Color(255, 255, 255, 170));
						}
					}
					else if (MB->position.x > WinResizePlace.getPosition().x - WinResizePlace.getSize().x
						&& MB->position.y > WinResizePlace.getPosition().y - WinResizePlace.getSize().y) {
						if (MB->button == sf::Mouse::Button::Left) {
							MouseState = MOUSESTATE::WiRe;
							//WinResizePlace.setFillColor(sf::Color(255, 255, 255, 170));
						}
					}

					else {
						if (MB->button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
							MouseState = MOUSESTATE::WiD;
						}
						else if (MB->button == sf::Mouse::Button::Left) {
							MouseState = MOUSESTATE::ImD;
						}
						else if (MB->button == sf::Mouse::Button::Middle) {
							MouseState = MOUSESTATE::ImR;
						}

					}

					switch (MouseState)
					{
					case FloatingWindows::MOUSESTATE::WiD:
						offset = sf::Mouse::getPosition() - windows.at(i)->getPosition();
						break;
					case FloatingWindows::MOUSESTATE::WiRe:
						currMouPos = MB->position;
						break;
					case FloatingWindows::MOUSESTATE::ImD:
						sprOffset = sf::Vector2f(sf::Mouse::getPosition()) - sprites.at(i)->getPosition();
						break;
					case FloatingWindows::MOUSESTATE::ImR:
						currMouPos = MB->position;
						break;
					case FloatingWindows::MOUSESTATE::ImRe:
						break;
					case FloatingWindows::MOUSESTATE::Non:
						break;
					default:
						break;
					}

					//windows.at(i)->setPosition(sf::Mouse::getPosition());
				}

				// Mouse Button released

				else if (const auto* MB = event->getIf<sf::Event::MouseButtonReleased>()) {
					if (MouseState == MOUSESTATE::WiRe) {
						windows.at(i)->setSize(windows.at(i)->getSize() + newSize);
					}
					MouseState = MOUSESTATE::Non;
				}

				// Mouse Moved

				if (const auto* MP = event->getIf<sf::Event::MouseMoved>()) {
					if (MouseState == MOUSESTATE::Non) {
						if (MP->position.x > draggingPlace.getPosition().x - draggingPlace.getSize().x
							&& MP->position.y < draggingPlace.getPosition().y + draggingPlace.getSize().y) {
							draggingPlace.setFillColor(sf::Color(255, 255, 255, 75));
						}
						else {
							draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));
						}

						if (MP->position.x > WinResizePlace.getPosition().x - WinResizePlace.getSize().x
							&& MP->position.y > WinResizePlace.getPosition().y - WinResizePlace.getSize().y) {
							WinResizePlace.setFillColor(sf::Color(255, 255, 255, 75));
						}
						else {
							WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));
						}
					}

					sf::Angle an;
					float deltaX;




					switch (MouseState)
					{
					case FloatingWindows::MOUSESTATE::WiD:
						windows.at(i)->setPosition(sf::Mouse::getPosition() - offset);
						break;
					case FloatingWindows::MOUSESTATE::WiRe:
						//Print("WindowFloating", std::to_string(currMouPos.x - MP->position.x));
						Print("WindowFloating", std::to_string((MP->position.y - currMouPos.y)));
						newSize = sf::Vector2u(MP->position - currMouPos);
						break;
					case FloatingWindows::MOUSESTATE::ImD:
						sprites.at(i)->setPosition(sf::Vector2f(sf::Mouse::getPosition()) - sprOffset);
						break;
					case FloatingWindows::MOUSESTATE::ImR:
						an = sf::degrees(currMouPos.x - MP->position.x);
						deltaX = currMouPos.x - MP->position.x;
						sprites.at(i)->rotate(sf::Angle(-sf::degrees(deltaX)));
						currMouPos = MP->position;
						break;
					case FloatingWindows::MOUSESTATE::ImRe:
						break;
					case FloatingWindows::MOUSESTATE::Non:
						break;
					default:
						break;
					}

				}
				else {
					//currMouPos = sf::Mouse::getPosition();
				}


			}
		}

	}

	//================= [End of Events] =================
//============================================================================


};

*/